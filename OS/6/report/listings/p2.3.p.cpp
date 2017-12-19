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
		// Инициализация дескриптора значениями по умолчанию
		SECURITY_DESCRIPTOR securityDescriptor;
		if(!InitializeSecurityDescriptor(&securityDescriptor, SECURITY_DESCRIPTOR_REVISION)) {
			std::cerr << "It's impossible to initialize security descriptor." << std::endl;
			return 0x1;
		}
		
		// Обнуление поля DACL
		if(!SetSecurityDescriptorDacl(&securityDescriptor, TRUE, nullptr, FALSE)) {
			std::cerr << "It's impossible to set security descriptor." << std::endl;
			return 0x2;
		}

		// Установка атрибутов для именованного канала
		SECURITY_ATTRIBUTES securityAttributes;
		ZeroMemory(&securityAttributes, sizeof(securityAttributes));
		securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.lpSecurityDescriptor = &securityDescriptor;
		securityAttributes.bInheritHandle = FALSE;
		
		// Создаем именованный канал
		HANDLE namedPipe = CreateNamedPipe(_tcsdup(TEXT(PIPE_NAME)), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, CLIENT_COUNT, BUFFER_SIZE, BUFFER_SIZE, 5000, &securityAttributes);
		if(namedPipe == INVALID_HANDLE_VALUE) {
			std::cerr << "It's impossible to create pipe." << std::endl;
			return 0x3;
		}

		// Подключаемся к именованному каналу
		if(!ConnectNamedPipe(namedPipe, nullptr)) {
			std::cerr << "It's impossible to connect to pipe." << std::endl;
			return 0x4;
		}

		std::cout << "Pipe has been successfully created." << std::endl;

		if(!CreateThread(nullptr, NULL, threadExecutor, LPVOID(namedPipe), NULL, nullptr)) {
			std::cerr << "It's impossible to create thread." << std::endl;
			return 0x5;
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
			exit(0x6);
		}

		std::cout << "Message from client: " << buffer << std::endl;

		// Отправляем сообщение обратно клиенту
		DWORD countOfBytesWrite;
		if(!WriteFile(namedPipe, buffer, BUFFER_SIZE, &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			exit(0x7);
		}

		// Завершаем работу приложения
		if(!strcmp(buffer, EXIT_MESSAGE))
			interrupt = true;
	}

	CloseHandle(namedPipe);
	ExitThread(NULL);
}
