#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <tchar.h>

// Путь к конфигурационному файлу
const char* PATH_TO_FILE = "D:\\afiles\\student\\temp\\OS\\5\\p1.2.config";

int main() {
	// Открываем конфигурационный файл на чтение
	std::ifstream stream(PATH_TO_FILE);
	
	// Если не удалось открыть файл
	if(!stream.is_open()) {
		std::cerr << "It's impossible to open configuration file." << std::endl;
		return 0x1;
	}

	std::string line;
	do {
		line.clear();

		// Считываем из файла построчно
		std::getline(stream, line);
		if(line.empty())
			break;

		// Универсальная форма строки с командой
		LPTSTR commandLine = _tcsdup(TEXT(line.data()));

		STARTUPINFO startupInfo;
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		// Информация о процессе (будет получена после создания процесса)
		PROCESS_INFORMATION processInforation;
		// Пробуем создать процесс
		if (!CreateProcess(nullptr, commandLine, nullptr, nullptr, false, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInforation)) {
			std::cerr << "It's impossible to create process." << std::endl;
			continue;
		}

		std::cout << "New process was created, process id " << processInforation.dwProcessId << ", thread id " << processInforation.dwThreadId << "." << std::endl;

		CloseHandle(processInforation.hThread);
		CloseHandle(processInforation.hProcess);
	} while(!stream.eof());
	
	// Очищаем считывающий поток
	stream.close();

	std::getchar();
	return 0x0;
}