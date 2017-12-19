#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string>

// Название почтового слота
static const char* MAILSLOT_NAME = "\\\\.\\mailslot\\MyMailSlot";
// Размер буфера
static const int BUFFER_SIZE = 1024;

int main() {
	// Создаем почтовый слот
	HANDLE mailslot = CreateFile(MAILSLOT_NAME, GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(mailslot == INVALID_HANDLE_VALUE) {
		std::cerr << "It's impossible to create file." << std::endl;
		return 0x1;
	}

	std::cout << "Ready to send messages." << std::endl;

	std::string message;
	while(true) {
		message.clear();
		std::getline(std::cin, message);

		// Отправляем сообщение на сервер
		DWORD countOfBytesWrite;     
		if(!WriteFile(mailslot, message.data(), int(message.size()), &countOfBytesWrite, nullptr)) {
			std::cerr << "It's impossible to write file." << std::endl;
			return 0x2;
		}
	}

	CloseHandle(mailslot);
	return 0x0;
}
