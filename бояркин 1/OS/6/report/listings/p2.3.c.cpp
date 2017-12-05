#include <windows.h>
#include <stdio.h>
#include <tchar.h>  
#include <iostream>
#include <string>

// Имя именованного канала
static const char* PIPE_NAME = "\\\\.\\pipe\\$$MyPipe$$";
// Размер буфера
static const int BUFFER_SIZE = 1024;
// Сообщение для выхода
static const char* EXIT_MESSAGE = "@EXIT@";

int main() {
	std::cout << "Client started." << std::endl;

	// Ожидаем пока канал освободится
	if(!WaitNamedPipe(_tcsdup(TEXT(PIPE_NAME)), NMPWAIT_WAIT_FOREVER)) {
		std::cerr << "It's impossible to wait pipe." << std::endl;
		return 0x1;
	}

	HANDLE namedPipe = CreateFile(_tcsdup(TEXT(PIPE_NAME)), GENERIC_READ | GENERIC_WRITE, NULL, nullptr, OPEN_EXISTING, NULL, nullptr);
	if(namedPipe == INVALID_HANDLE_VALUE) {
		std::cerr << "It's impossible to create file." << std::endl;
		return 0x2;
	}

	std::cout << "Connection established." << std::endl;

	std::string message;
	char buffer[BUFFER_SIZE];

	while(true) {
		std::getline(std::cin, message);

		// Отправляем сообщение на сервер
		DWORD countOfBytesWrite;
		if(!WriteFile(namedPipe, message.data(), DWORD(message.size()), &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			return 0x3;
		}

		// Завершаем работу приложения
		if(message == EXIT_MESSAGE)
			break;

		ZeroMemory(buffer, sizeof(buffer));

		// Получаем сообщение обратно от сервера 
		DWORD countOfBytesRead;
		if(!ReadFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			return 0x4;
		}

		std::cout << "Message from server: " << buffer << std::endl;
	}

	CloseHandle(namedPipe);

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}
