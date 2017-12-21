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
#define SET_WRITE_ENABLE 1

// Структура сообщения
typedef struct {
	// Тип сообщения
	long type;
	// Текст сообщения
	char buffer[BUFFER_SIZE];
} Message;

static struct sembuf LOCK[1] = {0, -1, 0};
static struct sembuf RELEASE[1] = {1, 1, 0};

Message* message;
int semaphore;
int shmemory;

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
	shmemory = shmget(key, sizeof(Message), IPC_CREAT | 0666);
	if(shmemory < 0) {
		perror("It's impossible to create shm.\n");	
		return 0x2;
	}

	// Присоединяем shm в наше адресное пространство
	message = (Message*) shmat(shmemory, 0, 0);
	if(message < 0) {
		perror("It's impossible to attach shm.\n");		
		return 0x3;
	}

	// Обработка сигнала прерывания с терминала для корректного завершения работы
	signal(SIGINT, signalHandler);

	// Создание группы из двух семафоров: первый показывает, что можно читать, второй - что можно писать
	semaphore = semget(key, 2, IPC_CREAT | 0666);
	if(semaphore < 0) {
		perror("It's impossible to create semaphore.\n");	
		clearAndExit(0x4);
	}

	// Устанавливаем второй семофор в единицу (можно писать)
	int result = semop(semaphore, SET_WRITE_ENABLE, 1);
	if(result < 0) {
		perror("It's imposibble to set write enable.\n");
		clearAndExit(0x5);	
	}

	while(1) {
		// Ожидаем начало работы клиента
		result = semop(semaphore, LOCK, 1);
		if(result < 0) {
			perror("It's imposibble to lock.\n");
			clearAndExit(0x6);	
		}	

		printf("Client's message: %s", message->buffer);

		// Устанавливаем второй семофор в единицу (можно писать)
		result = semop(semaphore, RELEASE, 1);
		if(result < 0) {
			perror("It's imposibble to release.\n");
			clearAndExit(0x7);	
		}
	}
		
	return 0x0;
}

void signalHandler(int sig) {
	// Корректно завершаем работу приложения
	clearAndExit(0x8);
}

void clearAndExit(int code) {
	// Устанавливаем обработчик сигнала по умолчанию
	signal(SIGINT, SIG_DFL);

	// Отключаем разделяемую память
	int result = shmdt(message);
	if(result < 0)
		perror("It's impossible to detach shm.\n");

	// Удаление shm
	result = shmctl(shmemory, IPC_RMID, 0);
	if(result < 0)
		perror("It's impossible to delete shm.\n");

	// Удаление семафоров
	result = semctl(semaphore, 0, IPC_RMID);
	if(result < 0)
		perror("It's impossible to delete semaphore.\n");	
	
	printf("Clear finished.\n");	
	exit(code);
}

