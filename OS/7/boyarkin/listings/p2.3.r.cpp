#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>

static const int OFFSET = sizeof(int) * 2;
static const int READ_DELAY = 200;

static const char* SHARE_MEMORY_NAME = "$$SharedMemory$$";
// Названия событий
static const char* EVENT_CAN_READ_NAME = "$$EventCanRead$$";
static const char* EVENT_CAN_WRITE_NAME = "$$EventCanWrite$$";
static const char* EVENT_CHANGE_COUNT_NAME = "$$EventChangeCount$$";
static const char* EVENT_COUNT_NAME = "$$EventCount$$";
static const char* EVENT_EXIT_NAME = "$$EventExit$$";
static const char* EVENT_ALL_READ_NAME = "$$EventAllRead$$";

// События для синхронизации
HANDLE eventCanRead, eventCanWrite, eventChangeCount, eventExit, eventAllRead, eventCount;

int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cerr << "Wrong count of arguments." << std::endl;
		return 0x1;
	}

	// Получаем идентификатор из аргумента командной строки
	int readerId = std::stoi(argv[1]);

	std::cout << "Reader #" << readerId << " started." << std::endl;

	CRITICAL_SECTION criticalSection;
	InitializeCriticalSection(&criticalSection);

	// Инициализация событий для синхронизации
	eventCanRead = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_CAN_READ_NAME);
	eventCanWrite = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_CAN_WRITE_NAME);
	eventAllRead = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_ALL_READ_NAME);
	eventChangeCount = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_CHANGE_COUNT_NAME);
	eventExit = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_EXIT_NAME);
	eventCount = OpenEvent(EVENT_ALL_ACCESS, false, EVENT_COUNT_NAME);
	if(!eventCanRead || !eventCanWrite || !eventAllRead || !eventChangeCount || !eventExit || !eventCount) {
		std::cerr << "It's impossible to open event." << std::endl;
		return 0x2;
	}

	// Открытие отображаемого файла
	HANDLE fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, SHARE_MEMORY_NAME);
	if(!fileMapping) {
		std::cerr << "It's impossible to open shared file." << std::endl;
		return 0x3;
	}

	LPVOID readersMap = MapViewOfFile(fileMapping, FILE_MAP_READ, NULL, NULL, NULL);
	if(!readersMap) {
		std::cerr << "It's impossible to view map." << std::endl;
		return 0x4;
	}

	// Задаем массив событий для функции ожидания
	HANDLE readerHandlers[2];
	readerHandlers[0] = eventExit;
	readerHandlers[1] = eventCanRead;

	while(true) {
		EnterCriticalSection(&criticalSection);
		// Ожидаем, пока все прочитают
		WaitForSingleObject(eventAllRead, INFINITE);
		// Узнаем сколько потоков читателей прошло данную границу
		WaitForSingleObject(eventChangeCount, INFINITE);

		SetEvent(eventCount);

		DWORD analyzeEvent = WaitForMultipleObjects(2, readerHandlers, false, INFINITE);
		switch(analyzeEvent) {
		case WAIT_OBJECT_0:
			// Событие завершения потока
			printf("Reader #%d finished.\n", readerId);
			goto exit;

		case (WAIT_OBJECT_0 + 1):
			// Событие чтения
			printf("Reader #%d. Get data \"%s\".\n", readerId, ((char *) readersMap) + OFFSET);

			// Ожидание уменьшение счетчика количества читателей
			WaitForSingleObject(eventChangeCount, INFINITE);
			SetEvent(eventCount);
			break;

		default:
			std::cerr << "Error in function WaitForMultipleObjects in reader executor." << std::endl;
			exit(0x5);
		}

		LeaveCriticalSection(&criticalSection);
		Sleep(READ_DELAY);
	}

exit:
	DeleteCriticalSection(&criticalSection);

	// Закрываем описатели объектов синхронизации
	CloseHandle(eventCanRead);
	CloseHandle(eventCanWrite);
	CloseHandle(eventAllRead);
	CloseHandle(eventChangeCount);
	CloseHandle(eventExit);
	CloseHandle(eventCount);

	// Удаляем разделяемую память
	UnmapViewOfFile(readersMap);

	CloseHandle(fileMapping);

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}