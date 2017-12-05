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

static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1000;
static const int FLAGS = 0;

// Серверный сокет
SOCKET serverSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Функция отправки строки символов клиенту
int sendLine(char* buffer, int flags, const sockaddr_in* address);

int main(int argc, char** argv) {
	int port = PORT;
	if(argc < 2)
		std::cout << "Using default port: " << port << "." << std::endl;
	else
		port = std::stoi(argv[1]);

	// Инициализация библиотеки
	WSADATA wsaData;
	int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(wsaStartup == SOCKET_ERROR) {
		std::cerr << "It's impossible to startup wsa." << std::endl;
		return 0x1;
	}

	// Создание серверного сокета
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serverSocket == INVALID_SOCKET) {
		std::cerr << "It's impossible to create socket." << std::endl;
		return 0x2;
	}

	std::cout << "Server socket " << serverSocket << " created." << std::endl;

	// Структура, задающая адресные характеристики
	struct sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr.s_addr = htonl(INADDR_ANY);

	// Биндим сервер на определенный адрес
	int serverBind = bind(serverSocket, (struct sockaddr *) &info, sizeof(info));
	if(serverBind == SOCKET_ERROR) {
		std::cerr << "It's impossible to bind socket." << std::endl;
		return 0x3;
	}

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	std::cout << "Wait clients." << std::endl;

	char buffer[BUFFER_SIZE];
	while(true) {
		struct sockaddr_in* address = new sockaddr_in;
		size_t size = sizeof(struct sockaddr_in);

		// Ожидаем прибытия строки
		ZeroMemory(buffer, BUFFER_SIZE);
		int result = recvfrom(serverSocket, buffer, BUFFER_SIZE, FLAGS, (sockaddr *) address, (socklen_t *) &size);
		if(result < 0) {
			delete address;
			std::cerr << "It's impossible to receive message from client." << std::endl;
			continue;
		}

		if(strlen(buffer) <= 1) {
			delete address;
			std::cerr << "Message is empty." << std::endl;
			continue;
		}

		std::cout << "Client message: " << buffer;

		// Отправляем строку назад
		result = sendLine(buffer, FLAGS, address);
		if(result < 0) {
			delete address;
			std::cerr << "It's impossible to send message to client." << std::endl;
			continue;
		}

		delete address;
	}

	return 0x0;
}

void signalHandler(int sig) {
	// Закрываем серверный сокет
	int socketClose = closesocket(serverSocket);
	if(socketClose != 0)
		std::cerr << "It's impossible to close socket." << std::endl;
	else
		std::cout << "Server socket " << serverSocket << " closed." << std::endl;
	
	WSACleanup();
	exit(0x0);
}

int sendLine(char* buffer, int flags, const sockaddr_in* address) {
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

	// Отправляем строку клиенту
	int result = sendto(serverSocket, buffer, int(length), flags, (struct sockaddr *) address, sizeof(struct sockaddr_in));
	return result;
}
