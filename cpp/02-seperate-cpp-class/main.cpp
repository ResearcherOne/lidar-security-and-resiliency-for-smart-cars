#include <iostream>
#include "dog.h"

using namespace std;

//Compiling the example on Ubuntu: g++ main.cpp -o main.o dog.h dog.cpp
int main()
{
	cout <<"This program shows ultra simple class that is seperated from main program. \n";
	
	Dog stanley;
	stanley.bark();
	return 0;
}