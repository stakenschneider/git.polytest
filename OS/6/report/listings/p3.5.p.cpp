#define _WINSOCK_DEPRECATED_NO_WARNINGS 0;

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

// Порт
static const char* PORT = "65100";

// Количество обрабатывающих потоков
static const int COUNT_OF_THREADS = 8;
// Размер буфера для передачи
static const int BUFFER_SIZE = 1024;
// Флаги
static const int FLAGS = 0;

// Структура посылки
struct OverlappedConnection: public OVERLAPPED {
	int client_number;
	SOCKET socket_handle;
	char* buffer;

	enum {
		op_type_send,
		op_type_recv
	} op_type;
};

// Список всех клиентских соединений для корректного завершения их работы
std::map<SOCKET, OverlappedConnection*> clients;
// Серверный сокет
SOCKET serverSocket;

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Закрывает все соединения и завершает работу сервера
void closeClientConnection(SOCKET clientSocket);
// Закрывает соединение с конкретным клиентом
void closeAllConnections();

// Функция потока сервера для обслуживания порта завершения
DWORD WINAPI threadExecutor(HANDLE ioPort);

int main(int argc, char** argv) {
	std::string port = PORT;
	if(argc < 2)
		std::cout << "Using default port: " << port << "." << std::endl;
	else
		port = argv[1];

	// Инициализация библиотеки
	WSADATA wsaData;
	int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(wsaStartup == SOCKET_ERROR) {
		std::cerr << "It's impossible to startup wsa." << std::endl;
		return 0x1;
	}

	// Создаем порт завершения
	HANDLE ioPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, NULL);
	if(!ioPort) {
		std::cerr << "It's impossible to create competition port." << std::endl;
		WSACleanup();
		return 0x2;
	}

	// Задаем параметры прослушивающего сокета сервера
	serverSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, WSA_FLAG_OVERLAPPED);
	if(serverSocket == INVALID_SOCKET) {
		std::cerr << "It's impossible to create socket." << std::endl;
		WSACleanup();
		return 0x3;
	}

	std::cout << "Server socket " << serverSocket << " created." << std::endl;

	// Используем ранее созданный порт завершения
	if(!CreateIoCompletionPort(HANDLE(serverSocket), ioPort, NULL, NULL)) {
		std::cerr << "It's impossible to create io competition port." << std::endl;
		WSACleanup();
		return 0x4;
	}

	// Заполнение адресной структуры
	SOCKADDR_IN sockaddrIn;
	ZeroMemory(&sockaddrIn, sizeof(sockaddrIn));
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(std::stoi(port.data()));
	sockaddrIn.sin_addr.s_addr = INADDR_ANY;

	// Биндим сервер на определенный адрес
	int serverBind = bind(serverSocket, LPSOCKADDR(&sockaddrIn), sizeof(sockaddrIn));
	if(serverBind == SOCKET_ERROR) {
		std::cerr << "It's impossible to bind socket." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0x5;
	}

	// Прослушиваем сокет
	int serverListen = listen(serverSocket, SOMAXCONN);
	if(serverListen == SOCKET_ERROR) {
		std::cerr << "It's impossible to listen socket." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0x6;
	}

	// Создаем набор потоков для обслуживания сообщений от порта завершения
	DWORD threadId;
	for(int threadIndex = 0; threadIndex < COUNT_OF_THREADS; ++threadIndex)
		if(!CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(threadExecutor), ioPort, NULL, &threadId)) {
			std::cerr << "It's impossible to create thread." << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			return 0x7;
		}

	// Обработка прерывания для корректного завершения приложения
	signal(SIGINT, signalHandler);

	std::cout << "Wait clients." << std::endl;

	int clientIndex = 0;
	while(true) {
		// Ждем подключения клиентов
		SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

		if(clientSocket == INVALID_SOCKET)
			continue;

		if(!CreateIoCompletionPort(HANDLE(clientSocket), ioPort, NULL, NULL)) {
			std::cerr << "It's impossible to create io competition port." << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			return 0x8;
		}

		// Создаем и заполняем overlapped структуру
		OverlappedConnection* overlappedConnection = new OverlappedConnection;
		overlappedConnection->socket_handle = clientSocket;
		overlappedConnection->op_type = OverlappedConnection::op_type_recv;
		overlappedConnection->buffer = new char[BUFFER_SIZE];
		overlappedConnection->hEvent = nullptr;
		overlappedConnection->client_number = ++clientIndex;

		// Добавляем в список для корретного завершения
		clients.insert(std::pair<SOCKET, OverlappedConnection*>(clientSocket, overlappedConnection));

		std::cout << "Client #" << clientIndex << " has been connected. Client socket " << clientSocket << "." << std::endl;

		WSABUF buffer;
		buffer.buf = overlappedConnection->buffer;
		buffer.len = BUFFER_SIZE;

		DWORD countOfBytesRecv;
		if(!WSARecv(overlappedConnection->socket_handle, &buffer, 1, &countOfBytesRecv, nullptr, overlappedConnection, nullptr))
			std::cout << "WSA recv error." << std::endl;
	}
}

