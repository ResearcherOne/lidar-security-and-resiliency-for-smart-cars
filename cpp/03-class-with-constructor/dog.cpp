#include <iostream>
#include "dog.h"

using namespace std;


Dog::Dog(int tag_no)
{
	tag_number = tag_no;
}

void Dog::bark()
{
	cout << "Dog "<< tag_number <<": Get the hell out of my zone you little human!! \n";
}