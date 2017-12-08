#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// Обработчик сигнала
void handler() {
	// Вывод текущего системного времени
	char buffer[100];
	struct timeval timeV;
	gettimeofday(&timeV, NULL);
	time_t currentTime = timeV.tv_sec;
	strftime(buffer, 100, "%T", localtime(&currentTime));
	
	printf("%s.%.3ld Signal handle.\n", buffer, timeV.tv_usec);

	exit(0);
}

int main() {
	printf("Parent pid %d, ppid %d.\n", getpid(), getppid());

	// Задаем обработчик сигнала
	signal(SIGUSR1, handler);
	while(1);
}
