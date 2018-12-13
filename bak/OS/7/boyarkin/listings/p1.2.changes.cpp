...

// Глобальный дескриптор
HANDLE mutex;

...

int main(int argc, char* argv[]) {
	...
	
	// Создание мьютекса для синхронизации
	mutex = CreateMutex(nullptr, FALSE, nullptr);
	if(!mutex) {
		std::cerr << "It's impossible to create mutex." << std::endl;
		return 0x7;
	}
	
	...
	
	// Закрываем дескриптор мьютекса
	CloseHandle(mutex);
	
	...
}

...

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(mutex, INFINITE);
		
		if(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull) {
			...
		}
		
		// Освобождение объекта синхронизации
		ReleaseMutex(mutex);
		
		Sleep(globalConfig.readersDelay);
	}
	
	...
}

...

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(mutex, INFINITE);

		if(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull) {
			...
		}

		// Освобождение объекта синхронизации
		ReleaseMutex(mutex);
		
		Sleep(globalConfig.writersDelay);
	}

	...
}