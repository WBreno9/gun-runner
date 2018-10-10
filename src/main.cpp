#include <inc.h>
#include <game.h>

int main(int argc, char** argv)
{
	Game game;
	game.createWindow(800, 600);
	game.start();

	return 0;
}
