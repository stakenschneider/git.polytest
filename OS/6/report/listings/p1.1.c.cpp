#include <iostream>
#include <stdio.h>
#include <windows.h>

int main() {
	std::cout << "STD INPUT HANDLE = " << GetStdHandle(STD_INPUT_HANDLE) << std::endl
	          << "STD OUTPUT HANDLE = " << GetStdHandle(STD_OUTPUT_HANDLE) << std::endl;

	while(true) {
		DWORD totalBytesAvailable;

		// Получаем данные из канала
		PeekNamedPipe(GetStdHandle(STD_INPUT_HANDLE), nullptr, NULL, nullptr, &totalBytesAvailable, nullptr);

		if(totalBytesAvailable) {
			char buffer;
			DWORD countOfBytesRead;

			// Считываем один байт из канала
			ReadFile(GetStdHandle(STD_INPUT_HANDLE), &buffer, 1, &countOfBytesRead, nullptr);
			std::cout << buffer;
		}
	}

	return 0x0;
}
