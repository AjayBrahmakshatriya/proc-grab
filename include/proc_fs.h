#ifndef PROC_FS_H
#define PROC_FS_H

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

#include "logs.h"

typedef struct {
	pid_t pid;
	int ptrace_fd;
	FILE *map_file;
}PROC_HANDLE;

typedef enum {
	rd = 1,
	wr = 2,
	ex = 4,
	pr = 8,
} SECTION_PERMS;

typedef struct {
	unsigned long long lower;
	unsigned long long upper;
	unsigned long long size;
	unsigned long long file_offset;
	char file_name[128];
	SECTION_PERMS perms;
}SECTION;

PROC_HANDLE* attach_process(pid_t pid);
int get_section_count(PROC_HANDLE *handle);
SECTION *get_section(PROC_HANDLE *handle, int section_id, SECTION *section);
void detach_process(PROC_HANDLE *handle);



#endif
