#pragma once
#include "MainClass.h"
class Parameters: public MainClass
{
private:
	double e[10];
public:
	double del_u;
	Parameters();
	void del_uComputing(int a, int b, int c);
};

