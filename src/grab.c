#include "proc_fs.h"
#include "file_format.h"

#include "logs.h"
#include <string.h>
#include <stdlib.h>
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

	int total_sections = get_section_count(process);
	printf("Number of sections = %d\n", total_sections);
	SECTION section;

	for (int i = 0; i < total_sections; i++) {
		get_section(process, i, &section);
		SECTION_FILE_HEADER section_header;
		section_header.lower = section.lower;
		section_header.upper = section.upper;
		section_header.perms = section.perms;
		section_header.type = file_mapped;
		strcpy(section_header.filename, section.file_name);
		write_section(file, &section_header, NULL);
	}
	close_file(file);
	detach_process(process);
	return 0;
}
