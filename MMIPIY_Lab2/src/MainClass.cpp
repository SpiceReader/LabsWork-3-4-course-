#include "MainClass.h"
#include <iostream>
using namespace std;

MainClass::MainClass ()
{
	cout << " Input K parameter :  ";
	cin >> K;	
	cout << " Input T parameter :  ";
	cin >> T;
	cout << " Input T_0 parameter :  ";
	cin >> T_0; 
	cout << " Input T_d parameter :  ";
	cin >> T_d;	
}

void MainClass::q_params()
{
	q0 = K * (1 + T_d / T_0);
	q1 = -K * (1 + 2 * T_d / T_0 - T_0 / T);
	q2 = K * T_d / T_0;
}


