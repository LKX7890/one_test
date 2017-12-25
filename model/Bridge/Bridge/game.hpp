#ifndef __GAME_HPP_
#define __GAME_HPP_

#include <iostream>
#include "softwares.hpp"

class Game : public SoftWares
{
public:
	Game() : SoftWares(){}
	~Game(){}

	virtual void OnStartSoftWares() { std::cout << "game start" << std::endl; this->OnStopSoftWares(); }
	void OnStopSoftWares(){ std::cout << "game over" << std::endl; }
};
#endif
