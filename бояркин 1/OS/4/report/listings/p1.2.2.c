#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define WHILE_DELAY 1
#define KILL_DELAY 1

// Обработчик сигнала
void signalHandler();
// Обработчик потока
void* threadHandler(void*);

int main() {
	pthread_t thread;

	// Создаем новый поток
	pthread_create(&thread, NULL, &threadHandler, NULL);

	sleep(KILL_DELAY);

	// Отправление сигнала прерывания
	pthread_kill(thread, SIGUSR2);

	while(1) {
		printf("Main thread message.\n");
		sleep(WHILE_DELAY);	
	}
}

void signalHandler() {
	printf("Signal handle.\n");

	// Завершаем текущий поток из обработчика сигнала
	pthread_exit(0);
}

void* threadHandler(void* ptr) {
	// Установка обработчика сигнала
	signal(SIGUSR2, signalHandler);

	while(1) {
		printf("Thread handler message.\n");
		sleep(WHILE_DELAY);	
	}
	
}
