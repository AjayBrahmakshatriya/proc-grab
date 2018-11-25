#include "proc_fs.h"
#include "file_format.h"

#include "logs.h"
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/personality.h>
#include "loader_params.h"


#define TASK_SIZE64 (0x800000000000UL - 4096)
#define TASK_SIZE TASK_SIZE64
#define PAGE_OFFSET (void*)TASK_SIZE

#define xstr(a) str(a)
#define str(a) #a
#define RAW_BINARY_PATH xstr(_RAW_BINARY_PATH)

#define PAGE_ALIGN(x) ((((x) + PAGE_SIZE-1)/PAGE_SIZE)*PAGE_SIZE)
void jump_to_raw_binary(void *raw_binary_address, void* params_address, void *stack_address);

int print_usage(char *argv[]) {
	err("Please invoke as: %s <dump_file>\n", argv[0]);
}

int main(int argc, char *argv[]) {
	int old_persona = personality(0xfffffffful);
	int persona = old_persona & (~READ_IMPLIES_EXEC);
	personality(persona);

	if (argc < 2) {
		print_usage(argv);
		return -1;
	}
	FILE_HANDLE *file = open_file(argv[1]);


	if (!file) 
		return -1;
	FILE *binary_file = fopen(RAW_BINARY_PATH, "r");
	if (!binary_file) {
		err("Cannot find binary loader file: %s\n", RAW_BINARY_PATH);
		return -1;
	}
	fseek(binary_file, 0, SEEK_END);
	int file_size = ftell(binary_file);
	fseek(binary_file, 0, SEEK_SET);
	int file_size_aligned = PAGE_ALIGN(file_size);
	char *load_address = (void*)(PAGE_SIZE + 0x100000000);
	if(load_address != mmap(load_address, file_size_aligned, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_FILE | MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0)){
		err("Failed to allocate memory for raw_binary with: %d\n", errno);
		return -1;
	}

	fread(load_address, file_size, 1, binary_file);
	fclose(binary_file);
	
	int params_size = sizeof(LOADER_PARAMS) + sizeof(SECTION_PARAMS) * file->header.number_of_sections + PAGE_SIZE; // Last page_size for stack
	int params_size_aligned = PAGE_ALIGN(params_size);
	char *params_address = load_address + file_size_aligned;
	if(params_address != mmap(params_address, params_size_aligned, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0)) {
		err("Failed to allocate memory for raw_binary_parameters: %d\n", errno);
		return -1;
	}
	LOADER_PARAMS *params = (LOADER_PARAMS*)params_address;
	params->unmap_address_start = (unsigned long long)(params_address + params_size_aligned);
	params->unmap_address_size = (unsigned long long)(PAGE_OFFSET - params->unmap_address_start);
	
	params->number_of_sections = file->header.number_of_sections;
	
	SECTION_PARAMS *section_params = (SECTION_PARAMS*)((char*)params + sizeof(LOADER_PARAMS));
	params->section_data_start = (unsigned long long)section_params;

	params->final_unmap_start = (unsigned long long) load_address;
	params->final_unmap_end = (unsigned long long) params->unmap_address_start;;


	for (int sec_id = 0; sec_id < file->header.number_of_sections; sec_id++) {
		SECTION_FILE_HEADER section_header;
		read_section(file, &section_header);
		section_params->start_address = section_header.lower;
		section_params->size = section_header.upper - section_header.lower;
		section_params->proto = 0;
		section_params->flags = 0;
		if(section_header.perms & rd)
			section_params->proto |= PROT_READ;
		if(section_header.perms & wr)
			section_params->proto |= PROT_WRITE;
		if(section_header.perms & ex)
			section_params->proto |= PROT_EXEC;
		if(section_header.perms & pr)
			section_params->flags |= MAP_PRIVATE;
		else
			section_params->flags |= MAP_SHARED;

		section_params->flags |= MAP_FIXED;
		if(section_header.type == raw) {
			section_params->flags |= MAP_ANONYMOUS;
			section_params->is_raw = 1;
			realpath(argv[1], section_params->filename);
			section_params->offset = section_header.raw_data_offset;
			section_params->open_flags = O_RDONLY;
		}else {
			section_params->flags |= MAP_FILE;
			section_params->is_raw = 0;
			strcpy(section_params->filename, section_header.filename);
			section_params->offset = section_header.raw_data_offset;
			section_params->open_flags = O_RDONLY;
		}
		section_params++;
	}
	close_file(file);


	if(!fork()) {
		return 0;
	}else {
		log("process id: %d\n", getpid());
		jump_to_raw_binary(load_address, params, params_address + params_size_aligned - 16);
	}
}
