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

// Структура сообщения
typedef struct {
	// Тип сообщения
	long type;
	// Текст сообщения
	char buffer[BUFFER_SIZE];
} Message;

static struct sembuf WAIT[1] = {1, -1, 0};
static struct sembuf UNLOCK[1] = {0, 1, 0};

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
	shmemory = shmget(key, sizeof(Message), 0666);
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
	semaphore = semget(key, 2, 0666);
	if(semaphore < 0) {
		perror("It's impossible to create semaphore.\n");	
		clearAndExit(0x4);
	}

	char buffer[BUFFER_SIZE];
	while(1) {
		// Получаем сообщение из входного потока
		bzero(buffer, BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);

		// Настройка для отправки сообщения
		int result = semop(semaphore, WAIT, 1);
		if(result < 0) {
			perror("It's imposibble to write.\n");
			clearAndExit(0x5);	
		}	

		// Записываем сообщение в разделяемую память
		sprintf(message->buffer, "%s", buffer);

		// Отправляем серверу сообщение что можно писать
		result = semop(semaphore, UNLOCK, 1);
		if(result < 0) {
			perror("It's imposibble to set write enable.\n");
			clearAndExit(0x6);	
		}
	}
		
	return 0x0;
}

void signalHandler(int sig) {
	// Корректно завершаем работу приложения
	clearAndExit(0x7);
}

void clearAndExit(int code) {
	// Устанавливаем обработчик сигнала по умолчанию
	signal(SIGINT, SIG_DFL);

	// Отключаем разделяемую память
	int result = shmdt(message);
	if(result < 0)
		perror("It's impossible to detach shm.\n");

	printf("Clear finished.\n");
	exit(code);
}

