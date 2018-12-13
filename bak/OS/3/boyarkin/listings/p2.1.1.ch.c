#include <stdio.h>
#include <unistd.h>

#define DELAY 3

int main() {
	sleep(DELAY);
	printf("Child pid %d, ppid %d.\n", getpid(), getppid());
	printf("Child has been finished.\n");

	return 0x0;
}
