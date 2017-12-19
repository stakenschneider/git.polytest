#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>

// Имя файла
static const char* FILENAME = "result.out";
// Строка для записи
static const char* PARENT_BUFFER = "Parent message\n";

static const char* COMMAND_LINE = "p4.3.c.exe %d";

static const int COMMAND_BUFFER_SIZE = 100;

int main() {
	// Задаем аттрибуты для файла
	SECURITY_ATTRIBUTES securityAttributes;
	ZeroMemory(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.lpSecurityDescriptor = nullptr;
	securityAttributes.bInheritHandle = TRUE;

	// Создаем новый файл
	HANDLE createFile = CreateFile(TEXT(FILENAME), GENERIC_WRITE, NULL, &securityAttributes, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);	
	if(createFile == INVALID_HANDLE_VALUE) {
		std::cerr << "It's impossible to create file." << std::endl;
		return 0x1;
	}

	// Записываем строку в файл
	DWORD countOfBytesWritten;
	BOOL writeFile = WriteFile(createFile, PARENT_BUFFER, DWORD(strlen(PARENT_BUFFER)), &countOfBytesWritten, nullptr);
	if(writeFile == FALSE) {
		std::cerr << "It's impossible to write file." << std::endl;
		return 0x2;
	}

	// Запись команды для создания процесса
	TCHAR cmdLine[COMMAND_BUFFER_SIZE];
	wsprintf(cmdLine, TEXT(COMMAND_LINE), createFile);
	
	// Задаем информацию для создания процесса
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION processInformation;

	// Создаем новый процесс
	if(!CreateProcess(nullptr, cmdLine, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInformation)) {
		std::cerr << "It's impossible to create process." << std::endl;
		return 0x3;
	}

	CloseHandle(processInformation.hThread);
	CloseHandle(processInformation.hProcess);

	printf("Press \"Enter\" to exit.\n");
	std::getchar();

	CloseHandle(createFile);
	return 0x0;
}
