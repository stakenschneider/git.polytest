#include <iostream>
#include <windows.h>

static const int SLEEP_DURATION = 1000;

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID);
// Вывод информации о динамическом изменении приоритетов
void printPriorityBoostInfo(HANDLE mainProcess, HANDLE mainThread, HANDLE sideThread);
// Расшифровка результата
const char* getStringByBool(BOOL value);

int main() {
	// Получение дескриптора текущего процесса
	HANDLE mainProcess = GetCurrentProcess();

	// Получение дескриптора текущей нити
	HANDLE mainThread = GetCurrentThread();

	// Создание второго потока
	HANDLE sideThread = CreateThread(nullptr, NULL, threadExecutor, nullptr, NULL, nullptr);

	// Вывод информации о динамическом изменении приоритетов
	std::cout<< "Default priorities" << std::endl;
	printPriorityBoostInfo(mainProcess, mainThread, sideThread);

	// Повышаем приоритет второго потока
	if(!SetThreadPriorityBoost(sideThread, true)) {
		std::cerr << "It's impossible to change thread priority." << std::endl;
		return 0x1;
	}

	// Вывод информации о динамическом изменении приоритетов
	std::cout << std::endl << "Side thread priority has been changed" << std::endl;
	printPriorityBoostInfo(mainProcess, mainThread, sideThread);
	
	// Повышаем приоритет процесса
	if(!SetProcessPriorityBoost(mainProcess, true)) {
		std::cout << "It's impossible to change process priority." << std::endl;
		return 0x2;
	}

	// Вывод информации о динамическом изменении приоритетов
	std::cout << std::endl << "Process priority has been changed" << std::endl;
	printPriorityBoostInfo(mainProcess, mainThread, sideThread);

	// Понижаем приоритет второго потока
	if(!SetThreadPriorityBoost(sideThread, false)) {
		std::cerr << "It's impossible to change thread priority." << std::endl;
		return 0x3;
	}

	// Вывод информации о динамическом изменении приоритетов
	std::cout << std::endl << "Side thread priority has been changed" << std::endl;
	printPriorityBoostInfo(mainProcess, mainThread, sideThread);
	
	std::cout << "Press \"Enter\" to exit.\n" << std::endl;
	std::getchar();

	return 0;
}

DWORD WINAPI threadExecutor(LPVOID) {
	while(true)
		Sleep(SLEEP_DURATION);
}

void printPriorityBoostInfo(HANDLE mainProcess, HANDLE mainThread, HANDLE sideThread) {
	BOOL result;

	GetProcessPriorityBoost(mainProcess, &result);
	std::cout << "Process dynamic: " << getStringByBool(result) << std::endl;

	GetThreadPriorityBoost(mainThread, &result);
	std::cout << "Main thread dynamic: " << getStringByBool(result) << std::endl;

	GetThreadPriorityBoost(sideThread, &result);
	std::cout << "Side thread dynamic: " << getStringByBool(result) << std::endl;
}

const char* getStringByBool(BOOL value) {
	return ((value == 0) ? "Dynamic enable": "Disabled");
}
