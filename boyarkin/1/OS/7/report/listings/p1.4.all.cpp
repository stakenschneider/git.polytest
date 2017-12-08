#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Структура начальной конфигурации приложения
struct Configuration {
	// Количество потоков читателей
	int readersCount;
	// Количество потоков писателей
	int writersCount;
	// Размер очереди
	int queueSize;
	// Задержка читателей в миллисекундах
	int readersDelay;
	// Задержка писателей в миллисекундах
	int writersDelay;
	// Время жизни приложения
	int timeToLive;
};

// Очередь FIFO
struct Queue {
	// Массив сообщений
	char** messages;
	// Индекс записи
	int writeIndex;
	// Индекс чтения
	int readIndex;
	// Размер очереди
	int queueSize;
	// Флаг заполненности очереди
	bool isFull;
};

static const char* CONFIG_PATH = "config.ini";
static const __int64 TIMER_CONSTANT = -1 * 10000000;
static const char* EXIT_MESSAGE = "q";

// Глобальные структуры конфигурации и очереди
struct Configuration globalConfig;
struct Queue globalQueue;

// Массив, содержащий дескрипторы всех потоков
std::vector<HANDLE> handlers;
// Переменная, которая завершает все потоки
bool interrupt = false;

CRITICAL_SECTION criticalSection;

// Функция для заполнения структуры конфигурации
void setConfig(const std::string filename, Configuration* config);
// Получение параметра структуры конфигурации
void getConfigParam(int* parametr, std::istream& stream);
// Функция, конфигурирующая и запускающая таймер
HANDLE createAndStartWaitableTimer(const unsigned int secondsCount);

// Функция, создающая все потоки
void createAllThreads(Configuration* config);
// Обработчики потоков
DWORD WINAPI threadReaderExecutor(LPVOID argument);
DWORD WINAPI threadWriterExecutor(LPVOID argument);
DWORD WINAPI threadTimeManagerExecutor(LPVOID argument);

int main(int argc, char* argv[]) {
	// Получение названия файла конфигурации
	std::string configFilename;
	configFilename = (argc < 2) ? CONFIG_PATH : argv[1];
	setConfig(configFilename, &globalConfig);

	// Создание всех потоков
	createAllThreads(&globalConfig);

	// Заполнение структуры очереди
	globalQueue.isFull = false;
	globalQueue.readIndex = 0;
	globalQueue.writeIndex = 0;
	globalQueue.queueSize = globalConfig.queueSize;
	globalQueue.messages = new char*[globalConfig.queueSize];

	// Инициализируем критическую секцию
	InitializeCriticalSection(&criticalSection);

	// Старт всех потоков
	for(auto& current: handlers)
		ResumeThread(current);

	// Ожидание завершения всех потоков
	WaitForMultipleObjects(handlers.size(), &(handlers[0]), TRUE, INFINITE);

	// Закрываем дескрипторы потоков
	for(auto& current: handlers)
		CloseHandle(current);

	// Удаляем критическую секцию
	DeleteCriticalSection(&criticalSection);

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}

void getConfigParam(int* parametr, std::istream& stream) {
	// Получение параметра структуры конфигурации

	std::string line;

	if(stream.eof())
		throw std::exception();

	std::getline(stream, line);
	*parametr = std::stoi(line);
}

void setConfig(const std::string configFilename, Configuration* config) //функция установки конфигурации
{
	// Открываем конфигурационный файл
	auto stream = std::ifstream(configFilename.data());
	if(!stream.is_open()) {
		std::cerr << "It's impossible to open config file." << std::endl;
		exit(0x1);
	}

	// Заполняем структуру конфигурациии
	std::string line;
	try {
		getConfigParam(&(config->readersCount), stream);
		getConfigParam(&(config->readersDelay), stream);
		getConfigParam(&(config->writersCount), stream);
		getConfigParam(&(config->writersDelay), stream);
		getConfigParam(&(config->queueSize), stream);
		getConfigParam(&(config->timeToLive), stream);
	}
	catch(const std::exception& exception) {
		std::cerr << "It's impossible to parse config file." << std::endl;
		stream.close();
		exit(0x2);
	}

	// Проверка корректности полученных данных
	if(
		config->readersCount <= 0 || config->readersDelay <= 0 ||
		config->writersCount <= 0 || config->writersDelay <= 0 ||
		config->queueSize <= 0 || config->timeToLive == 0
		) {
		std::cerr << "Wrong config file values." << std::endl;
		stream.close();
		exit(0x3);
	}

	stream.close();

	std::cout << "----- Current config -----" << std::endl
		<< "Readers count: " << config->readersCount << std::endl
		<< "Readers delay: " << config->readersDelay << std::endl
		<< "Writers count: " << config->writersCount << std::endl
		<< "Writers delay: " << config->writersDelay << std::endl
		<< "Queue size: " << config->queueSize << std::endl
		<< "Time to live: " << config->timeToLive << std::endl << std::endl;
}

