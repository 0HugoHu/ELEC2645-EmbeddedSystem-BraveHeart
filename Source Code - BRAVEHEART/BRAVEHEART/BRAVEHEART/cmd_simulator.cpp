
#include "cmd_simulator.h"

cmd::cmd() {
	system("mode con:cols=168 lines=48");
	// auto-check
	for (auto j = 0; j < HEIGHT; j++)
		for (auto i = 0; i < WIDTH * 2; i++)
			if (i % 2 == 0) std::cout << "*";
			else std::cout << " ";
	for (auto j = 0; j < HEIGHT; j++)
		for (auto i = 0; i < WIDTH * 2; i++)
			std::cout << " ";
}

void cmd::setPixel(int x, int y) {
	COORD coord;
	coord.X = 2 * x - 2;
	coord.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "*";
}