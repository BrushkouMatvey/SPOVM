#include "gasStation.hpp"
#include<Windows.h>
#include<fstream>
#include<iostream>

#define CLS "cls"
#define COST92 3
#define COST95 4
#define COST98 5


void gasStation::showAmountOfFuel() const
{
	std::ifstream fin("balance.bin");
	system(CLS);
	int gasoline[3];
	std::cout << "Amount of fuel at a gas station" << std::endl;
	fin.read((char*)& gasoline, 3 * sizeof(int));
	std::cout << "Gasoline AI - 92 : " << gasoline[0] << " litres." << std::endl;
	std::cout << "Gasoline AI - 95 : " << gasoline[1] << " litres." << std::endl;
	std::cout << "Gasoline AI - 98 : " << gasoline[2] << " litres." << std::endl;
	
	fin.close();
}

void gasStation::fileGasolineInit(std::string fileName)
{
	int DEFOULT_CAPACITY_OF_AI92 = 1000;
	int DEFOULT_CAPACITY_OF_AI95 = 2000;
	int DEFOULT_CAPACITY_OF_AI98 = 3000;

	std::ofstream fout(fileName, std::ios::binary);

	fout.write((char*)&DEFOULT_CAPACITY_OF_AI92, sizeof(DEFOULT_CAPACITY_OF_AI92));
	fout.write((char*)&DEFOULT_CAPACITY_OF_AI95, sizeof(DEFOULT_CAPACITY_OF_AI95));
	fout.write((char*)&DEFOULT_CAPACITY_OF_AI98, sizeof(DEFOULT_CAPACITY_OF_AI98));
	fout.close();
}

void gasStation::dataAnalisis(int costOfFill, int &gasStationNumLiters, int numLiters, int typeOfFill)
{
	int cashback;
	std::fstream fout("balance.bin");
	int size;
	switch (typeOfFill)
	{
	case 1:
		fout.seekp(0, std::ios::beg);
		size = fout.tellp();
		break;
	case 2:
		fout.seekp(sizeof(int), std::ios::beg);
		size = fout.tellp();
		break;
	case 3:
		fout.seekp(2 * sizeof(int), std::ios::beg);
		size = fout.tellp();
		break;
	}

	if (gasStationNumLiters >= numLiters)
	{
		gasStationNumLiters -= numLiters;
		fout.write((char*)&gasStationNumLiters, sizeof(gasStationNumLiters));
		std::cout << "Car's gassed up successfully." << std::endl <<
			"Cost of refueling: " << costOfFill << std::endl<<
			"The remainder of gasoline:"<<gasStationNumLiters<<std::endl;
	}
	else
	{
		switch (typeOfFill)
		{
		case 1:
			cashback = costOfFill - gasStationNumLiters * COST92;
			break;
		case 2:
			cashback = costOfFill - gasStationNumLiters * COST95;
			break;
		case 3:
			cashback = costOfFill - gasStationNumLiters * COST98;
			break;
		}
		
		std::cout << "Sorry. Not enough petrol at the gas station." << std::endl <<
			"Filled with " << gasStationNumLiters << " litres." << std::endl <<
			"Your cashback :" << cashback << std::endl;
		gasStationNumLiters = 0;
		fout.write((char*)&gasStationNumLiters, sizeof(gasStationNumLiters));
		
	}
	fout.close();
}


void gasStation::fielTheCar(int typeOfFiel)
{
	std::ifstream fin("balance.bin", std::ios::binary);
	std::cout << "Enter the number of liters for refueling cars." << std::endl;
	int numLiters;
	std::cin >> numLiters;
	system(CLS);

	int gasStationNumLiters;	

	int costOfFill;
	int cashback;
	int size;
	
	switch (typeOfFiel)
	{
	case 1:
	{
		costOfFill = numLiters * COST92;
		fin.seekg(0, std::ios::beg);
		size = fin.tellg();
		fin.read((char*)&gasStationNumLiters,sizeof(int));
		fin.close();
		dataAnalisis(costOfFill, gasStationNumLiters, numLiters, 1);	
		break;
	}
	case 2:
	{
		costOfFill = numLiters * COST95;
		fin.seekg(sizeof(int), std::ios::beg);
		size = fin.tellg();
		fin.read((char*)&gasStationNumLiters, sizeof(int));
		//fin >> gasStationNumLiters;
		fin.close();
		dataAnalisis(costOfFill, gasStationNumLiters, numLiters, 2);
		break;
	}
	case 3:
	{
		costOfFill = numLiters * COST98;
		fin.seekg(2 * sizeof(int), std::ios::beg);
		size = fin.tellg();
		fin.read((char*)&gasStationNumLiters, sizeof(int));
		//fin >> gasStationNumLiters;
		fin.close();
		dataAnalisis(costOfFill, gasStationNumLiters, numLiters, 3);
	
		break;
	}
		
	}
}

gasStation::gasStation(int argc, char * argv[])
{
	if (argc > 2 || argc <= 0)
	{
		std::cout << ("Invalid information");
		return;
	}

	if (isFileExist("balance.bin"))
	{
		fileGasolineInit("balance.bin");
	}

	if (argv[0] == std::string("1"))
	{
		this->fielTheCar(1);	
	}
	else if (argv[0] == std::string("2"))
	{
		this->fielTheCar(2);
	}
	else if (argv[0] == std::string("3"))
	{
		this->fielTheCar(3);
	}
	else if (argv[0] == std::string("4"))
	{
		this->showAmountOfFuel();
	}
}

gasStation::~gasStation()
{
}
bool gasStation::isFileExist(std::string fileName)
{
	int length;
	std::ifstream file(fileName);
	int a;
	if (!file.is_open())
	{
		std::cout << "File already exists" << std::endl;
		return true;
	}
	file.seekg(0, std::ios::end);
	length = file.tellg();
	if (length == 0)
	{
		return true;
	}
	return false;
	
}