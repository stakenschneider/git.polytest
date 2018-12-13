#include <windows.h>
#include <iostream>

// Размер буфера разделяемой памяти
static const int BUFFER_SIZE = 256;
// Количество сообщений
static const int ITERATIONS_COUNT = 15;

static const char* MUTEX_NAME = "SYNC_MUTEX";
static const char* MAPPING_NAME = "FILE_MAPPING_OBJECT";

int main() {
	// Открываем мьютекс
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT(MUTEX_NAME));
	if(mutex == nullptr) {
		std::cerr << "It's impossible to open mutex." << std::endl;
		return 0x1;
	}

	HANDLE mapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT(MAPPING_NAME));
	if(mapFile == nullptr) {
		std::cerr << "It's impossible to open mapping file." << std::endl;
		return 0x2;
	}

	// Получаем представление проецироанной области памяти
	LPCTSTR viewFile = LPTSTR(MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, NULL, NULL, BUFFER_SIZE));
	if(viewFile == nullptr) {
		std::cerr << "It's impossible to view map file." << std::endl;
		return 0x3;
	}

	for(int index = 0; index < ITERATIONS_COUNT; ++index) {
		// Ожидаем освобождения мьютекса
		WaitForSingleObject(mutex, INFINITE);
		// Считываем разделяемую память
		std::cout << "Read message: " << (char *) viewFile << std::endl;
		ReleaseMutex(mutex);
	}

	UnmapViewOfFile(viewFile);
	CloseHandle(mapFile);
	return 0x0;
}
