#include <iostream>

using namespace std;

//Compiling the example on Ubuntu: g++ main.cpp -o main.o
class A2DD
{
	private:
		int gx;
		int gy;
	public:
		void setValues(int x, int y);
		int getSum();

};

void A2DD::setValues(int x,int y)
{
  gx = x;
  gy = y;
}

int A2DD::getSum()
{
  return gx + gy;
}


int main()
{
	int a = 3;
	int b = 4;
	cout << "Hello, Classes! This program will calculate "<<a<<"+"<<b<<" using a C++ class"<<"\n";

	A2DD yo;
	yo.setValues(a,b);
	cout <<"Here is the sum: "<<yo.getSum()<<"\n";
	return 0;
}