void createAllThreads(Configuration * config) {
	// Создание всех потоков читателей
	for(int readerIndex = 0; readerIndex < config->readersCount; ++readerIndex) {
		HANDLE reader = CreateThread(nullptr, NULL, threadReaderExecutor, LPVOID(readerIndex), CREATE_SUSPENDED, nullptr);
		if(!reader) {
			std::cerr << "It's impossible to create reader." << std::endl;
			exit(0x4);
		}

		handlers.push_back(reader);
	}

	std::cout << "Readers have been successfully created." << std::endl;


	// Создание всех потоков писателей
	for(int writerIndex = 0; writerIndex < config->readersCount; ++writerIndex) {
		HANDLE writer = CreateThread(nullptr, NULL, threadWriterExecutor, LPVOID(writerIndex), CREATE_SUSPENDED, nullptr);
		if(!writer) {
			std::cerr << "It's impossible to create reader." << std::endl;
			exit(0x5);
		}

		handlers.push_back(writer);
	}

	std::cout << "Writers have been successfully created." << std::endl;


	// Создание временного обработчика
	HANDLE timeManager = CreateThread(nullptr, NULL, threadTimeManagerExecutor, LPVOID(config->timeToLive), CREATE_SUSPENDED, nullptr);
	if(!timeManager) {
		std::cerr << "It's impossible to create time manager." << std::endl;
		exit(0x6);
	}

	handlers.push_back(timeManager);

	std::cout << "Time manager has been successfully created." << std::endl;
}

HANDLE createAndStartWaitableTimer(const unsigned int secondsCount) //создание, установка и запуск таймера
{
	// Вычисляем временные характеристики таймера
	__int64 endTimeValue = TIMER_CONSTANT * secondsCount;
	LARGE_INTEGER endTimeStruct;
	endTimeStruct.LowPart = DWORD(endTimeValue & 0xFFFFFFFF);
	endTimeStruct.HighPart = LONG(endTimeValue >> 0x20);

	// Создаем таймер
	HANDLE timer = CreateWaitableTimer(nullptr, false, nullptr);
	SetWaitableTimer(timer, &endTimeStruct, NULL, nullptr, nullptr, false);
	return timer;
}

DWORD WINAPI threadTimeManagerExecutor(LPVOID argument) {
	int timeToLive = int(argument);
	std::string message;

	// Если время жизни отрицательное, то выходим по введенной строке
	if(timeToLive < 0)
		while(true) {
			message.clear();
			std::getline(std::cin, message);

			if(message == EXIT_MESSAGE) {
				interrupt = true;
				break;
			}
		}
	// Если время жизни положительное, то выходим по таймеру
	else {
		HANDLE timer = createAndStartWaitableTimer(timeToLive);
		WaitForSingleObject(timer, INFINITE);
		interrupt = true;
		CloseHandle(timer);
	}

	printf("Time manager finished.\n");
	return NULL;
}

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	int threadId = int(argument);
	while(!interrupt) {
		EnterCriticalSection(&criticalSection);

		if(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull) {
			// Если в очереди есть данные
			printf("Reader #%d. Get data \"%s\" from position %d.\n", threadId, globalQueue.messages[globalQueue.readIndex], globalQueue.readIndex);

			// Берем данные из очереди
			delete[] globalQueue.messages[globalQueue.readIndex];
			globalQueue.messages[globalQueue.readIndex] = nullptr;
			globalQueue.isFull = false;

			//printf("Queue size: %d.\n", globalQueue.queueSize);
			//printf("Queue write index: %d.\n", globalQueue.writeIndex);

			// Вычисляем индекс читателя
			globalQueue.readIndex = (globalQueue.readIndex + 1) % globalQueue.queueSize;
		}

		LeaveCriticalSection(&criticalSection);
		Sleep(globalConfig.readersDelay);
	}

	printf("Reader finished.\n");
	return NULL;
}

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	int threadId = int(argument);

	int messageIndex = 0;
	std::string message;
	while(!interrupt) {
		EnterCriticalSection(&criticalSection);

		if(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull) {
			// Если в очереди нет данных

			// Записываем данные в очередь
			message = "Writer #" + std::to_string(threadId) + ". Message number #" + std::to_string(messageIndex++) + ".";
			globalQueue.messages[globalQueue.writeIndex] = _strdup(message.data());

			printf("Writer #%d. Put data \"%s\" in position %d.\n", threadId, globalQueue.messages[globalQueue.writeIndex], globalQueue.writeIndex);

			// Вычисляем индекс писателя
			globalQueue.writeIndex = (globalQueue.writeIndex + 1) % globalQueue.queueSize;
			// Получаем флаг заполненности очереди
			globalQueue.isFull = globalQueue.writeIndex == globalQueue.readIndex;
		}

		LeaveCriticalSection(&criticalSection);
		Sleep(globalConfig.writersDelay);
	}

	printf("Writer finished.\n");
	return NULL;
}