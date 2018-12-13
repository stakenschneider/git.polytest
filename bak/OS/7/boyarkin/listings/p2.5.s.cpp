#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

static const char* PORT = "65100";
static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1024;
static const int NUMBER_LIMIT = 5;
static const int SLEEP_DURATION = 500;

struct  SharedData {
	int writerId;
};

// Массивы с потоками и сокетами
std::vector<HANDLE> writerThreads;
std::vector<SOCKET> clientSockets;

// Серверный сокет
SOCKET serverSocket;
// Критическая секция
CRITICAL_SECTION criticalSection;

struct SharedData sharedData;
int counter = 0;

// Обработчик потока-писателя
DWORD WINAPI threadWriterExecutor(LPVOID argument);

int main(int argc, char* argv[]) {
	// Получение порта
	std::string port = PORT;
	if(argc < 2)
		std::cout << "Using default port: " << port << "." << std::endl;
	else
		port = argv[1];

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
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* result = nullptr;
	int addressInfo = getaddrinfo(nullptr, port.data(), &hints, &result);
	if(addressInfo != 0) {
		std::cerr << "It's impossible to get address info." << std::endl;
		WSACleanup();
		return 0x2;
	}

	// Создание серверного сокета
	serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(serverSocket == INVALID_SOCKET) {
		std::cerr << "It's impossible to create socket." << std::endl;
		WSACleanup();
		return 0x3;
	}

	std::cout << "Server socket " << serverSocket << " created." << std::endl;

	// Биндим сервер на определенный адрес
	int serverBind = bind(serverSocket, result->ai_addr, int(result->ai_addrlen));
	if(serverBind == SOCKET_ERROR) {
		std::cerr << "It's impossible to bind socket." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0x4;
	}

	freeaddrinfo(result);

	// Слушаем сокет
	int serverListen = listen(serverSocket, BACKLOG);
	if(serverListen == SOCKET_ERROR) {
		std::cerr << "It's impossible to listen socket." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0x5;
	}

	// Инициализация критической секции
	InitializeCriticalSection(&criticalSection);
	sharedData.writerId = 0;

	std::cout << "Wait clients." << std::endl;

	int threadIndex = 0;
	while(true) {
		// Прием клиента
		SOCKET client = accept(serverSocket, nullptr, nullptr);
		
		if(client == INVALID_SOCKET)
			continue;

		// Добавляем сокет в коллекцию
		clientSockets.push_back(client);
		
		// Создаем поток
		HANDLE writer = CreateThread(nullptr, NULL, threadWriterExecutor, LPVOID(threadIndex), NULL, nullptr);
		if(!writer) {
			std::cerr << "It's impossible to create thread." << std::endl;
			return 0x6;
		}

		// Добавляем поток в коллекцию
		writerThreads.push_back(writer);
		++threadIndex;
	}

	return 0x0;
}

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	int writerId = int(argument);

	SOCKET writeSocket = clientSockets[writerId];

	char charNumber;
	char buffer[BUFFER_SIZE];
	while(true) {
		ZeroMemory(buffer, BUFFER_SIZE);

		// Получаем один байт
		while(true)
			if(recv(writeSocket, &charNumber, 1, NULL) > 0)
				break;

		EnterCriticalSection(&criticalSection);

		// Формируем новое число
		++counter;
		if(counter > NUMBER_LIMIT)
			counter = 0;

		printf("Client #%d. Get number = %c, counter = %d.\n", writerId, charNumber, counter);

		// Получаем число из байта
		int number = charNumber - 0x30;
		if(number <= 0 || number > 9) {
			counter = 0;
			printf("Client reset information to NULL.\n");
		}

		// Формируем результат
		if(number == counter)
			strcpy_s(buffer, "Yes!");
		else
			strcpy_s(buffer, "No!");

		// Отправляем результат читателю
		send(writeSocket, buffer, sizeof(buffer), NULL);

		LeaveCriticalSection(&criticalSection);
		Sleep(SLEEP_DURATION);
	}

	return NULL;
}
