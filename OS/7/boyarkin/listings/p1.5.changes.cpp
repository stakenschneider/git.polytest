...

// Глобальный дескриптор
HANDLE event;

...

int main(int argc, char* argv[]) {
	...
	
	// Создаем событие
	event = CreateEvent(nullptr, false, true, nullptr);
	if(!event) {
		std::cerr << "It's impossible to create event." << std::endl;
		return 0x7;
	}
	
	...
	
	// Закрываем дескриптор события
	CloseHandle(event);
	
	...
}

...

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(event, INFINITE);
		
		if(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull) {
			...
		}
		
		// Освобождение объекта синхронизации
		SetEvent(event);
		
		Sleep(globalConfig.readersDelay);
	}
	
	...
}

...

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	...
	
	while(!interrupt) {
		// Захват объекта синхронизации
		WaitForSingleObject(event, INFINITE);

		if(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull) {
			...
		}

		// Освобождение объекта синхронизации
		SetEvent(event);
		
		Sleep(globalConfig.writersDelay);
	}

	...
}