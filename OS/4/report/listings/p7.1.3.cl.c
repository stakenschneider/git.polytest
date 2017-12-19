#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>

#define DEFAULT_FILENAME "readfile.txt"
#define BUFFER_SIZE 100
#define ITERATIONS_COUNT 10

static struct sembuf WAIT[1] = {1, -1, 0};
static struct sembuf RELEASE[1] = {2, 1, 0};
static struct sembuf LOCK[1] = {0, -1, 0};
static struct sembuf UNLOCK[1] = {0, 1, 0};

int* buffer;
int shmemory;
int semaphore;

// Обработчик сигнала прерывания (необходим для корректного завершения работы при прерывании)
void signalHandler(int sig);
// Функция для корректного завершения работы приложения
void clearAndExit(int code);

int main(int argc, char** argv) {
	char* filename = DEFAULT_FILENAME;

	if(argc < 2)
		printf("Using default filename: %s.\n", filename);
	else
		strcpy(filename, argv[1]);

	// Пробуем создать ключ
	key_t key = ftok(filename, 'Q');
	if(key == -1) {
		perror("It's impossible to get key for key file.\n");
		return 0x1;	
	}

	// Создаем shm
	shmemory = shmget(key, (BUFFER_SIZE + 1) * sizeof(int), 0666);
	if(shmemory < 0) {
		perror("It's impossible to create shm.\n");	
		return 0x2;
	}

	// Присоединяем shm в наше адресное пространство
	buffer = (int*) shmat(shmemory, 0, 0);
	if(buffer < 0) {
		perror("It's impossible to attach shm.\n");		
		return 0x3;
	}

	// Обработка сигнала прерывания с терминала для корректного завершения работы
	signal(SIGINT, signalHandler);

	// Создание группы из двух семафоров: первый показывает, что можно читать, второй - что можно писать
	semaphore = semget(key, 2, 0666);
	if(semaphore < 0) {
		perror("It's impossible to create semaphore.\n");	
		clearAndExit(0x4);
	}

	printf("Press \"Enter\" to start.\n");
	getchar();

	int send = 0;
	for(int index = 0; index < ITERATIONS_COUNT; ++index) {
		// Ожидаем хотя бы одну свободную ячейку
		int result = semop(semaphore, WAIT, 1);
		if(result < 0) {
			perror("It's imposibble to wait not empty.\n");
			clearAndExit(0x5);	
		}

		// Ожидаем доступа к разделяемой памяти
		result = semop(semaphore, LOCK, 1);
		if(result < 0) {
			perror("It's imposibble to set lock.\n");
			clearAndExit(0x6);	
		}

		++buffer[BUFFER_SIZE];
		printf("Add to cell #%d, value %d.\n", buffer[BUFFER_SIZE], send);
		buffer[buffer[BUFFER_SIZE]] = send++;
		
		// Ожидаем доступ к памяти
		result = semop(semaphore, UNLOCK, 1);
		if(result < 0) {
			perror("It's imposibble to set unlock.\n");
			clearAndExit(0x7);	
		}

		// Увеличиваем число занятых ячеек
		result = semop(semaphore, RELEASE, 1);
		if(result < 0) {
			perror("It's imposibble to release.\n");
			clearAndExit(0x8);	
		}
	}
		
	return 0x0;
}

void signalHandler(int sig) {
	// Корректно завершаем работу приложения
	clearAndExit(0x9);
}

void clearAndExit(int code) {
	// Устанавливаем обработчик сигнала по умолчанию
	signal(SIGINT, SIG_DFL);

	// Отключаем разделяемую память
	int result = shmdt(buffer);
	if(result < 0)
		perror("It's impossible to detach shm.\n");
	
	printf("Clear finished.\n");	
	exit(code);
}

