#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>

// Количество возможных сигналов
#define COUNT_OF_SIGNALS 8
// Количество посылаемых случайных сиглаов
#define COUNT_OF_RANDOM_SIGNALS 20
// Ожидание приема сигналов
#define DELAY 1

// Задаем явно, т.к. стандартный SIGRTMIN не константа
#undef SIGRTMIN
#define SIGRTMIN 34

// Набор доступных сигналов для отправки
static int intSignal[COUNT_OF_SIGNALS] = {SIGUSR1, SIGUSR2, SIGCHLD, SIGRTMIN, SIGRTMIN + 1, SIGRTMIN + 2, SIGRTMIN + 3, SIGRTMIN + 4};

// Сигналы в виде строки для отображения
static char* stringSignal[COUNT_OF_SIGNALS] = {"SIGUSR1", "SIGUSR2", "SIGCHLD", "SIGRTMIN", "SIGRTMIN+1", "SIGRTMIN+2", "SIGRTMIN+3", "SIGRTMIN+4"};

const char* signalToString(int sig);
void signalHandler(int sig);

int main() {
	sigset_t mask;
	struct sigaction action;

	// Инициализация структур
	memset(&action, 0, sizeof(action));
	sigemptyset(&mask);

	// Добавляем в маску все возможные сигналы
	for(int index = 0; index < COUNT_OF_SIGNALS; ++index)
		sigaddset(&mask, intSignal[index]);

	// Устанавливаем обработчик для сигнала
	action.sa_handler = signalHandler;
	action.sa_mask = mask;

	// Изменение действия при получении сигнала
	for(int index = 0; index < COUNT_OF_RANDOM_SIGNALS; ++index)
		sigaction(intSignal[index], &action, NULL);

	// Перед отправкой сигналов блокируем обработчик сигналов
	sigprocmask(SIG_BLOCK, &mask, NULL);

	// Для правильной генерации чисел
	srand(time(NULL));

	printf("Send signals: ");
	for(int index = 0; index < COUNT_OF_RANDOM_SIGNALS; ++index) {
		// Случайный индекс массива всех возможных сигналов
		int randomIndex = rand() % COUNT_OF_SIGNALS;
		int currentSignal = intSignal[randomIndex];
		// Отправка сигналов самому себе
		raise(currentSignal);
		printf("%s, ", signalToString(currentSignal));
	}
	printf("\nReceived signals: ");

	// После отправки сигналов разблокируем обработчик
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	// Ожидаем завершение обработки сигналов
	sleep(DELAY);

	printf("\n");
	return 0x0;
}

void signalHandler(int sig) {
	printf("%s, ", signalToString(sig));
}

const char* signalToString(int sig) {
	int contains = -1;
	for(int index = 0; index < COUNT_OF_SIGNALS; ++index)
		if(intSignal[index] == sig) {
			contains = index;
			break;
		}

	// Если сигнал не содержится в массиве доступных сигналов, NULL
	if(contains == -1)
		return NULL;

	// Если содержится, то возвращаем строку
	return stringSignal[contains];
}

