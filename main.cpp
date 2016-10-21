#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include "Gameplay.h"

int main () {
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	cout << "Welcome to Video Poker!" << endl;
	cout << "You have started with $10" << endl;

	Gameplay play;
	while (play.menu ()) {}

	system ("pause");
	return 0;
}