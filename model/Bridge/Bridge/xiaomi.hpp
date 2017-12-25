#ifndef __XIAOMI_HPP_
#define __XIAOMI_HPP_

class XiaoMi : public Phone
{
public:
	XiaoMi() : Phone() {}
	~XiaoMi() {}

	virtual void StartSoftWares(int softwares_name) { std::cout << "taobao" << std::endl; }
};
#endif
