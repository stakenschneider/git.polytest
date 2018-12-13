#include <windows.h>
#include <stdio.h>
#define SIZE_OF_BUF 1000

DWORD WINAPI threadHandler(LPVOID param){
    HANDLE hNamedPipe = (HANDLE)param;
    DWORD readbytes, writebytes; //число байт прочитанных и переданных
    char buf[SIZE_OF_BUF]; // Буфер для передачи данных через канал
    while (1)
    {
        // Получаем очередную команду через канал Pipe
        if (ReadFile(hNamedPipe, buf, SIZE_OF_BUF, &readbytes, NULL))
        {
            // Посылаем эту команду обратно клиентскому
            // приложению
            if (!WriteFile(hNamedPipe, buf, strlen(buf) + 1, &writebytes, NULL))
                break;
            // Выводим принятую команду на консоль
            printf("Get client msg: %s\n", buf);
            // Если пришла команда "exit",
            // завершаем работу приложения
            if (!strncmp(buf, "exit", 4))
                break;
        }
        else
        {
            fprintf(stdout, "ReadFile: Error %ld\n", GetLastError());
            getchar();
            break;
        }
    }
    CloseHandle(hNamedPipe);
    ExitThread(0);
}



int main()
{
    HANDLE hNamedPipe; // Идентификатор канала Pipe
    HANDLE t; //Для потока
    LPCWSTR lpszPipeName = L"\\\\.\\pipe\\$$MyPipe$$"; // Имя создаваемого канала Pipe
    printf("Server is started. Try to create named pipe\n");
    // Создаем канал Pipe, имеющий имя lpszPipeName
    while (1){
        hNamedPipe = CreateNamedPipe(
            lpszPipeName, //имя канала
            PIPE_ACCESS_DUPLEX, //доступ и на чтение и на запись
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, //передача сообщений (как чтение,
            //так и запись)
            5, //максимальное число экземпляров каналов равно 5 (число клиентов)
            SIZE_OF_BUF, SIZE_OF_BUF, 5000, NULL); //размеры выходного и входного буферов канала, 5
        //секунд - длительность для функции WaitNamedPipe
        if (hNamedPipe == INVALID_HANDLE_VALUE) // Если возникла ошибка, выводим ее код и завершаем
            //работу приложения
        {
            fprintf(stdout, "CreateNamedPipe: Error %ld\n", GetLastError());
            getchar();
            return 1000;
        }
        printf("Named pipe created successfully\n");
        // Выводим сообщение о начале процесса создания канала
        printf("waiting for connect\n");
        // Ожидаем соединения со стороны клиента

        if (!ConnectNamedPipe(hNamedPipe, NULL))
        {
            // При возникновении ошибки выводим ее код
            printf("error with function ConnectNamedPipe\n");
            getchar();
            CloseHandle(hNamedPipe);
            return 1001;
        }
        // Выводим сообщение об успешном создании канала
        fprintf(stdout, "Client connected\n");
        t = CreateThread(NULL, 0, threadHandler, (LPVOID)hNamedPipe, 0, NULL);
        CloseHandle(t);
    }
    printf("server is ending\n press any key\n");
    getchar();
    return 0;
}
