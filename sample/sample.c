#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define PAGE_OFFSET 0xFFFF800000000000
#include <sys/mman.h>
int main (int argc, char *argv[]) {
	while(1) {
		printf("Awake: %d\n", getpid());
		sleep(1);
	}
}
