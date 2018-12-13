...
case CTRL_C_EVENT:
		std::cout << "Ctrl-C event." << std::endl;
		Beep(750, 300);
		
		std::cout << "Are you sure you want to close application?" << std::endl
		          << "Press Ctrl+C to close it." << std::endl;
		
		SetConsoleCtrlHandler(PHANDLER_ROUTINE(ctrlHandler), TRUE);
		return TRUE;		
...