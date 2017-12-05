#include <windows.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <tchar.h>

// Путь к процессу потомку
static const char* CHILD_NAME = "p1.1.c.exe";

// Особые коды нажатых клавиш
static const short CARRIAGE_CODE = 0xD;
static const short EXIT_CODE = 0x1B;

int main() {
	// Установка атрибутов для пайпа
	SECURITY_ATTRIBUTES securityAttributes;
	securityAttributes.nLength = sizeof(securityAttributes);
	securityAttributes.lpSecurityDescriptor = nullptr;
	securityAttributes.bInheritHandle = true;

	HANDLE readPipe, writePipe;

	// Создание анонимного канала
	if(!CreatePipe(&readPipe, &writePipe, &securityAttributes, NULL)) {
		std::cerr << "It's impossible to create pipe." << std::endl;
		return 0x1;
	}

	std::cout << "Pipe has been created." << std::endl 
		      << "Handle of pipe " << readPipe << std::endl;

	// Подменяем стандартный дескриптор ввода дескриптором ввода канала
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_NORMAL;
	startupInfo.hStdInput = readPipe;
	startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	startupInfo.hStdError = startupInfo.hStdOutput;

	PROCESS_INFORMATION processInformation;

	// Создадим новый процесс
	if(!CreateProcess(nullptr, _tcsdup(TEXT(CHILD_NAME)), nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInformation)) {
		std::cerr << "It's impossible to create process." << std::endl;
		return 0x2;
	}

	std::cout << "Process has been created." << std::endl;

	std::cout << "STD INPUT HANDLE " << GetStdHandle(STD_INPUT_HANDLE) << std::endl
	          << "STD OUTPUT HANDLE " << GetStdHandle(STD_OUTPUT_HANDLE) << std::endl;

	char buffer;
	while(true) {
		buffer = _getch();

		std::cout.put(buffer);

		if(buffer == CARRIAGE_CODE) {
			buffer = '\n';
			std::cout.put(buffer);
		}

		DWORD countOfWrittenBytes;

		// Записываем в канал один байт
		WriteFile(writePipe, &buffer, 1, &countOfWrittenBytes, nullptr);

		// При нажатии на ESC завершаем цикл
		if(buffer == EXIT_CODE)
			break;
	}

	// Завершаем процесс потомок
	TerminateProcess(processInformation.hProcess, NULL);

	CloseHandle(processInformation.hThread);
	CloseHandle(processInformation.hProcess);

	CloseHandle(readPipe);
	CloseHandle(writePipe);

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}
