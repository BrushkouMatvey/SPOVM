#include "Car.hpp"
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<fstream>
#include<iostream>

Car::Car()
{
	runMenu();
}

Car::~Car()
{
}

void Car::setTankCapacity(int tankCapacity)
{
	this->tankCapacity = tankCapacity;
}

int Car::getTankCapacity()
{
	return this->tankCapacity;
}

void Car::RunChild(const std::string & params) const
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	SecureZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	SecureZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess
	(
		NULL,
		(LPSTR)(("child.exe " + params).c_str()),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		std::cout << "Create process error." << std::endl;
		return;
	}
	SYSTEMTIME time;
	clock_t begin, end;
	begin = clock();
	while (WaitForSingleObject(pi.hProcess, 50))
	{
		GetLocalTime(&time);
		printf("Time: %02d:%02d:%02d \r", time.wHour, time.wMinute, time.wSecond);
	}
	end = clock();
	system("cls");
	std::cout << "Child process runtime: " << (double)(end - begin) / CLK_TCK << std::endl;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	controller();
}

void Car::runMenu() const
{
	do
	{
		system("cls");
		std::cout << "Choose the type of fiel:" << std::endl;
		std::cout << "\t1)Gasoline AI - 92." << std::endl;
		std::cout << "\t2)Gasoline AI - 95." << std::endl;
		std::cout << "\t3)Gasoline AI - 98." << std::endl;
		std::cout << "\t4)Show amount of fuel." << std::endl;
		std::cout << "\t5)Finish work." << std::endl;
		std::cout << "Enter action: ";
		int t;
		std::cin >> t;

		bool isGood = true;

		std::string params;

		switch (t)
		{
		case 1:
		{
			params += "1";
			break;
		}
		case 2:
		{
			params += "2";
			break;
		}
		case 3:
		{
			params += "3";
			break;
		}
		case 4:
		{
			params += "4";
			break;
		}
		case 5:
		{
			std::ofstream fOut("balance.bin", std::ios::trunc);
			fOut.close();
			return;
		}	
		default:
		{
			std::cout << "Invalid data. Repeat input again" << std::endl;
			isGood = false;
			continue;			
		}
		}
		if (isGood)
		{
			RunChild(params);
		}

	} while (true);
}

void Car::controller() const
{
	std::cout << std::endl << "Continue work? " << std::endl;
	std::cout << "1) Continue " << std::endl << "0) Finish work " << std::endl;
	int i;
	std::cin >> i;
	switch (i)
	{
	case 1:
		break;
	case 0:
		std::ofstream fOut("balance.bin", std::ios::trunc);
		fOut.close();
		exit(0);
	}
}