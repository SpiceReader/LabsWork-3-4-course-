#include "pch.h"
#include "linearequation.h"
#include "nonlinearequation.h"
#include <iostream>
using namespace std;

int main() {
	linearequation lineq;
	nonlinearequation nonlineq;
	lineq.Calculating();
	cout << endl << endl;
	nonlineq.Calculating();
	return 0;
}