#include <windows.h>
#include <stdio.h>
#include <tchar.h>	
#include <iostream>

// Имя именованного канала
static const char* PIPE_NAME = "\\\\.\\pipe\\$$MyPipe$$";
// Размер буфера
static const int BUFFER_SIZE = 1024;
// Сообщение для выхода
static const char* EXIT_MESSAGE = "@EXIT@";

int main() {
	std::cout << "Server started." << std::endl;

	// Создаем именованный канал
	HANDLE namedPipe = CreateNamedPipe(_tcsdup(TEXT(PIPE_NAME)), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUFFER_SIZE, BUFFER_SIZE, NULL, nullptr);
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
	
	char buffer[BUFFER_SIZE];
	
	while(true) {
		ZeroMemory(buffer, sizeof(buffer));
		
		// Получаем сообщение от клиента 
		DWORD countOfBytesRead;
		if(!ReadFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			return 0x3;
		}
		
		if(!strcmp(buffer, EXIT_MESSAGE))
			break;
				
		std::cout << "Message from client: " << buffer << std::endl;

		// Отправляем сообщение обратно клиенту
		DWORD countOfBytesWrite;
		if(!WriteFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			return 0x4;
		}
	}

	CloseHandle(namedPipe);
	return 0x0;
}
