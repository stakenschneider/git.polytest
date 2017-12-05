#define _WINSOCK_DEPRECATED_NO_WARNINGS 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <thread>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
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

static const int SERVICE_ERROR = -1;

// Аргументы командной строки для управления службой
static const char* INSTALL_PARAMETR = "install";
static const char* REMOVE_PARAMETR = "remove";
static const char* START_PARAMETR = "start";

// Название службы, путь к службе, путь к файлу логирования
static constexpr char* SERVICE_NAME = "ECHO_SERVER";
static const char* SERVICE_PATH = "D:\\temp\\p3.6.p.exe";
static const char* LOG_PATH = "D:\\temp\\p3.6.p.log";

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

SERVICE_STATUS serverStatus;
SERVICE_STATUS_HANDLE handleServerStatus;

std::ofstream* out = nullptr;

void log(char* message);
int installEchoServer();
int removeEchoServer();
int startEchoServer();
int serviceMain(int argc, char** argv);

// Обработчик сигнала прерывания (корректное завершение приложения)
void signalHandler(int sig);
// Закрывает все соединения и завершает работу сервера
void closeClientConnection(SOCKET clientSocket);
// Закрывает соединение с конкретным клиентом
void closeAllConnections();

// Функция потока сервера для обслуживания порта завершения
DWORD WINAPI threadExecutor(HANDLE ioPort);

int main(int argc, char** argv) {
	if(argc == 1) {
		// Выполнение основных задач службы
		SERVICE_TABLE_ENTRY serviceTable[] = {
			{ SERVICE_NAME, LPSERVICE_MAIN_FUNCTION(serviceMain) },
			{ nullptr, nullptr }
		};

		if(!StartServiceCtrlDispatcher(serviceTable)) {
			std::cerr << "It's impossible to start service dispatcher." << std::endl;
			return 0x1;
		}
	}
	else if(!strcmp(argv[argc - 1], INSTALL_PARAMETR))
		installEchoServer();
	else if(!strcmp(argv[argc - 1], REMOVE_PARAMETR))
		removeEchoServer();
	else if(!strcmp(argv[argc - 1], START_PARAMETR))
		startEchoServer();

	return 0x0;
}

void log(char* message) {
	// Логирование

	if(out == nullptr)
		out = new std::ofstream();

	if(!out->is_open())
		out->open(LOG_PATH);

	if(!out->is_open())
		return;

	*out << " PID " << GetCurrentProcessId() << ": " << message << std::endl;

	out->close();
}

int installEchoServer() {
	// Создание службы

	SC_HANDLE serviceManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
	if(!serviceManager) {
		log("It's impossible to open service control manager.");
		return SERVICE_ERROR;
	}

	SC_HANDLE service = CreateService(
		serviceManager, SERVICE_NAME, SERVICE_NAME,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL, SERVICE_PATH, nullptr, nullptr, nullptr, nullptr, nullptr );
	if(!service) {
		log("It's impossible to create service.");
		CloseServiceHandle(serviceManager);
		return SERVICE_ERROR;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceManager);

	log("Service has been successfully installed.");
	return NULL;
}

int removeEchoServer() {
	// Удаление службы

	SC_HANDLE serviceManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if(!serviceManager) {
		log("It's impossible to open service control manager.");
		return SERVICE_ERROR;
	}

	SC_HANDLE service = OpenService(serviceManager, SERVICE_NAME, SERVICE_STOP | DELETE);
	if(!serviceManager) {
		log("It's impossible to remove service.");
		CloseServiceHandle(serviceManager);
		return SERVICE_ERROR;
	}

	DeleteService(service);
	CloseServiceHandle(service);
	CloseServiceHandle(serviceManager);

	log("Service has been successfully removed.");
	return NULL;
}

int startEchoServer() {
	// Запуск службы

	SC_HANDLE serviceManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
	if(!serviceManager) {
		log("It's impossible to open service control manager.");
		return SERVICE_ERROR;
	}

	SC_HANDLE service = OpenService(serviceManager, SERVICE_NAME, SERVICE_START);
	if(!serviceManager) {
		log("It's impossible to start service.");
		CloseServiceHandle(serviceManager);
		return SERVICE_ERROR;
	}

	CloseServiceHandle(serviceManager);
	CloseServiceHandle(serviceManager);

	log("Service has been successfully started.");
	return NULL;
}

int serviceMain(int argc, char** argv) {
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

