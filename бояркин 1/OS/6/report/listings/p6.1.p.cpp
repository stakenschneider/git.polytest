#include <windows.h>
#include <iostream>

// Название почтового слота
static const char* MAILSLOT_NAME = "\\\\.\\mailslot\\MyMailSlot";
// Размер буфера
static const int BUFFER_SIZE = 1024;

int main() {
	// Создаем почтовый слот
	HANDLE mailslot = CreateMailslot(MAILSLOT_NAME, BUFFER_SIZE, MAILSLOT_WAIT_FOREVER, nullptr);
	if(mailslot == INVALID_HANDLE_VALUE) {
		std::cerr << "It's impossible to create mailslot." << std::endl;
		return 0x1;
	}

	std::cout << "Mailslot successfully created." << std::endl;

	char buffer[BUFFER_SIZE];
	while(true) {
		ZeroMemory(buffer, BUFFER_SIZE);

		// Считываем клиентское сообщение
		DWORD countOfBytesRead;
		if(!ReadFile(mailslot, buffer, BUFFER_SIZE, &countOfBytesRead, nullptr)) {
			std::cerr << "It's impossible to read file." << std::endl;
			return 0x2;
		}

		std::cout << "Client message: " << buffer << std::cout;
	}

	CloseHandle(mailslot);
	return 0x0;
}
