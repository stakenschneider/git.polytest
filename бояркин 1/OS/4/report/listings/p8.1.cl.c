#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define PORT 65100
#define IP "127.0.0.1"

#define BACKLOG 5
#define BUFFER_SIZE 1000
#define IP_SIZE 16
#define FLAGS 0

// Клиентский сокет
int clientSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Функция считывания строки символов с сервера
int readLine(int socket, char* buffer, int bufferSize, int flags);
// Функция отправки строки символов серверу
int sendLine(int socket, char* buffer, int flags);
// Корректное закрытие сокета
void closeSocket(int socket);

int main(int argc, char** argv) {
	int port = PORT;
	char ip[IP_SIZE];

	strcpy(ip, IP);
	if(argc < 3) {
		printf("Using default ip: %s.\n", ip);
		printf("Using default port: %d.\n", port);
	}
	else {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
	}

	// Создание клиентского сокета
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(clientSocket < 0) {
		perror("It's impossible to create socket");
		return 0x1;	
	}
	
	printf("Client socket %d created.\n", clientSocket);

	// Структура, задающая адресные характеристики
	struct sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr.s_addr = inet_addr(ip);

	// Подключение к серверу
	int clientConnect = connect(clientSocket, (struct sockaddr *) &info, sizeof(info));
	if(clientConnect) {
		perror("It's impossible to connect");
		return 0x2;
	}

	printf("Connection established.\n");

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	printf("Ready to send messages.\n");

	char buffer[BUFFER_SIZE];
	while(1) {
		bzero(buffer, BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);

		if(strlen(buffer) == 0 || buffer[0] == '\n') {
			printf("Empty message, try to close client socket.\n");
			closeSocket(clientSocket);
			return 0x0;
		}

		// Отправляем строку на сервер
		int result = sendLine(clientSocket, buffer, FLAGS);
		if(result < 0) {
			printf("It's impossible to send message, try to close client socket.\n");
			closeSocket(clientSocket);
			return 0x0;
		}	

		// Ожидаем ответ строки
		result = readLine(clientSocket, buffer, BUFFER_SIZE, FLAGS);
		if(result < 0) {
			printf("It's impossible to receive message, try to close client socket.\n");
			closeSocket(clientSocket);
			return 0x0;
		}	
			
		printf("Server message: %s\n", buffer);
	}

	return 0x0;	
}

void signalHandler(int sig) {
	// Закрываем клиентский сокет
	closeSocket(clientSocket);
	printf("Client socket %d closed.\n", clientSocket);
	
	exit(0x0);
}

int readLine(int socket, char* buffer, int bufferSize, int flags) {
	// Очищаем буфер
	bzero(buffer, bufferSize);

	char resolvedSymbol = ' ';
	for(int index = 0; index < BUFFER_SIZE; ++index) {
		// Считываем по одному символу
		int readSize = recv(socket, &resolvedSymbol, 1, flags);
		if(readSize <= 0)
		    return -1;
		else if(resolvedSymbol == '\n')
		    break;
		else if(resolvedSymbol != '\r')
		    buffer[index] = resolvedSymbol;
	}
	
	return 0x0;
}

int sendLine(int socket, char* buffer, int flags) {
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

	// Отправляем строку серверу
	int result = send(socket, buffer, length, flags);
	return result;
}

void closeSocket(int socket) {
	// Завершение работы сокета
	int socketShutdown = shutdown(socket, SHUT_RDWR);
    	if(socketShutdown != 0)
		perror("It's impossible to shutdown socket");

	// Закрытие сокета
	int socketClose = close(socket);
    	if(socketClose != 0)
		perror("It's impossible to close socket");
}
