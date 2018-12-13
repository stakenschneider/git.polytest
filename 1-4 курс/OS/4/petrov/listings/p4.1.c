#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Название файла для чтения
#define READ_FILE "readfile.txt"
// Название файла для записи
#define WRITE_FILE "writefile.txt"
// Размер буфера для считывания и записи
#define BUFFER_SIZE 100

int main() {
	// Пара дескрипторов канала
	int pipeArray[2];

	// Пробуем создать неименованный канал
	if(pipe(pipeArray) < 0) {
		perror("It's impossible to create pipe.\n");	
		return 0x1;
	}	

	int result;

	if(!fork()) {
		// Закрываем канал на чтение
		close(pipeArray[0]);

		// Пробуем открыть файл на чтение
		FILE* file = fopen(READ_FILE, "r");	
		if(!file) {
			perror("Child couldn't open file to read.\n");	
			return 0x1;
		}

		char buffer[BUFFER_SIZE];
		while(!feof(file)) {
			// Считываем файл по размеру буффера
			result = fread(buffer, sizeof(char), BUFFER_SIZE, file);		
			// Записываем результат чтения в неименованный канал
			write(pipeArray[1], buffer, result);
		}

		// Закрываем файл
		fclose(file);
		// Закрываем канал на запись
		close(pipeArray[1]);
		return 0x0;
	} else {
		// Закрываем канал на запись
		close(pipeArray[1]);

		// Пробуем открыть файл на запись
		FILE* file = fopen(WRITE_FILE, "w");
		if(!file) {
			perror("Parent couldn't open file to write.\n");	
			return 0x2;
		}

		printf("Read from pipe:\n");

		char buffer[BUFFER_SIZE];
		while(1) {
			// Очищаем буфер
			bzero(buffer, BUFFER_SIZE);
			// Считываем данные из канала в буфер
			result = read(pipeArray[0], buffer, BUFFER_SIZE);		

			if(!result)
				break;

			printf("%s", buffer);
			// Записываем считанные данные в выходной файл
			fwrite(buffer, sizeof(char), result, file);
		}

		// Закрываем файл
		fclose(file);
		// Закрываем канал на чтение
		close(pipeArray[0]);
		return 0x0;
	}
}
