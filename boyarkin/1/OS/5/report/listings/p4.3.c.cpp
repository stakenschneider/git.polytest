#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>

// Строка для записи
static const char* CHILD_BUFFER = "Child message\n";

int main(int argc, char *argv[]) {
	// Получаем название файла для записи из командной строки
	HANDLE filename = HANDLE(std::stoi(argv[1]));

	DWORD countOfBytesWritten;

	// Записываем строку в файл
	BOOL writeFile = WriteFile(filename, CHILD_BUFFER, DWORD(strlen(CHILD_BUFFER)), &countOfBytesWritten, nullptr);
	if(writeFile == FALSE) {
		std::cerr << "It's impossible to write file." << std::endl;
		return 0x1;
	}

	CloseHandle(filename);
	return 0x0;
}
