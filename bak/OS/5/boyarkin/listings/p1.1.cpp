#include <iostream>
#include <windows.h>
#include <tchar.h>

// Задержка перед завершением процесса
const int DELAY = 5 * 1000;
// Путь к запускаемой программе 
const char* PATH_TO_CALC = "C:\\Windows\\System32\\calc.exe";

int main() {
	// Универсальная форма строки с командой
	LPTSTR commandLine = _tcsdup(TEXT(PATH_TO_CALC));
		
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	// Информация о процессе (будет получена после создания процесса)
	PROCESS_INFORMATION processInforation;
	// Пробуем создать процесс
	if(!CreateProcess(nullptr, commandLine, nullptr, nullptr, false, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInforation)) {
		std::cerr << "It's impossible to create process." << std::endl;
		return 0x1;
	}

	std::cout << "New process was created, process id " << processInforation.dwProcessId << ", thread id " << processInforation.dwThreadId << "." << std::endl;

	// Задержка перед завершением процесса
	Sleep(DELAY);

	CloseHandle(processInforation.hThread);
	CloseHandle(processInforation.hProcess);

	std::cout << "Program finished." << std::endl;

	std::getchar();
	return 0x0;
}