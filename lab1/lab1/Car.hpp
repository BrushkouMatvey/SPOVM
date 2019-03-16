#pragma once
#include <string>
class Car
{
public:
	Car();
	~Car();
	void setTankCapacity(int tankCapacity);
	int getTankCapacity();
private:
	int tankCapacity;
	void RunChild(const std::string& params) const;
	void runMenu() const;
	void controller() const;
};