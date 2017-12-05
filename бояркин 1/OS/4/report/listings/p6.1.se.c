#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <signal.h>

#define DEFAULT_FILENAME "readfile.txt"
#define BUFFER_SIZE 100

// Тип присылаемого пакета
#define MESSAGE_TYPE 1L

// Текст ответа
#define RESPONSE_STRING "Message received!"
// Тип посылаемого ответа
#define RESPONSE_TYPE 2L

// Структура сообщения
typedef struct {
	// Тип сообщения
	long type;
	// Текст сообщения
	char buffer[BUFFER_SIZE];
} Message;

// Глобальная переменная для доступа к очереди
int queue;

// Обработчик сигнала прерывания с терминала для корректного удаления очереди
void signalHandler(int sig);

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

	// Получаем идентификатор очереди
	queue = msgget(key, IPC_CREAT | 0666);
	if(queue < 0) {
		perror("It's impossible to create queue.\n");	
		return 0x2;
	}
	
	// Обработка сигнала прерывания с терминала для корректного удаления очереди
	signal(SIGINT, signalHandler);

	Message message;

	while(1) {
		bzero(message.buffer, BUFFER_SIZE);

		// Принимаем сообщения определенного типа от клиентов 
		int result = msgrcv(queue, &message, sizeof(Message), MESSAGE_TYPE, 0);
		if(result < 0) {
			perror("It's impossible to receive message.\n");
			signalHandler(SIGINT);	
		}
		perror("Client's message: %s", message.buffer);

		// Формируем ответ
		message.type = RESPONSE_TYPE;
		bzero(message.buffer, BUFFER_SIZE);
		strcpy(message.buffer, RESPONSE_STRING);
		
		// Отправляем ответ
		result = msgsnd(queue, (void*) &message, sizeof(Message), 0);
		if(result != 0) {
			perror("It's impossible to send message.\n");
			signalHandler(SIGINT);
		}
	}
	
	return 0x0;
}

void signalHandler(int sig) {
	// Возвращаем обработчик сигнала по умолчанию
	signal(sig, SIG_DFL);

	// Удаляем очередь
	if(msgctl(queue, IPC_RMID, 0) < 0)
		perror("It's impossible to delete queue.\n");
	else
		perror("Queue has been successfully deleted.\n");
	
	exit(0x3);
}
