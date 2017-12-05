#include <stdio.h>
#include <windows.h>
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

static const char* CONFIG_PATH = "config.ini";
static const __int64 TIMER_CONSTANT = -1 * 10000000;
static const char* EXIT_MESSAGE = "q";
static const char* SHARE_MEMORY_NAME = "$$SharedMemory$$";
static const int BUFFER_SIZE = 1024;
static const int OFFSET = sizeof(int) * 2;
static constexpr char* READER_PROCESS_PATH = "p2.2.r.exe";

static const char* EVENT_CAN_READ_NAME = "$$EventCanRead$$";
static const char* EVENT_CAN_WRITE_NAME = "$$EventCanWrite$$";
static const char* EVENT_CHANGE_COUNT_NAME = "$$EventChangeCount$$";
static const char* EVENT_EXIT_NAME = "$$EventExit$$";
static const char* EVENT_ALL_READ_NAME = "$$EventAllRead$$";

// Глобальные структуры конфигурации и очереди
struct Configuration globalConfig;

// Массив, содержащий дескрипторы всех потоков
std::vector<HANDLE> handlers;
// Переменная, которая завершает все потоки
bool interrupt = false;

// События для синхронизации
HANDLE eventCanRead, eventCanWrite, eventChangeCount, eventExit, eventAllRead;
// Отображение файла
HANDLE fileMapping;
// Указатели на отображаемую память
LPVOID writersMap;

// Функция для заполнения структуры конфигурации
void setConfig(const std::string filename, Configuration* config);
// Получение параметра структуры конфигурации
void getConfigParam(int* parametr, std::istream& stream);
// Функция, конфигурирующая и запускающая таймер
HANDLE createAndStartWaitableTimer(const unsigned int secondsCount);

// Функция, создающая все потоки
void createAllThreads(Configuration* config);
// Обработчики потоков
DWORD WINAPI threadWriterExecutor(LPVOID argument);
DWORD WINAPI threadTimeManagerExecutor(LPVOID argument);

int main(int argc, char* argv[]) {
	// Получение названия файла конфигурации
	std::string configFilename;
	configFilename = (argc < 2) ? CONFIG_PATH : argv[1];
	setConfig(configFilename, &globalConfig);

	// Создание всех потоков
	createAllThreads(&globalConfig);

	// Создание отображаемого файла
	fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, NULL, BUFFER_SIZE, SHARE_MEMORY_NAME);
	if(!fileMapping) {
		std::cerr << "It's impossible to create shared file." << std::endl;
		return 0x7;
	}

	// Отображение файла на адресное пространство процесса
	writersMap = MapViewOfFile(fileMapping, FILE_MAP_WRITE, NULL, NULL, NULL);
	if(!writersMap) {
		std::cerr << "It's impossible to view map." << std::endl;
		return 0x8;
	}

	*((int*) writersMap) = 0;
	*(((int*) writersMap) + 1) = globalConfig.readersCount;

	// Инициализация событий для синхронизации
	eventCanRead = CreateEvent(nullptr, true, false, EVENT_CAN_READ_NAME);
	eventCanWrite = CreateEvent(nullptr, false, false, EVENT_CAN_WRITE_NAME);
	eventAllRead = CreateEvent(nullptr, true, true, EVENT_ALL_READ_NAME);
	eventChangeCount = CreateEvent(nullptr, false, true, EVENT_CHANGE_COUNT_NAME);
	eventExit = CreateEvent(nullptr, true, false, EVENT_EXIT_NAME);
	if(!eventCanRead || !eventCanWrite || !eventAllRead || !eventChangeCount || !eventExit) {
		std::cerr << "It's impossible to create event." << std::endl;
		return 0x9;
	}

	// Старт всех потоков
	for(auto& current : handlers)
		ResumeThread(current);

	// Ожидание завершения всех потоков
	WaitForMultipleObjects(handlers.size(), &(handlers[0]), TRUE, INFINITE);

	// Закрываем дескрипторы потоков
	for(auto& current : handlers)
		CloseHandle(current);

	// Закрываем описатели объектов синхронизации
	CloseHandle(eventCanRead);
	CloseHandle(eventCanWrite);
	CloseHandle(eventAllRead);
	CloseHandle(eventChangeCount);
	CloseHandle(eventExit);

	// Удаляем разделяемую память
	UnmapViewOfFile(writersMap);

	CloseHandle(fileMapping);

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
	STARTUPINFO startupInformation;
	ZeroMemory(&startupInformation, sizeof(startupInformation));
	startupInformation.cb = sizeof(startupInformation);

	PROCESS_INFORMATION processInformation;

	std::string readerPath;
	// Создание всех процессов читателей
	for(int readerIndex = 0; readerIndex < config->readersCount; ++readerIndex) {
		readerPath = std::string(READER_PROCESS_PATH) + " " + std::to_string(readerIndex);

		BOOL reader = CreateProcess(nullptr, _strdup(readerPath.data()), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInformation, &processInformation);
		if(!reader) {
			std::cerr << "It's impossible to create reader." << std::endl;
			exit(0x5);
		}
	}

	std::cout << "Readers have been successfully created." << std::endl;


	// Создание всех потоков писателей
	for(int writerIndex = 0; writerIndex < config->writersCount; ++writerIndex) {
		HANDLE writer = CreateThread(nullptr, NULL, threadWriterExecutor, LPVOID(writerIndex), CREATE_SUSPENDED, nullptr);
		if(!writer) {
			std::cerr << "It's impossible to create writer." << std::endl;
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
		SetEvent(eventExit);
		interrupt = true;
		CloseHandle(timer);
	}

	printf("Time manager finished.\n");
	return NULL;
}

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	int threadId = int(argument);

	int messageIndex = 0;

	// Задаем массив событий для функции ожидания
	HANDLE writerHandlers[2];
	writerHandlers[0] = eventExit;
	writerHandlers[1] = eventCanWrite;

	while(!interrupt) {
		DWORD analyzeEvent = WaitForMultipleObjects(2, writerHandlers, false, INFINITE);
		switch(analyzeEvent) {
		case WAIT_OBJECT_0:
			// Событие завершения потока
			printf("Writer #%d finished.\n", threadId);
			return NULL;

		case (WAIT_OBJECT_0 + 1):
			// Событие записи
			++messageIndex;

			// Запись в разделяемую память
			sprintf_s(((char *) writersMap) + OFFSET, BUFFER_SIZE, "Writer #%d. Message number #%d.", threadId, messageIndex);

			printf("Writer #%d. Put data \"%s\".\n", threadId, ((char *) writersMap) + OFFSET);

			WaitForSingleObject(eventChangeCount, INFINITE);

			*((int*) writersMap) += globalConfig.readersCount;
			*(((int*) writersMap) + 1) += globalConfig.readersCount;

			SetEvent(eventChangeCount);

			// Разрешаем читателям прочитать сообщение и ставим событие в занятое
			SetEvent(eventCanRead);
			break;

		default:
			std::cerr << "Error in function WaitForMultipleObjects in writer executor." << std::endl;
			exit(0xA);
		}
	}

	printf("Writer #%d finished.\n", threadId);
	return NULL;
}