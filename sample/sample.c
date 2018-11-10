#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	while(1) {
		printf("Awake: %d\n", getpid());
		sleep(1);
	}
}
