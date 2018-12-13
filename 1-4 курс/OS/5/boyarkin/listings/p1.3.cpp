#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <tchar.h>

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cerr << "Wrong count of arguments." << std::endl;
		return 0x1;
	}

	// Открываем конфигурационный файл на чтение
	std::ifstream stream(argv[1]);
	
	// Если не удалось открыть файл
	if(!stream.is_open()) {
		std::cerr << "It's impossible to open configuration file." << std::endl;
		return 0x2;
	}

	std::string line;
	do {
		line.clear();

		// Считываем из файла построчно
		std::getline(stream, line);
		if(line.empty()) {
			std::cerr << "Skip empty line." << std::endl;
			continue;
		}
			
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