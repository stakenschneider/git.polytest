...

// Глобальный объект
CRITICAL_SECTION criticalSection;

...

int main(int argc, char* argv[]) {
	...
	
	// Инициализируем критическую секцию
	InitializeCriticalSection(&criticalSection);
	
	...
	
	// Удаляем критическую секцию
	DeleteCriticalSection(&criticalSection);
	
	...
}

...

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		EnterCriticalSection(&criticalSection);
		
		if(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull) {
			...
		}
		
		// Освобождение объекта синхронизации
		LeaveCriticalSection(&criticalSection);
		
		Sleep(globalConfig.readersDelay);
	}
	
	...
}

...

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		EnterCriticalSection(&criticalSection);

		if(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull) {
			...
		}

		// Освобождение объекта синхронизации
		LeaveCriticalSection(&criticalSection);
		
		Sleep(globalConfig.writersDelay);
	}

	...
}