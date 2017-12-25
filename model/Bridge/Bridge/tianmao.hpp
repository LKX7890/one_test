#ifndef __TIANMAO_HPP_
#define __TIANMAO_HPP_

#include <iostream>
#include "softwares.hpp"

class TianMao : public SoftWares
{
public:
	TianMao() : SoftWares() {}
	~TianMao(){}

	virtual void OnStartSoftWares() { std::cout << "shopping" << std::endl; this->OnStopSoftWares(); }
	void OnStopSoftWares(){ std::cout << "pay money" << std::endl; }
};
#endif
