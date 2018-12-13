#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEFAULT_FILENAME "readfile.txt"
#define BUFFER_SIZE 100

int main(int argc, char** argv) {
	char* filename = DEFAULT_FILENAME;

	if(argc < 2)
		printf("Using default filename: %s.\n", filename);
	else
		strcpy(filename, argv[1]);

	// Создаем первый канал
	int result = mknod("firstChannel", S_IFIFO | 0666, 0);
	if(result) {
		perror("It's impossible to create first channel.\n");
		return 0x1;
	}

	// Создаем второй канал
	result = mknod("secondChannel", S_IFIFO | 0666, 0);
	if(result) {
		perror("It's impossible to create second channel.\n");
		return 0x2;
	}

	// Открываем первый канал на запись
	int firstChannel = open("firstChannel", O_WRONLY);
	if(firstChannel == -1) {
		perror("It's impossible to open first channel for writing.\n");	
		return 0x3;
	}

	// Открываем первый канал на чтение
	int secondChannel = open("secondChannel", O_RDONLY);
	if(secondChannel == -1) {
		perror("It's impossible to open second channel for reading.\n");	
		return 0x4;
	}

	// Записываем в первый канал имя файла
	write(firstChannel, filename, strlen(filename));

	char buffer[BUFFER_SIZE];
	while(1) {
		bzero(buffer, BUFFER_SIZE);
		// Считываем из второго канала и выводим содержимое
		result = read(secondChannel, buffer, BUFFER_SIZE);
		if(result <= 0)
			break;
		printf("File's part: %s\n", buffer);	
	}

	// Закрываем каналы
	close(firstChannel);
	close(secondChannel);

	// Разрываем связь и удаляем файлы
	unlink("firstChannel");
	unlink("secondChannel");

	return 0x0;
}


