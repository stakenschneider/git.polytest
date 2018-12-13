#define _CRT_SECURE_NO_WARNINGS 0
#define _WINSOCK_DEPRECATED_NO_WARNINGS 0

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

static const int PORT = 65100;
static const char* IP = "127.0.0.1";

static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1000;
static const int IP_SIZE = 16;
static const int FLAGS = 0;

// Клиентский сокет
SOCKET clientSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Функция отправки строки символов серверу
int sendLine(char* buffer, int flags, const struct sockaddr_in* address);
// Корректное закрытие сокета
void clearSocket(SOCKET socket);

int main(int argc, char** argv) {
	int port = PORT;
	char ip[IP_SIZE];

	strcpy(ip, IP);
	if(argc < 3) {
		std::cout << "Using default ip: " << ip << "." << std::endl
			<< "Using default port: " << port << "." << std::endl;
	}
	else {
		strcpy(ip, argv[1]);
		port = std::stoi(argv[2]);
	}

	// Инициализация библиотеки
	WSADATA wsaData;
	int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(wsaStartup == SOCKET_ERROR) {
		std::cerr << "It's impossible to startup wsa." << std::endl;
		return 0x1;
	}

	// Создание клиентского сокета
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(clientSocket == INVALID_SOCKET) {
		std::cerr << "It's impossible to create socket." << std::endl;
		return 0x1;
	}

	std::cout << "Client socket " << clientSocket << " created." << std::endl;

	// Структура, задающая адресные характеристики
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip);

	std::cout << "Connection established." << std::endl;

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	std::cout << "Ready to send messages." << std::endl;

	char buffer[BUFFER_SIZE];

	while(true) {
		ZeroMemory(buffer, BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);

		if(strlen(buffer) == 0 || buffer[0] == '\n') {
			std::cerr << "Empty message, trying to close client socket." << std::endl;
			clearSocket(clientSocket);
			return 0x0;
		}

		// Отправляем строку на сервер
		int result = sendLine(buffer, FLAGS, &address);
		if(result < 0) {
			std::cerr << "It's impossible to send message to server." << std::endl;
			clearSocket(clientSocket);
			return 0x0;
		}

		size_t size = sizeof(struct sockaddr_in);

		// Ожидаем ответ сервера
		ZeroMemory(buffer, BUFFER_SIZE);
		result = recvfrom(clientSocket, buffer, BUFFER_SIZE, FLAGS, (struct sockaddr *) &address, (socklen_t *) &size);
		if(result < 0) {
			std::cerr << "It's impossible to receive message from server." << std::endl;
			clearSocket(clientSocket);
			return 0x0;
		}

		std::cout << "Server message: " << buffer;
	}

	return 0x0;
}

void signalHandler(int sig) {
	// Закрываем клиентский сокет
	clearSocket(clientSocket);
	std::cout << "Client socket " << clientSocket << " closed." << std::endl;

	WSACleanup();
	exit(0x0);
}

int sendLine(char* buffer, int flags, const struct sockaddr_in* address) {
	size_t length = strlen(buffer);

	// Перед отправкой сообщения добавляем в конец перевод строки
	if(length == 0)
		return -1;

	if(buffer[length - 1] != '\n') {
		if(length >= BUFFER_SIZE)
			return -1;

		buffer[length] = '\n';
	}

	length = strlen(buffer);

	// Отправляем строку серверу
	int result = sendto(clientSocket, buffer, int(length), flags, (struct sockaddr *) address, sizeof(struct sockaddr_in));
	return result;
}

void clearSocket(SOCKET socket) {
	// Закрытие сокета
	int socketClose = closesocket(socket);
	if(socketClose != 0)
		std::cerr << "It's impossible to close socket." << std::endl;
}
