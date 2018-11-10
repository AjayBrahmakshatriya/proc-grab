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
	return handle;
}
