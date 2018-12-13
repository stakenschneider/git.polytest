#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

static const char* PORT = "65100";

static const int BACKLOG = 5;
static const int BUFFER_SIZE = 1000;
static const int FLAGS = 0;

// Коллекция для хранения пар значений:
// сокет + идентификатор потока
std::map<SOCKET, std::shared_ptr<std::thread>> threads;
// Серверный сокет
SOCKET serverSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Обработчик клиентского потока
void* clientExecutor(void* clientSocket);
// Функция считывания строки символов с клиента
int readLine(SOCKET socket, char* buffer, int bufferSize, int flags);
// Функция отправки строки символов клиенту
int sendLine(SOCKET socket, char* buffer, int flags);
// Корректное закрытие сокета
void clearSocket(SOCKET socket);
// Завершение работы клиентского потока
void destroyClient(SOCKET socket);

int main(int argc, char** argv) {
	std::string port = PORT;
	if(argc < 2)
		std::cout << "Using default port: " << port << "." << std::endl;
	else
		port = argv[1];

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

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	std::cout << "Wait clients." << std::endl;

	while(true) {
		// Ждем подключения клиентов
		SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

		if(clientSocket == INVALID_SOCKET)
			continue;

		// Пробуем создать поток обработки клиентских сообщений
		auto bindThread = std::bind(&clientExecutor, std::placeholders::_1);
		auto sharedThread = std::make_shared<std::thread>(bindThread, &clientSocket);

		// Добавляем в коллекцию пару значений: сокет + идентификатор потока
		threads.insert(std::pair<SOCKET, std::shared_ptr<std::thread>>(clientSocket, sharedThread));
	}
}

void signalHandler(int sig) {
	// Для всех элементов коллекции
	for(auto& current: threads) {
		std::cout << "Try to finish client with socket " << current.first << "." << std::endl;
		// Закрываем клиентские сокеты
		clearSocket(current.first);
		std::cout << "Client socket " << current.first << " closed." << std::endl;
	}

	// Закрываем серверный сокет
	clearSocket(serverSocket);
	std::cout << "Server socket " << serverSocket << " closed." << std::endl;

	WSACleanup();

	exit(0x0);
}

void* clientExecutor(void* socket) {
	SOCKET clientSocket = *((SOCKET*) socket);

	std::cout << "Client thread with socket " << clientSocket <<" created." << std::endl;

	char buffer[BUFFER_SIZE];
	while(true) {
		// Ожидаем прибытия строки
		int result = readLine(clientSocket, buffer, BUFFER_SIZE, FLAGS);
		if(result < 0) {
			destroyClient(clientSocket);
			break;
		}

		if(strlen(buffer) <= 1) {
			destroyClient(clientSocket);
			break;
		}

		std::cout << "Client message: " << buffer << std::endl;

		// Отправляем строку назад
		result = sendLine(clientSocket, buffer, FLAGS);
		if(result < 0) {
			destroyClient(clientSocket);
			break;
		}
	}

	return nullptr;
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

int sendLine(SOCKET socket, char* buffer, int flags) {
	auto length = strlen(buffer);

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
	int result = send(socket, buffer, int(length), flags);
	return result;
}

void clearSocket(SOCKET socket) {
	// Завершение работы сокета
	int socketShutdown = shutdown(socket, SD_BOTH);
	if(socketShutdown != 0)
		std::cerr << "It's impossible to shutdown socket." << std::endl;

	// Закрытие сокета
	closesocket(socket);
}

void destroyClient(SOCKET socket) {
	std::cout << "It's impossible to receive message from client or send message to client." << std::endl;
	// Завершение работы сокета
	clearSocket(socket);
	// Отсоединение потока для удаления из коллекции
	threads.at(socket)->detach();
	// Удаление пары значений из коллекции по ключю
	threads.erase(socket);

	std::cout << "Client socket " << socket << " closed." << std::endl;
}

