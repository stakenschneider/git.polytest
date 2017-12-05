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
// Если был буст
static int boost[COUNT_OF_PRIORITIES] = { 0, 0, 0, 0, 0, 0, 0 };
// Показывает, изменился ли приоритет в процессе
static bool change[COUNT_OF_PRIORITIES] = { false, false, false, false, false, false, false };
// Значения приоритетов для каждого из потоков
static const int priority[COUNT_OF_PRIORITIES] = { THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL };
// Текстовые строки приоритетов для вывода
static const char* string[COUNT_OF_PRIORITIES] = { "THREAD_PRIORITY_IDLE", "THREAD_PRIORITY_LOWEST", "THREAD_PRIORITY_BELOW_NORMAL", "THREAD_PRIORITY_NORMAL",
"THREAD_PRIORITY_ABOVE_NORMAL", "THREAD_PRIORITY_HIGHEST", "THREAD_PRIORITY_TIME_CRITICAL" };

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID);

int main(int argc, char** argv) {
	int countOfThreads = DEFAULT_COUNT_OF_THREADS;
	int timeToStop = DEFAULT_TIME_TO_STOP;

	// Получение аргументов командной строки
	if(argc > 2) {
		try {
			countOfThreads = std::stoi(argv[1]);
			timeToStop = std::stoi(argv[2]);
			if(countOfThreads > DEFAULT_COUNT_OF_THREADS)
				return 0x1;
		}
		catch(const std::invalid_argument& exception) {
			std::cerr << exception.what() << std::endl;
			return 0x2;
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

	// Устанавливаем приоритет реального времени
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	bool interrupt = false;
	for(int index = 0; index < countOfThreads; ++index) {
		// Создаем указатель на структуру аргумента
		Argument* argument = new Argument;
		argument->number = index;
		argument->interrupt = &interrupt;

		// Создаем поток
		HANDLE thread = CreateThread(nullptr, NULL, threadExecutor, LPVOID(argument), NULL, nullptr);
		// Задаем приоритет для каждого потока
		SetThreadPriority(thread, priority[index]);
		SetThreadPriorityBoost(thread, true);
		GetThreadPriorityBoost(thread, &boost[index]);
		CloseHandle(thread);
	}

	// Ожидаем таймер
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	// Посылаем прерывание всем потокам
	interrupt = true;

	// Выводим в виде таблицы
	std::cout << std::left << std::setw(32) << std::setfill(' ') << "Priority" <<
				 std::left << std::setw(7) << std::setfill(' ') << "Boost" <<
				 std::left << std::setw(7) << std::setfill(' ') << "Changed" <<
				 std::right << std::setw(12) << std::setfill(' ') << "Counter" <<
				 std::endl;
	for(int index = 0; index < countOfThreads; ++index)
		std::cout << std::left << std::setw(32) << std::setfill(' ') << string[index] <<
					 std::left << std::setw(7) << std::setfill(' ') << ((boost[index] == 0) ? "No" : "Yes") <<
					 std::left << std::setw(7) << std::setfill(' ') << ((!change[index]) ? "No" : "Yes") <<
					 std::right << std::setw(12) << std::setfill(' ') << counter[index] << 
					 std::endl;

	printf("Press \"Enter\" to exit.\n");
	std::getchar();
	
	return 0x0;
}

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID ptr) {
	// Указатель на идентификатор потока был передан как параметр функции
	const Argument argument = *static_cast<Argument*>(ptr);

	// Увеличиваем счетчик пока не пришло прерывание
	while(!(*argument.interrupt)) {
		// Получаем текущий приоритет
		int currentPriority = GetThreadPriority(GetCurrentThread());
		// Если приоритет поменялся
		if(currentPriority != priority[argument.number])
			change[argument.number] = true;

		++counter[argument.number];
	}

	return 0x0;
}
