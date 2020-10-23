#include "pch.h"
#include "Equation.h"
#include <iostream>
using namespace std;

Equation::Equation()
{
	cout << " Input u[0] parameter :  ";
	cin >> u[0];
}

void Equation::Calculating()
{
	cout << "          u   " << "  |         del_u " << endl;
	for (int i = 1; i < 10; i++)
	{	
		for (int n = 2; n < 10; n++)
		{
			del_uComputing(n, n - 1, n - 2);
		}
		u[i] = u[i - 1] + del_u;
		cout << "   " << i << ". " << u[i] << "  " << del_u << endl;
		
	}
}

