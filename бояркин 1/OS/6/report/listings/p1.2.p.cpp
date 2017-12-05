#include <windows.h>
#include <stdio.h>
#include <ostream>
#include <iostream>
#include <tchar.h>

// Путь к процессу потомку
static const char* CHILD_NAME = "p1.2.c.exe";
// Размер буфера
static const int BUFFER_SIZE = 1024;
// Количество отсылаемых сообщений
static const int MESSAGES_COUNT = 5;
// Задержка перед завершением
static const int TERMINATE_DELAY = 6000;

int main() {
	// Установка атрибутов для пайпов
	SECURITY_ATTRIBUTES securityAttributes;
	ZeroMemory(&securityAttributes, sizeof(securityAttributes));
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.lpSecurityDescriptor = nullptr;
	securityAttributes.bInheritHandle = TRUE;

	// Создаем канал для передачи от сервера клиенту
	HANDLE readPipeFromServerToClient, writePipeFromServerToClient;
	if(!CreatePipe(&readPipeFromServerToClient, &writePipeFromServerToClient, &securityAttributes, NULL)) {
		std::cerr << "It's impossible to create pipe from server to client." << std::endl;
		return 0x1;
	}

	// Создаем канал для передачи от клиента серверу
	HANDLE readPipeFromClientToServer, writePipeFromClientToServer;
	if(!CreatePipe(&readPipeFromClientToServer, &writePipeFromClientToServer, &securityAttributes, NULL)) {
		std::cerr << "It's impossible to create pipe from client to server." << std::endl;
		return 0x2;
	}

	std::cout << "Server started." << std::endl;

	// Подменяем стандартный дескриптор ввода и вывода
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	GetStartupInfo(&startupInfo);
	startupInfo.hStdInput = readPipeFromServerToClient;
	startupInfo.hStdOutput = writePipeFromClientToServer;
	startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	startupInfo.dwFlags = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processInformation;

	// Создадим новый процесс
	if(!CreateProcess(nullptr, _tcsdup(TEXT(CHILD_NAME)), nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInformation)) {
		std::cerr << "It's impossible to create process." << std::endl;
		return 0x3;
	}

	CloseHandle(readPipeFromServerToClient);
	CloseHandle(writePipeFromClientToServer);

	std::string message;
	char buffer[BUFFER_SIZE];

	for(int index = 0; index < MESSAGES_COUNT; ++index) {
		ZeroMemory(buffer, sizeof(buffer));
		message.clear();

		// Получаем сообщение от клиента
		DWORD countOfBytesRead;
		if(!ReadFile(readPipeFromClientToServer, buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			return 0x4;
		}

		message = buffer;

		std::cout << "Message from client: " << message.data() << std::endl;

		// Отправляем сообщение обратно клиенту
		DWORD countOfBytesWrite;
		if(!WriteFile(writePipeFromServerToClient, message.data(), DWORD(message.size()), &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			return 0x5;
		}
	}

	std::cout << "Client terminate across few seconds." << std::endl;
	Sleep(TERMINATE_DELAY);

	CloseHandle(readPipeFromClientToServer);
	CloseHandle(writePipeFromServerToClient);

	return 0x0;
}
