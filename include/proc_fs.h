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

}PROC_HANDLE;

PROC_HANDLE* attach_process(pid_t pid);




#endif
