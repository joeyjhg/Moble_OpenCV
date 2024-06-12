#include "Include.h"
#include "Process.h"
#include "MainUI.h"


#define __TEST__

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetConsoleOutputCP(CP_UTF8);

	MainUI* gui= new MainUI();
	gui->Mainmenu();
	
	Safe_Delete(gui);
	return 0;
}