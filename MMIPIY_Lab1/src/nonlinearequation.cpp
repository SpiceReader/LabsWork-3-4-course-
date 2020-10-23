#include "pch.h"
#include "nonlinearequation.h"
#include <iostream>
#include <math.h>
using namespace std;

void nonlinearequation::Calculating() {

	cout << "Nonlinear Model " << endl << endl;
	//cout.width(8);
	
	arrayOfInputTemperature[0] = 1;
	arrayOfInputTemperature[1] = 0;
	for (int i = 0; i < 10; i++)
	{
		cout << "Input u : ";
		cin >> arrayOf_u[i];
	}
	cout << "t" << " |  " << "u" << "  | " << " InputTemperature " << endl << endl;
	for (int i = 1; i < 10; i++)
	{
		arrayOfInputTemperature[i+1] = a*arrayOfInputTemperature[i] - b*pow(arrayOfInputTemperature[i - 1], 2) + c*arrayOf_u[i] + d * sin(arrayOf_u[i-1]);
		cout << i+1 << " | " << arrayOf_u[i-1] << " | " << arrayOfInputTemperature[i - 1] << endl;
	}

	
}

