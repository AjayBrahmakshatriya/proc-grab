#include "proc_fs.h"

#include <stdlib.h>
#include <errno.h>

PROC_HANDLE *attach_process(pid_t pid) {
	if(ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
		err("PTRACE on pid:%d failed with %d\n", pid, errno);
		return NULL;
	}
	wait(NULL);
	struct user_regs_struct regs;
	
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	log("RIP: %llx\n", regs.rip);
	log("Attach successful\n");
	PROC_HANDLE *handle = malloc(sizeof * handle);
	handle->pid = pid;
	handle->map_file = NULL;
	return handle;
}

void open_map_file(PROC_HANDLE *handle) {
	if (handle->map_file == NULL) {
		char filename[100];
		sprintf(filename, "/proc/%d/maps", handle->pid);
		handle->map_file = fopen(filename, "r");
		if (handle->map_file == NULL) {
			err("Error opening map file for pid: %d\n", handle->pid);
			return;
		}
	}	
}

int get_section_count(PROC_HANDLE *handle) {
	open_map_file(handle);
	fseek(handle->map_file, 0, SEEK_SET);
	int count = 0;
	int ch = 0;
	while(ch != EOF)
		if ((ch = fgetc(handle->map_file)) == '\n')
			count++;
	return count;
}

SECTION *get_section(PROC_HANDLE *handle, int section_id, SECTION *section) {
	if (section == NULL)
		return NULL;
	open_map_file(handle);
	fseek(handle->map_file, 0, SEEK_SET);
	int count = 0;
	int ch = 0;
	while (ch != EOF && count < section_id)
		if((ch = fgetc(handle->map_file)) == '\n')
			count++;
	if (count != section_id)
		err("Could not find section:%d in pid:%d\n", section_id, handle->pid);
	fscanf(handle->map_file, "%llx-%llx", &section->lower, &section->upper);
	section->size = section->upper - section->lower;
	fscanf(handle->map_file, " ");
	section->perms = 0;
	if (fgetc(handle->map_file) == 'r')
		section->perms |= rd;
	if (fgetc(handle->map_file) == 'w')
		section->perms |= wr;
	if (fgetc(handle->map_file) == 'x')
		section->perms |= ex;
	if (fgetc(handle->map_file) == 'p')
		section->perms |= pr;
	fscanf(handle->map_file, " %llx", &section->file_offset);
	fscanf(handle->map_file, " %*d:%*d %*d");
	fgetc(handle->map_file);
	if(fgetc(handle->map_file) != '\n'){
		fscanf(handle->map_file, " ");
		fscanf(handle->map_file, "%s", section->file_name);
	}else
		section->file_name[0] = 0;
	return section;
}

