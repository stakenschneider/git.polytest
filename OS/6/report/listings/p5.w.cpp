#include <windows.h>
#include <iostream>
#include <ctime>
#include <string>

// Размер буфера разделяемой памяти
static const int BUFFER_SIZE = 256;
// Количество сообщений
static const int ITERATIONS_COUNT = 15;

static const int DELAY = 1000;

static const char* MESSAGE = "WRITER_MESSAGE";

static const char* MUTEX_NAME = "SYNC_MUTEX";
static const char* MAPPING_NAME = "FILE_MAPPING_OBJECT";

int main() {
	// Создаем мьютекс
	HANDLE mutex = CreateMutex(nullptr, FALSE, TEXT(MUTEX_NAME));
	if(mutex == nullptr) {
		std::cerr << "It's impossible to create mutex." << std::endl;
		return 0x1;
	}

	HANDLE mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, NULL, BUFFER_SIZE, TEXT(MAPPING_NAME));
	if(mapFile == nullptr || mapFile == INVALID_HANDLE_VALUE) {
		std::cerr << "It's impossible to create mapping file." << std::endl;
		return 0x2;
	}

	// Получаем представление проецироанной области памяти
	LPCTSTR viewFile = LPTSTR(MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, NULL, NULL, BUFFER_SIZE));
	if(viewFile == nullptr) {
		std::cerr << "It's impossible to view map file." << std::endl;
		return 0x3;
	}

	srand(time(nullptr));

	std::string message;

	int number;
	while(true) {
		// Пишем в разделяемую памят случайные цифры
		number = rand();
		message = std::to_string(number);
		CopyMemory(PVOID(viewFile), message.data(), message.size());
		std::cout << "Write message: " << (char *) viewFile << std::endl;

		ReleaseMutex(mutex);
	}

	UnmapViewOfFile(viewFile);
	CloseHandle(mapFile);
	CloseHandle(mutex);
	return 0x0;
}
