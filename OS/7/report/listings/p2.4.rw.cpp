#include <stdio.h>
#include <windows.h>
#include <iostream>

static const int WRITERS_COUNT = 4;
static const int READERS_COUNT = 3;

// Структура для синхронизации данных
struct DataSync {
	int writerId;
	bool canWrite;
};

CRITICAL_SECTION criticalSection;
struct DataSync dataSync;

// Обработчики потоков
DWORD WINAPI threadWriterExecutor(LPVOID argument);
DWORD WINAPI threadReaderExecutor(LPVOID argument);

int main() {
	// Инициализация структуры синхронизации данных
	dataSync.writerId = 0;
	dataSync.canWrite = true;

	// Инициализация критической секции
	InitializeCriticalSection(&criticalSection);

	// Создание потоков-писателей
	for(int writerIndex = 0; writerIndex < WRITERS_COUNT; ++writerIndex) {
		HANDLE writer = CreateThread(nullptr, NULL, threadWriterExecutor, LPVOID(writerIndex), CREATE_SUSPENDED, nullptr);
		if(!writer) {
			std::cerr << "It's impossible to create writer." << std::endl;
			return 0x1;
		}
		
		// Запуск потока
		ResumeThread(writer);
		CloseHandle(writer);
	}

	// Создание потоков-читателей
	for(int readerIndex = 0; readerIndex < READERS_COUNT; ++readerIndex) {
		HANDLE reader = CreateThread(nullptr, NULL, threadReaderExecutor, LPVOID(readerIndex), CREATE_SUSPENDED, nullptr);
		if(!reader) {
			std::cerr << "It's impossible to create reader." << std::endl;
			return 0x2;
		}

		// Запуск потока
		ResumeThread(reader);
		CloseHandle(reader);
	}

	// Удаление критической секции
	DeleteCriticalSection(&criticalSection);

	std::cout << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	int readerId = int(argument);

	while(true) {
		EnterCriticalSection(&criticalSection);

		if(!dataSync.canWrite) {
			printf("Reader #%d. Get data from writer #%d.\n", readerId, dataSync.writerId);

			// Разрешение дальнейшей записи после считывания
			dataSync.canWrite = true;
		}

		LeaveCriticalSection(&criticalSection);
	}
}

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	int writerId = int(argument);

	while(true) {
		EnterCriticalSection(&criticalSection);

		if(dataSync.canWrite) {
			// Запись данных
			dataSync.writerId = writerId;
			dataSync.canWrite = false;

			printf("Writer #%d. Put data.\n", writerId);
		}

		LeaveCriticalSection(&criticalSection);
	}
}
