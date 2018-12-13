#include <iostream>
#include <windows.h>

// Задержка перед каждым выводом потока (в миллисекундах)
const int DELAY = 500;

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID);

int main() {
	std::cout << "Press \"Enter\" to exit." << std::endl;

	// Число для передачи в поток (идентификатор первого потока)
	static const int firstNumber = 1;
	// Создаем первый поток
	HANDLE thread = CreateThread(nullptr, NULL, threadExecutor, LPVOID(&firstNumber), NULL, nullptr);
	// Закрываем дескриптор потока (не завершает поток)
	CloseHandle(thread);

	// Число для передачи в поток (идентификатор второго потока)
	static const int secondNumber = 2;
	// Создаем первый поток
	thread = CreateThread(nullptr, NULL, threadExecutor, LPVOID(&secondNumber), NULL, nullptr);
	// Закрываем дескриптор потока (не завершает поток)
	CloseHandle(thread);

	// Завершаем программу по нажатию
	std::getchar();
	return 0x0;
}

// Обработчик потока
DWORD WINAPI threadExecutor(LPVOID ptr) {
	// Указатель на идентификатор потока был передан как параметр функции
	const int threadId = *static_cast<int*>(ptr);

	while(true) {
		// Вывод организован функцией printf, так как потоки в C++ вызывают проблемы с синхронизацией из-за endl
		printf("%d\n", threadId);
		// Задержка перед следующим выводом
		Sleep(DELAY);
	}
}
