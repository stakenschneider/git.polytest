#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

static const char* PORT = "65100";
static const char* IP = "127.0.0.1";
static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1024;
static const int NUMBER_LIMIT = 5;
static const int SLEEP_DURATION = 200;

SOCKET clientSocket;

int main(int argc, char* argv[]) {
	// Получение порта и адреса 
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

	// Инициализация библиотеки
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

	// Получение информации об адресе
	struct addrinfo* addressResult = nullptr;
	int addressInfo = getaddrinfo(ip.data(), port.data(), &hints, &addressResult);
	if(addressInfo != 0) {
		std::cerr << "It's impossible to get address info." << std::endl;
		WSACleanup();
		return 0x2;
	}

	// Пробуем создать сокет и подключиться
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

	char buffer[BUFFER_SIZE];
	while(true) {
		// Формируем число
		char number = rand() % NUMBER_LIMIT + 0x30;

		// Посылаем число клиенту
		send(clientSocket, &number, 1, NULL);

		// Считываем ответ с сервера
		ZeroMemory(buffer, BUFFER_SIZE);
		if(recv(clientSocket, buffer, BUFFER_SIZE, NULL) > 0)
			std::cout << "Get answer from server: " << buffer << std::endl;

		Sleep(SLEEP_DURATION);
	}
}
