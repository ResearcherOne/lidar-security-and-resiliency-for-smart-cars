#include <stdio.h> // standard input / output functions
#include <stdlib.h>

int main(void)
{ 
	system("sudo echo \"trolo\" | sudo tee /dev/ttyUSB0");
	
	return(0);
	
} //main