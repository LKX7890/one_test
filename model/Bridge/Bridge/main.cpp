
#include "softwares.hpp"
#include "nokia.hpp"
#include "game.hpp"
#include "xiaomi.hpp"
#include "tianmao.hpp"

enum SoftWares_NAME
{
	SoftWares_NAME_GAME,
	SoftWares_NAME_TIANMAO,
};

int main()
{
	Game *game = new Game;
	Nokia nokia;
	nokia.InstallSoftWares(game, SoftWares_NAME_GAME);
	nokia.StartSoftWares(SoftWares_NAME_GAME);

	TianMao *tianmao = new TianMao;
	XiaoMi xiaomi;
	xiaomi.InstallSoftWares(tianmao, SoftWares_NAME_TIANMAO);
	xiaomi.StartSoftWares(SoftWares_NAME_TIANMAO);

	system("pause");
	return 0;
}