DWORD WINAPI threadExecutor(HANDLE ioPort) {
	DWORD countOfBytesTransferred, key;
	LPOVERLAPPED overlapped;

	while(true) {
		// Получаем информацию о завершении операции
		if(GetQueuedCompletionStatus(ioPort, &countOfBytesTransferred, PULONG_PTR(&key), &overlapped, INFINITE)) {
			// Операция завершена успешно
			OverlappedConnection* overlappedConnection = (OverlappedConnection*) overlapped;

			// Определяем тип завершенной операции и выполняем соответствующие действия
			switch(overlappedConnection->op_type) {
			case OverlappedConnection::op_type_send:
				//Завершена отправка данных
				delete[] overlappedConnection->buffer;
				delete overlappedConnection;
				break;

			case OverlappedConnection::op_type_recv:
				//Завершен приём данных
				if(!countOfBytesTransferred)
					closeClientConnection(overlappedConnection->socket_handle);

				overlappedConnection->buffer[countOfBytesTransferred] = '\0';

				std::cout << "Client #" << overlappedConnection->client_number << " received message: " << overlappedConnection->buffer << std::endl;

				if(send(overlappedConnection->socket_handle, overlappedConnection->buffer, countOfBytesTransferred, NULL) == SOCKET_ERROR)
					std::cout << "Send message error." << std::endl;

				WSABUF buffer;
				buffer.buf = overlappedConnection->buffer;
				buffer.len = BUFFER_SIZE;

				DWORD countOfBytesRecv;
				if(!WSARecv(overlappedConnection->socket_handle, &buffer, 1, &countOfBytesRecv, nullptr, overlappedConnection, nullptr))
					std::cout << "WSA recv error." << std::endl;

				break;
			}
		}
		else {
			if(!overlapped)
				closeAllConnections();
			else
				closeClientConnection(((OverlappedConnection*) overlapped)->socket_handle);
		}
	}
}

void signalHandler(int sig) {
	// Закрываем все соединения на сигнал прерывания
	closeAllConnections();
}

void closeClientConnection(SOCKET clientSocket) {
	// Закрытие конкретного клиентского соединения

	std::cout << "Client #" << clients.at(clientSocket)->client_number << " disconnected. Client socket " << clientSocket << "." << std::endl;

	delete clients.at(clientSocket);
	clients.erase(clientSocket);
	closesocket(clientSocket);
}

void closeAllConnections() {
	// Закрытие всех клиентских соединений, завершение работы приложения
	for(auto& current: clients) {
		SOCKET clientSocket = current.second->socket_handle;

		std::cout << "Client #" << current.second->client_number << " disconnected. Client socket " << clientSocket << "." << std::endl;

		delete clients.at(clientSocket);
		closesocket(clientSocket);
	}

	clients.clear();

	closesocket(serverSocket);
	WSACleanup();

	exit(0x0);
}

