#include "pch.h"
#include "linearequation.h"
#include <iostream>
using namespace std;


void linearequation::Calculating()
{
	cout << "Linear Model" << endl << endl;
	arrayOfInputTemperature[0] = 3;
	for (int i = 0; i < 10; i++)
	{
		cout << "Input u : ";
		cin >> arrayOf_u[i];
	}
	cout << "t" << " |  " << "u" << "  | " << " InputTemperature " << endl << endl;
	for (int i = 0; i < 10; i++) 
	{
		arrayOfInputTemperature[i+1] = a*arrayOfInputTemperature[i] + b*arrayOf_u[i];
		cout << i+1 << " | " << arrayOf_u[i] << " | " << arrayOfInputTemperature[i + 1] << endl;
	}
}

