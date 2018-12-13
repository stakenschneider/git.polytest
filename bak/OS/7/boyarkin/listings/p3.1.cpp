#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

static const int PHILOSOFERS_COUNT = 5;

struct Fork {
	std::mutex mutex;
};

void* threadEatExecutor(Fork* leftChopstick, Fork* rightChopstick, const int philosopherNumber, const int leftChopstickNumber, const int rightChopstickNumber);

int main() {
	// Массив с вилками для еды
	std::vector<std::unique_ptr<Fork>> chopsticks(PHILOSOFERS_COUNT);

	// Создание всех вилок
	for(int chopstickIndex = 0; chopstickIndex < PHILOSOFERS_COUNT; ++chopstickIndex) {
		auto chopstick = std::make_unique<Fork>();
		chopsticks[chopstickIndex] = std::move(chopstick);
	}

	// Создаем потоки инструментами стандартной библиотеки std::thread, потому что она позволяет передавать в функцию множество аргументов

	// Создание массива потоков
	std::vector<std::shared_ptr<std::thread>> threads(PHILOSOFERS_COUNT);

	// Биндим функцию (задаем количество аргументов)
	auto bindThread = std::bind(&threadEatExecutor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	// Запускаем поток с определенными аргументами
	auto sharedThread = std::make_shared<std::thread>(bindThread, chopsticks.front().get(), chopsticks.back().get(), 1, 1, PHILOSOFERS_COUNT);
	threads[0] = std::move(sharedThread);

	for(int threadIndex = 1; threadIndex < PHILOSOFERS_COUNT; ++threadIndex) {
		// Биндим функцию (задаем количество аргументов)
		bindThread = std::bind(&threadEatExecutor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		// Запускаем поток с определенными аргументами
		sharedThread = std::make_shared<std::thread>(bindThread, chopsticks[threadIndex - 1].get(), chopsticks[threadIndex].get(), threadIndex + 1, threadIndex, threadIndex + 1);
		threads[threadIndex] = std::move(sharedThread);
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	std::cout << std::endl << "Press \"Enter\" to exit." << std::endl;
	std::getchar();

	return 0x0;
}

void* threadEatExecutor(Fork* leftChopstick, Fork* rightChopstick, const int philosopherNumber, const int leftChopstickNumber, const int rightChopstickNumber) {
	if(leftChopstick == rightChopstick) {
		std::cerr << "Chopsticks are same." << std::endl;
		exit(0x1);
	}

	// Для того чтобы не было дедлока
	std::lock(leftChopstick->mutex, rightChopstick->mutex);

	// Блокируем первую вилку
	std::lock_guard<std::mutex>(leftChopstick->mutex, std::adopt_lock);

	printf("   Philosopher #%d. Picked %d chopstick.\n", philosopherNumber, leftChopstickNumber);

	// Блокируем вторую вилку
	std::lock_guard<std::mutex>(rightChopstick->mutex, std::adopt_lock);

	printf("   Philosopher #%d. Picked %d chopstick.\n", philosopherNumber, rightChopstickNumber);

	printf("Philosopher #%d eats.\n", philosopherNumber);
	return nullptr;
}