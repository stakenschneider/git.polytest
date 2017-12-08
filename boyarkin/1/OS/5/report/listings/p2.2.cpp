#include <iostream>
#include <windows.h>
#include <string>

// Аргумент для функции обработчика потока
struct Argument {
	int number;
	bool* interrupt;
};

// Количество создаваемых потоков по умолчанию
static const int DEFAULT_COUNT_OF_THREADS = 3;
// Время до завершения потока (в секундах)
static const int DEFAULT_TIME_TO_STOP = 5;
// Константа для таймера
static const __int64 TIMER_CONSTANT = -1 * 10000000;
// Задержка перед каждым выводом потока (в миллисекундах)
static const int DELAY = 1000;

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID);
// Вывод текущего времени
void printCurrentTime();

int main(int argc, char** argv) {
	int countOfThreads = DEFAULT_COUNT_OF_THREADS;
	int timeToStop = DEFAULT_TIME_TO_STOP;

	// Получение аргументов командной строки
	if(argc > 2) {
		try {
			countOfThreads = std::stoi(argv[1]);
			timeToStop = std::stoi(argv[2]);
		} catch(const std::invalid_argument& exception) {
			std::cerr << exception.what() << std::endl;
			return 0x1;
		}
	}

	std::cout << "Count of threads - " << countOfThreads << ", time to stop - " << timeToStop << "." << std::endl;

	// Создаем таймер
	HANDLE timer = CreateWaitableTimer(nullptr, false, nullptr);

	// Устанавливаем временные характеристики таймера
	__int64 endTimeValue = TIMER_CONSTANT * timeToStop;
	LARGE_INTEGER endTimeStruct;
	endTimeStruct.LowPart = DWORD(endTimeValue & 0xFFFFFFFF);
	endTimeStruct.HighPart = LONG(endTimeValue >> 0x20);
	SetWaitableTimer(timer, &endTimeStruct, NULL, nullptr, nullptr, false);

	printCurrentTime();

	bool interrupt = false;
	for(int index = 0; index < countOfThreads; ++index) {
		// Создаем указатель на структуру аргумента
		Argument* argument = new Argument;
		argument->number = index;
		argument->interrupt = &interrupt;

		// Создаем поток
		HANDLE thread = CreateThread(nullptr, NULL, threadExecutor, LPVOID(argument), NULL, nullptr);
		CloseHandle(thread);
	}

	// Ожидаем таймер
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	// Посылаем прерывание всем потокам
	interrupt = true;

	printCurrentTime();

	printf("Press \"Enter\" to exit.\n");
	std::getchar();
	
	return 0x0;
}

void printCurrentTime() {
	// Выводим текущее время
	SYSTEMTIME now;
	GetLocalTime(&now);
	printf("Current local time %.2d:%.2d:%.2d\n", now.wHour, now.wMinute, now.wSecond);
}

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID ptr) {
	// Указатель на идентификатор потока был передан как параметр функции
	const Argument argument = *static_cast<Argument*>(ptr);

	printf("Thread %d started.\n", argument.number);

	while(!(*argument.interrupt)) {
		// Вывод организован функцией printf, так как потоки в C++ вызывают проблемы с синхронизацией из-за endl
		printf("%d\n", argument.number);
		// Задержка перед следующим выводом
		Sleep(DELAY);
	}

	return 0x0;
}
