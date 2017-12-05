#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>

#define DEFAULT_FILENAME "readfile.txt"
#define BUFFER_SIZE 100

// Тип посылаемого пакета
#define MESSAGE_TYPE 1L
// Тип присылаемого ответа
#define RESPONSE_TYPE 2L

// Структура сообщения
typedef struct {
	// Тип сообщения
	long type;
	// Текст сообщения
	char buffer[BUFFER_SIZE];
} Message;

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
	int queue = msgget(key, 0);
	if(queue < 0) {
		perror("It's impossible to create queue.\n");	
		return 0x2;
	}

	Message message;

	while(1) {
		// Формируем сообщение

		// Считываем сообщение из потока ввода
		bzero(message.buffer, BUFFER_SIZE);
		fgets(message.buffer, BUFFER_SIZE, stdin);

		// Задаем тип сообщения
		message.type = MESSAGE_TYPE;

		// Отправка сообщения
		int result = msgsnd(queue, (void*) &message, sizeof(Message), 0);
		if(result != 0) {
			perror("It's impossible to send message.\n");
			return 0x3;
		}

		// Получение ответа
		result = msgrcv(queue, &message, sizeof(Message), RESPONSE_TYPE, 0);
		if(result < 0) {
			perror("It's impossible to receive message.\n");
			return 0x4;	
		}

		printf("Server's message: %s\n", message.buffer);		
	}
	
	return 0x0;
}
