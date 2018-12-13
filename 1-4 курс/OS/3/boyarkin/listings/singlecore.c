#include <sched.h>
#include "singlecore.h"

/* 
	Функция, перераспределяющая ресурсы процессора на одно ядро
	Если не получилось, то возвращает 0
*/

int disableMultithreading(int pid) {
	// Структура, сохраняющая набор процессоров
	cpu_set_t cpuSet;

	// Обнуление структуры cpu_set_t
	CPU_ZERO(&cpuSet);
	// Добавляем в структуру новый процессор для последующего маскирования
	CPU_SET(0, &cpuSet);

	// Устанавливаем маску для процесса и выводим результат операции
	return sched_setaffinity(pid, sizeof(cpu_set_t), &cpuSet);
}

