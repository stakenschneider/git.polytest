...

static const int TIMEOUT = 5000;

CRITICAL_SECTION criticalSection;
CONDITION_VARIABLE readConditionVariable;
CONDITION_VARIABLE writeConditionVariable;

...

int main(int argc, char* argv[]) {
	...
	
	// Инициализируем критическую секцию и условные переменные
	InitializeCriticalSection(&criticalSection);
	InitializeConditionVariable(&readConditionVariable);
	InitializeConditionVariable(&writeConditionVariable);
	
	...
	
	// Ожидание завершения всех потоков
	WaitForMultipleObjects(handlers.size(), &(handlers[0]), TRUE, TIMEOUT);
	
	...
	
	// Удаляем критическую секцию
	DeleteCriticalSection(&criticalSection);
	
	...
}

...

DWORD WINAPI threadReaderExecutor(LPVOID argument) {
	int threadId = int(argument);
	while(!interrupt) {
		EnterCriticalSection(&criticalSection);

		// Ожидаем пока в очереди не освободится место
		while(!(globalQueue.readIndex != globalQueue.writeIndex || globalQueue.isFull))
			SleepConditionVariableCS(&readConditionVariable, &criticalSection, INFINITE);

		// Если в очереди есть данные
		printf("Reader #%d. Get data \"%s\" from position %d.\n", threadId, globalQueue.messages[globalQueue.readIndex], globalQueue.readIndex);

		// Берем данные из очереди
		delete[] globalQueue.messages[globalQueue.readIndex];
		globalQueue.messages[globalQueue.readIndex] = nullptr;
		globalQueue.isFull = false;

		// Вычисляем индекс читателя
		globalQueue.readIndex = (globalQueue.readIndex + 1) % globalQueue.queueSize;

		// Посылаем сигнал потокам-писателям
		WakeConditionVariable(&writeConditionVariable);
		// Освобождение объекта синхронизации
		LeaveCriticalSection(&criticalSection);

		Sleep(globalConfig.readersDelay);
	}

	printf("Reader finished.\n");
	return NULL;
}

...

DWORD WINAPI threadWriterExecutor(LPVOID argument) {
	int threadId = int(argument);

	int messageIndex = 0;
	std::string message;
	while(!interrupt) {
		// Захват объекта синхронизации
		EnterCriticalSection(&criticalSection);

		// Ожидаем пока в очереди не освободится место
		while(!(globalQueue.readIndex != globalQueue.writeIndex || !globalQueue.isFull))
			SleepConditionVariableCS(&writeConditionVariable, &criticalSection, INFINITE);

		// Записываем данные в очередь
		message = "Writer #" + std::to_string(threadId) + ". Message number #" + std::to_string(messageIndex++) + ".";
		globalQueue.messages[globalQueue.writeIndex] = _strdup(message.data());

		printf("Writer #%d. Put data \"%s\" in position %d.\n", threadId, globalQueue.messages[globalQueue.writeIndex], globalQueue.writeIndex);

		// Вычисляем индекс писателя
		globalQueue.writeIndex = (globalQueue.writeIndex + 1) % globalQueue.queueSize;
		
		if(globalQueue.isFull)
			printf("Writer #%d. Queue is full.", threadId);

		// Посылаем сигнал потокам-читателям
		WakeConditionVariable(&readConditionVariable);
		// Освобождение объекта синхронизации
		LeaveCriticalSection(&criticalSection);

		Sleep(globalConfig.writersDelay);
	}

	printf("Writer finished.\n");
	return NULL;
}