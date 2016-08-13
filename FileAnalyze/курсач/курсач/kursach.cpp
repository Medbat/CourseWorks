// kursach.cpp: определяет точку входа для консольного приложения.
//
/*

#include <iostream>
#include "stdlib.h"
#include <Time.h>
#include "Digit.h"
#include "Digit10.h"
#include "Long.h"
using namespace std;

int main()
{
	srand(time(NULL));
	Digit **z = new Digit*[10];
	for (int i = 0; i<10; i++)
		z[i] = new Digit10(rand()%10);
	Long ex1, ex2;
	ex1.setNum(z, 10, false);
	ex1 = ex2 = -ex1;
	ex1.outNum();
	cout<<endl;
	ex2.outNum();

	return 0;
}

*/