#include <stdio.h>
#include <windows.h>
#include <ostream>
#include <iostream>
#include <string>

// Размер буфера
static const int BUFFER_SIZE = 1024;
// Количество отсылаемых сообщений
static const int MESSAGES_COUNT = 5;
// Задержка перед завершением
static const int TERMINATE_DELAY = 5000;

int main() {
	// Пишем в поток ошибок, так как ввод и вывод занят
	std::cerr << "Client started." << std::endl;
	
	std::string message;
	char buffer[BUFFER_SIZE];

	for(int index = 0; index < MESSAGES_COUNT; ++index) {
		message = "Message #";
		message += std::to_string(index);

		// Отправляем сообщение на сервер
		DWORD countOfBytesWrite;
		if(!WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), message.data(), DWORD(message.size()), &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			return 0x1;
		}

		ZeroMemory(buffer, sizeof(buffer));
		
		// Принимаем ответ с сервера
		DWORD countOfBytesRead;
		if(!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			return 0x2;
		}

		// Пишем в поток ошибок, так как ввод и вывод занят
		std::cerr << "Message from server: " << buffer << std::endl;
	}

	// Пишем в поток ошибок, так как ввод и вывод занят
	std::cerr << "Client terminate across few seconds." << std::endl;
	Sleep(TERMINATE_DELAY);

	return 0x0;
}
