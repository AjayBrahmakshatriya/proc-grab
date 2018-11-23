#include "proc_fs.h"
#include "file_format.h"

#include "logs.h"
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>



#define TASK_SIZE64 (0x800000000000UL - 4096)
#define TASK_SIZE TASK_SIZE64
#define PAGE_OFFSET (void*)TASK_SIZE

#define xstr(a) str(a)
#define str(a) #a

#define RAW_BINARY_PATH xstr(_RAW_BINARY_PATH)

void jump_to_raw_binary(void *raw_binary_address);

int print_usage(char *argv[]) {
	err("Please invoke as: %s <dump_file>\n", argv[0]);
}

int main(int argc, char *argv[]) {
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
	int file_size_aligned = ((file_size + PAGE_SIZE - 1)/PAGE_SIZE)*PAGE_SIZE;
	void *load_address = PAGE_OFFSET - file_size_aligned;
	printf("File size aligned = %d at = %p\n", file_size_aligned, load_address);
	if(load_address != mmap(load_address, file_size_aligned, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0)){
		err("Failed to allocate memory for raw_binary with: %d\n", errno);
		return -1;
	}

	fread(load_address, file_size, 1, binary_file);
	fclose(binary_file);
	jump_to_raw_binary(load_address);


}
