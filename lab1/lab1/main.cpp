#include"Car.hpp"
#include<iostream>
int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		Car car;
	}
	else
	{
		std::cout << "Invalid parameters. Programm has terminated." << std::endl;
	}
	return 0;

}


