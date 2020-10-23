#pragma once
#include "Parameters.h"

class Equation : public Parameters
{
private:
	double u[10];
public:
	Equation();
	void Calculating();
};

