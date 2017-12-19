#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char** argv) {
	// Вывод текущего системного времени
	char buffer[100];
	struct timeval timeV;
	gettimeofday(&timeV, NULL);
	time_t currentTime = timeV.tv_sec;
	strftime(buffer, 100, "%T", localtime(&currentTime));
	
	printf("%s.%.3ld Signal sent.\n", buffer, timeV.tv_usec);

	// Посылаем сигнал прерывания процессу, чей pid задан извне
	kill(atoi(argv[1]), SIGUSR1);
}
