#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Задержка перед следующим сигналом
#define DELAY 10

// Функция, изменяющая набор сигналов
void (*specialSignal(int sig, void (*handler) (int))) (int);
// Обработчик сигнала
void userHandler(int sig);

int main() {
	specialSignal(SIGUSR1, userHandler);
	
	while(1)
		pause();

	return 0;
}

void (*specialSignal(int sig, void (*handler) (int))) (int) {
	struct sigaction action;
	// Задаем обработчик сигнала
	action.sa_handler = handler;
	// Инициализируем набор сигналов
	sigemptyset(&action.sa_mask);
	// Добавляем сигнал прерывания SIGINT в набор
	sigaddset(&action.sa_mask, SIGINT);
	// Инициализируем флаг нулем
	action.sa_flags = 0;
	// Изменим действие процесса при получении сигнала прерывания
	if(sigaction(sig, &action, 0) < 0)
		return SIG_ERR;

	return action.sa_handler;
}

void userHandler(int sig) {
	// Обрабатываем прерывание

	// Если не тот сигнал
	if(sig != SIGUSR1) {
		perror("Wrong type of signal.\n");
		return;	
	}

	printf("SIGUSR catched.\n");

	// Отправляем сигнал прерывания
	kill(getpid(), SIGINT);

	// Устанавливаем задержку перед обработкой следующего сигнала
	sleep(DELAY);
}


