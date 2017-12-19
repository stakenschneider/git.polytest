#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <map>

#define PORT 65100

#define BACKLOG 5
#define BUFFER_SIZE 1000
#define FLAGS 0

// Серверный сокет
int serverSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Функция отправки строки символов клиенту
int sendLine(char* buffer, int flags, const sockaddr_in* address);

int main(int argc, char** argv) {
	int port = PORT;
	if(argc < 2)
		printf("Using default port: %d.\n", port);
	else
		port = atoi(argv[1]);

	// Создание серверного сокета
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serverSocket < 0) {
		perror("It's impossible to create socket");
		return 0x1;	
	}
	
	printf("Server socket %d created.\n", serverSocket);

	// Структура, задающая адресные характеристики
	struct sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr.s_addr = htonl(INADDR_ANY);

	// Биндим сервер на определенный адрес
	int serverBind  = bind(serverSocket, (struct sockaddr *) &info, sizeof(info));
	if(serverBind < 0) {
		perror("It's impossible to bind socket");
		return 0x2;	
	}

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	printf("Wait clients.\n");

	char buffer[BUFFER_SIZE];
	while(1) {
		struct sockaddr_in* address = new sockaddr_in;
		size_t size = sizeof(struct sockaddr_in);

		// Ожидаем прибытия строки
		bzero(buffer, BUFFER_SIZE);
		int result = recvfrom(serverSocket, buffer, BUFFER_SIZE, FLAGS, (sockaddr *) address, (socklen_t *) &size);
		if(result < 0) {
			delete address;
			printf("It's impossible to receive message from client.\n");
			continue;
		}

		if(strlen(buffer) <= 1) {
			delete address;
			printf("Message is empty.\n");
			continue;
		}
			
		printf("Client message: %s", buffer);

		// Отправляем строку назад
		result = sendLine(buffer, FLAGS, address);
		if(result < 0) {
			delete address;
			printf("It's impossible to send message to client.\n");
			continue;
		}

		delete address;
	}

	return 0x0;	
}

void signalHandler(int sig) {
	// Закрываем серверный сокет
	int socketClose = close(serverSocket);
    	if(socketClose != 0)
		perror("It's impossible to close socket");
	else
		printf("Server socket %d closed.\n", serverSocket);
	
	exit(0x0);
}

int sendLine(char* buffer, int flags, const sockaddr_in* address) {
	unsigned int length = strlen(buffer);
	
	// Перед отправкой сообщения добавляем в конец перевод строки
	if(length == 0)
		return -1;
	else if(buffer[length - 1] != '\n') {
		if(length >= BUFFER_SIZE)
			return -1;
		else
			buffer[length] = '\n';
		
	}
	
	length = strlen(buffer);

	// Отправляем строку клиенту
	int result = sendto(serverSocket, buffer, length, flags, (struct sockaddr *) address, sizeof(struct sockaddr_in));
	return result;
}
