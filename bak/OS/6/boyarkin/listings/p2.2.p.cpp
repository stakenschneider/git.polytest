#include <windows.h>
#include <stdio.h>
#include <tchar.h>  
#include <iostream>

// Имя именованного канала
static const char* PIPE_NAME = "\\\\.\\pipe\\$$MyPipe$$";
// Размер буфера
static const int BUFFER_SIZE = 1024;
// Максимальное количество клиентов
static const int CLIENT_COUNT = 10;
// Сообщение для выхода
static const char* EXIT_MESSAGE = "@EXIT@";

static bool interrupt = false;

DWORD WINAPI threadExecutor(LPVOID param);

int main() {
	std::cout << "Server started." << std::endl;

	while(!interrupt) {
		// Создаем именованный канал
		HANDLE namedPipe = CreateNamedPipe(_tcsdup(TEXT(PIPE_NAME)), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, CLIENT_COUNT, BUFFER_SIZE, BUFFER_SIZE, 5000, nullptr);
		if(namedPipe == INVALID_HANDLE_VALUE) {
			std::cerr << "It's impossible to create pipe." << std::endl;
			return 0x1;
		}

		// Подключаемся к именованному каналу
		if(!ConnectNamedPipe(namedPipe, nullptr)) {
			std::cerr << "It's impossible to connect to pipe." << std::endl;
			return 0x2;
		}

		std::cout << "Pipe has been successfully created." << std::endl;

		if(!CreateThread(nullptr, NULL, threadExecutor, LPVOID(namedPipe), NULL, nullptr)) {
			std::cerr << "It's impossible to create thread." << std::endl;
			return 0x3;
		}
	}

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}

DWORD WINAPI threadExecutor(LPVOID param) {
	HANDLE namedPipe = HANDLE(param);

	char buffer[BUFFER_SIZE];
	while(!interrupt) {
		ZeroMemory(buffer, sizeof(buffer));

		// Получаем сообщение от клиента 
		DWORD countOfBytesRead;
		if(!ReadFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			exit(0x3);
		}

		std::cout << "Message from client: " << buffer << std::endl;

		// Отправляем сообщение обратно клиенту
		DWORD countOfBytesWrite;
		if(!WriteFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			exit(0x4);
		}

		// Завершаем работу приложения
		if(!strcmp(buffer, EXIT_MESSAGE))
			interrupt = true;
	}

	CloseHandle(namedPipe);
	ExitThread(NULL);
}
