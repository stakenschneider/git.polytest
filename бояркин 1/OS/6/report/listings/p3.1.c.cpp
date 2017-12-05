#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

static const char* PORT = "65100";
static const char* IP = "127.0.0.1";

static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1000;
static const int FLAGS = 0;

// Клиентский сокет
SOCKET clientSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Функция считывания строки символов с сервера
int readLine(SOCKET socket, char* buffer, int bufferSize, int flags);
// Функция отправки строки символов серверу
int sendLine(SOCKET socket, std::string message, int flags);
// Корректное закрытие сокета
void clearSocket(SOCKET socket);

int main(int argc, char** argv) {
	std::string port = PORT;
	std::string ip = IP;

	if(argc < 3) {
		std::cout << "Using default ip: " << ip << "." << std::endl;
		std::cout << "Using default port: " << port << "." << std::endl;
	}
	else {
		ip = argv[1];
		port = argv[2];
	}

	WSADATA wsaData;
	int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(wsaStartup != 0) {
		std::cerr << "It's impossible to startup wsa." << std::endl;
		return 0x1;
	}

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	struct addrinfo* addressResult = nullptr;
	int addressInfo = getaddrinfo(ip.data(), port.data(), &hints, &addressResult);
	if(addressInfo != 0) {
		std::cerr << "It's impossible to get address info." << std::endl;
		WSACleanup();
		return 0x2;
	}

	for(auto currentPtr = addressResult; currentPtr != nullptr; currentPtr = currentPtr->ai_next) {
		clientSocket = socket(currentPtr->ai_family, currentPtr->ai_socktype, currentPtr->ai_protocol);
		if(clientSocket == INVALID_SOCKET) {
			std::cerr << "It's impossible to create socket." << std::endl;
			WSACleanup();
			return 0x3;
		}

		auto serverConnection = connect(clientSocket, currentPtr->ai_addr, int(currentPtr->ai_addrlen));
		if(serverConnection != SOCKET_ERROR)
			break;

		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}
	
	freeaddrinfo(addressResult);

	if(clientSocket == INVALID_SOCKET) {
		std::cerr << "It's impossible to create connection." << std::endl;
		WSACleanup();
		return 0x4;
	}

	std::cout << "Socket has been successfully created." << std::endl
	          << "Connection established." << std::endl;

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	std::string message;
	char buffer[BUFFER_SIZE];

	while(true) {
		message.clear();
		std::getline(std::cin, message);

		if(message.empty() || (message.size() == 1 && message.back() == '\n')) {
			std::cout << "Empty message, try to close client socket." << std::endl;
			clearSocket(clientSocket);
			WSACleanup();
			return 0x0;
		}

		// Отправляем строку на сервер
		int result = sendLine(clientSocket, message, FLAGS);
		if(result < 0) {
			std::cout << "It's impossible to send message, try to close client socket." << std::endl;
			clearSocket(clientSocket);
			WSACleanup();
			return 0x0;
		}

		// Ожидаем ответ строки
		result = readLine(clientSocket, buffer, BUFFER_SIZE, FLAGS);
		if(result < 0) {
			std::cout << "It's impossible to receive message, try to close client socket." << std::endl;
			clearSocket(clientSocket);
			WSACleanup();
			return 0x0;
		}

		std::cout << "Server message: " << buffer << std::endl;
	}
}

void signalHandler(int sig) {
	// Закрываем клиентский сокет
	clearSocket(clientSocket);
	std::cout << "Client socket " << clientSocket << " closed." << std::endl;

	WSACleanup();

	exit(0x0);
}

int readLine(SOCKET socket, char* buffer, int bufferSize, int flags) {
	// Очищаем буфер
	ZeroMemory(buffer, bufferSize);

	char resolvedSymbol = ' ';
	for(int index = 0; index < BUFFER_SIZE; ++index) {
		// Считываем по одному символу
		int readSize = recv(socket, &resolvedSymbol, 1, flags);
		if(readSize <= 0)
			return -1;
		
		if(resolvedSymbol == '\n')
			break;
		
		if(resolvedSymbol != '\r')
			buffer[index] = resolvedSymbol;
	}

	return 0x0;
}

int sendLine(SOCKET socket, std::string message, int flags) {
	// Перед отправкой сообщения добавляем в конец перевод строки
	if(message.empty())
		return -1;
	
	if(message.back() != '\n')
		message += '\n';

	// Отправляем строку на сервер
	return send(socket, message.data(), int(message.size()), flags);
}

void clearSocket(SOCKET socket) {
	// Завершение работы сокета
	int socketShutdown = shutdown(socket, SD_BOTH);
	if(socketShutdown == SOCKET_ERROR)
		std::cerr << "It's impossible to shutdown socket." << std::endl;

	// Закрытие сокета
	int socketClose = closesocket(socket);
}
