#include <iostream>

using namespace std;

int main()
{
	cout << "Change state_no variable for tweaking the program. \n";

	int state_no = 1;
	if (state_no == 0) {
		goto yoyo;
	} else if (state_no == 1) {
		goto aha;
	} else {
		goto hai;
	}

	yoyo:
		cout << "Yo yo! \n";
	aha:
		cout << "Aha! \n";
	hai:
		cout << "Hai! \n";
	return 0;
}