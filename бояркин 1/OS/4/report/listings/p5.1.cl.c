#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

int main() {
	// Каналы уже созданы сервером, открываем каналы
	int firstChannel = open("firstChannel", O_RDONLY);
	if(firstChannel == -1) {
		perror("It's impossible to open first channel for reading.\n");		
		return 0x1;
	}	
	
	int secondChannel = open("secondChannel", O_WRONLY);
	if(secondChannel == -1) {
		perror("It's impossible to open second channel for writing.\n");		
		return 0x2;
	}

	char filename[BUFFER_SIZE];
	bzero(filename, BUFFER_SIZE);

	// Считываем из первого канала имя файла
	int result = read(firstChannel, filename, BUFFER_SIZE);
	if(result <= 0) {
		perror("It's impossible to read filename from first channel.\n");	
		return 0x3;	
	}

	printf("Received filename: %s.\n", filename);
	
	// Открываем файл на чтение
	FILE* file = fopen(filename, "r");
	if(!file) {
		perror("It's impossible open file to read.\n");	
		return 0x4;	
	}

	// Отправляем во второй канал содержимое файла
	char* buffer[BUFFER_SIZE];
	while(!feof(file)) {
		result = fread(buffer, sizeof(char), BUFFER_SIZE, file);	
		write(secondChannel, buffer, result);
	}

	// Закрываем файл
	fclose(file);

	// Закрываем каналы
	close(firstChannel);
	close(secondChannel);
	return 0x0;
}
