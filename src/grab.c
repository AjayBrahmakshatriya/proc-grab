#include "proc_fs.h"
#include "logs.h"
#include <strings.h>
#include <stdlib.h>
void print_usage(char * argv[]) {
	err("Please invoke as: %s <pid to grab> -o <dump_file>\n", argv[0]);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		print_usage(argv);
		return -1;
	}
	pid_t pid = atoi(argv[1]);

	PROC_HANDLE *process = attach_process(pid);
	printf("Number of sections = %d\n", get_section_count(process));
	SECTION section;
	get_section(process, 0, &section);
	printf("%llx-%llx %d %llx %s\n", section.lower, section.upper, section.perms, section.file_offset, section.file_name);


}
