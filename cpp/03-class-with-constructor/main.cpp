#include <iostream>
#include "dog.h"

using namespace std;

//Compiling the example on Ubuntu: g++ main.cpp -o main.o dog.h dog.cpp
int main()
{
	int tag_number = 1;
	cout <<"This program will create dog with tag number "<<tag_number<<" using constructor of dog class \n";
	
	Dog stanley(tag_number);
	stanley.bark();
	return 0;
}