#include "proc_fs.h"
#include "file_format.h"

#include "logs.h"
#include <string.h>
#include <stdlib.h>
#include <sys/user.h>

#define PAGE_OFFSET 0xffff800000000000ULL

void print_usage(char * argv[]) {
	err("Please invoke as: %s <pid to grab> <dump_file>\n", argv[0]);
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		print_usage(argv);
		return -1;
	}
	pid_t pid = atoi(argv[1]);

	PROC_HANDLE *process = attach_process(pid);
	if (!process)
		return -1;
	
	FILE_HANDLE *file = create_file(argv[2]);
	if (!file)
		return -1;

	struct user_regs_struct regs;
	struct user_fpregs_struct fpregs;
	get_regs(process, &regs, &fpregs);
	set_regs(file, &regs, &fpregs);

	int total_sections = get_section_count(process);
	log("Number of sections = %d\n", total_sections);
	SECTION section;

	for (int i = 0; i < total_sections; i++) {
		get_section(process, i, &section);
		SECTION_FILE_HEADER section_header;
		if(section.lower >= PAGE_OFFSET)
			continue;
		section_header.lower = section.lower;
		section_header.upper = section.upper;
		section_header.perms = section.perms;
		section_header.type = file_mapped;
		section_header.raw_data_offset = section.file_offset;
		strcpy(section_header.filename, section.file_name);
		
		if (!is_real_file(section.file_name) || is_section_changed(process, &section)) {
			char *raw_data = malloc(section.size);
			get_section_raw_data(process, &section, raw_data);
			section_header.type = raw;
			write_section(file, &section_header, raw_data);
			log("RAW section: %s\n", section.file_name);
			free(raw_data);
		}else
			write_section(file, &section_header, NULL);
	}
	close_file(file);
	detach_process(process);
	return 0;
}
