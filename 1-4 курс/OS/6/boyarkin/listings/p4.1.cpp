#include <windows.h>
#include <iostream>

// Обработчик сигналов прерывания
BOOL ctrlHandler(DWORD signalType);

int main() {
	if(SetConsoleCtrlHandler(PHANDLER_ROUTINE(ctrlHandler), TRUE)) {
		std::cout << "Control handler is installed. Waiting interrupt signals." << std::endl;
		while(true);
	}
	
	std::cerr << "It's impossible to set control handler." << std::endl;
	return 0x1;
}

BOOL ctrlHandler(DWORD signalType) {
	// Выводим сообщение и звук определенной частоты, в зависимости от прерывания
	switch(signalType) {
	case CTRL_C_EVENT:
		std::cout << "Ctrl-C event." << std::endl;
		Beep(750, 300);
		return TRUE;

	case CTRL_CLOSE_EVENT:
		std::cout << "Ctrl-Close event." << std::endl;
		Beep(600, 200);
		return TRUE;

	case CTRL_BREAK_EVENT:
		std::cout << "Ctrl-Break event." << std::endl;
		Beep(900, 200);
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		std::cout << "Ctrl-Logoff event." << std::endl;
		Beep(1000, 200);
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		std::cout << "Ctrl-Shutdown event." << std::endl;
		Beep(750, 500);
		return FALSE;

	default:
		return FALSE;
	}
}
