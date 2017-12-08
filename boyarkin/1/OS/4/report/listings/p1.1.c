#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Обработчик сигнала
static void handler(int sig);

int main() {
	printf("Parent pid %d, ppid %d.\n", getpid(), getppid());

	// Задаем сигналам SIGUSR1, SIGUSR2 одинаковый обработчик сигнала
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	// Сигнал прерывания по умолчанию
	signal(SIGINT, SIG_DFL);
	// Сигнал прерывания игнорируется
	signal(SIGCHLD, SIG_IGN);

	if(!fork()) {
		printf("Child pid %d, ppid %d.\n", getpid(), getppid());
		// Отправка прерывания родительскому процессу
		if(kill(getppid(), SIGUSR1)) {
			perror("It's impossible to send SIGUSR1.\n");
			exit(0x1);
		}

		printf("Signal has been successfully sent.\n");
		return 0x0;
	}

	// Ожидание сигнала
	while(1)
		pause();

	printf("Exit message.\n");

	return 0x0;
}

static void handler(int sig) {
	printf("Signal handle pid %d, ppid %d.\n", getpid(), getppid());

	// Возвращаем обработчик по умолчанию
	signal(sig, SIG_DFL);
}
