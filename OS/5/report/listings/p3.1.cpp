#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>

// Аргумент для функции обработчика потока
struct Argument {
	int number;
	bool* interrupt;
};

static const int COUNT_OF_PRIORITIES = 7;
// Количество создаваемых потоков по умолчанию
static const int DEFAULT_COUNT_OF_THREADS = COUNT_OF_PRIORITIES;
// Время до завершения потока (в секундах)
static const int DEFAULT_TIME_TO_STOP = 5;
// Константа для таймера
static const __int64 TIMER_CONSTANT = -1 * 10000000;

// Счетчики увеличиваемые каждым потоком
static __int64 counter[COUNT_OF_PRIORITIES] = { 0, 0, 0, 0, 0, 0, 0 };
// Значения приоритетов для каждого из потоков
static const int priority[COUNT_OF_PRIORITIES] = { THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL };
// Текстовые строки приоритетов для вывода
static const char* string[COUNT_OF_PRIORITIES] = { "THREAD_PRIORITY_IDLE", "THREAD_PRIORITY_LOWEST", "THREAD_PRIORITY_BELOW_NORMAL", "THREAD_PRIORITY_NORMAL",
"THREAD_PRIORITY_ABOVE_NORMAL", "THREAD_PRIORITY_HIGHEST", "THREAD_PRIORITY_TIME_CRITICAL" };

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID);

int main() {
	std::cout << "Count of threads - " << DEFAULT_COUNT_OF_THREADS << ", time to stop - " << DEFAULT_TIME_TO_STOP << "." << std::endl;

	// Создаем таймер
	HANDLE timer = CreateWaitableTimer(nullptr, false, nullptr);

	// Устанавливаем временные характеристики таймера
	__int64 endTimeValue = TIMER_CONSTANT * DEFAULT_TIME_TO_STOP;
	LARGE_INTEGER endTimeStruct;
	endTimeStruct.LowPart = DWORD(endTimeValue & 0xFFFFFFFF);
	endTimeStruct.HighPart = LONG(endTimeValue >> 0x20);
	SetWaitableTimer(timer, &endTimeStruct, NULL, nullptr, nullptr, false);

	bool interrupt = false;
	for(int index = 0; index < DEFAULT_COUNT_OF_THREADS; ++index) {
		// Создаем указатель на структуру аргумента
		Argument* argument = new Argument;
		argument->number = index;
		argument->interrupt = &interrupt;

		// Создаем поток
		HANDLE thread = CreateThread(nullptr, NULL, threadExecutor, LPVOID(argument), NULL, nullptr);
		// Задаем приоритет для каждого потока
		SetThreadPriority(thread, priority[index]);
		SetThreadPriorityBoost(thread, true);
		CloseHandle(thread);
	}

	// Ожидаем таймер
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	// Посылаем прерывание всем потокам
	interrupt = true;

	for(int index = 0; index < countOfThreads; ++index)
		std::cout << std::left << std::setw(30) << std::setfill(' ') << string[index] << " - " <<
			         std::right << std::setw(30) << std::setfill(' ') << counter[index] << std::endl;

	printf("Press \"Enter\" to exit.\n");
	std::getchar();
	
	return 0x0;
}

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID ptr) {
	// Указатель на идентификатор потока был передан как параметр функции
	const Argument argument = *static_cast<Argument*>(ptr);

	// Увеличиваем счетчик пока не пришло прерывание
	while(!(*argument.interrupt))
		++counter[argument.number];

	return 0x0;
}
