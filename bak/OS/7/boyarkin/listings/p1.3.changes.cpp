...

// Глобальный дескриптор
HANDLE semaphore;

...

int main(int argc, char* argv[]) {
	...
	
	// Создание семафора для синхронизации
	semaphore = CreateSemaphore(nullptr, 1, 1, nullptr);
	if(!semaphore) {
		std::cerr << "It's impossible to create semaphore." << std::endl;
		return 0x7;
	}
	
	...
	
	// Закрываем дескриптор семафора
	CloseHandle(semaphore);
	
	...
}

...

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(semaphore, INFINITE);
		
		if(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull) {
			...
		}
		
		// Освобождение объекта синхронизации
		ReleaseSemaphore(semaphore, 1, nullptr);
		
		Sleep(globalConfig.readersDelay);
	}
	
	...
}

...

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(semaphore, INFINITE);

		if(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull) {
			...
		}

		// Освобождение объекта синхронизации
		ReleaseSemaphore(semaphore, 1, nullptr);
		
		Sleep(globalConfig.writersDelay);
	}

	...
}