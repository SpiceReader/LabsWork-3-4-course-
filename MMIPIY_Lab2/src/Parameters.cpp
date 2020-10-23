#include "pch.h"
#include "Parameters.h"
#include <iostream>
using namespace std;

Parameters::Parameters()
{
	for (int i = 0; i < 10; i++)
	{
		cout << " Input e[" << i << "] parameter :  ";
		cin >> e[i];
	}
}

void Parameters::del_uComputing(int a, int b, int c)
{
	del_u = q0 * e[a] + q1 * e[b] + q2 * e[c];
}




