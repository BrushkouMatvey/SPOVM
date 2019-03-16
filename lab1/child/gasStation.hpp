#pragma once
#include <string>

class gasStation
{
public:
	void dataAnalisis(int costOfFill, int &gasStationLitres, int numLitres,int typeOfFill);
	void showAmountOfFuel()const;
	void fileGasolineInit(std::string fileName);
	bool isFileExist(std::string fileName);

	void fielTheCar(int typeOfFiel);
	gasStation(int argc, char * argv[]);
	~gasStation();
private:
	int balance;
};