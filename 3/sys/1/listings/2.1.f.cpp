#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>

void activateFloatExceptions();

int main() {
	activateFloatExceptions();

	__try {
		// Инициируем переполнение переменной с плавающей точкой.
		pow(1e3, 1e3);
	} __except(GetExceptionCode() == EXCEPTION_FLT_OVERFLOW ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		std::cerr << "EXCEPTION_FLT_OVERFLOW: 0x" << std::hex << EXCEPTION_FLT_OVERFLOW << std::endl;
		return 0x1;
	}

	return 0x0;
}

void activateFloatExceptions() {
	// Включаем обработку исключений с плавающей точкой.
	auto control = _controlfp(NULL, NULL);
	control &= ~(EM_OVERFLOW | EM_UNDERFLOW | EM_INEXACT | EM_ZERODIVIDE | EM_DENORMAL);
	_controlfp(control, _MCW_EM);
}