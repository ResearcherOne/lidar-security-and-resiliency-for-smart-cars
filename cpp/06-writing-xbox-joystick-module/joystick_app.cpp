#include <stdlib.h>
#include <stdio.h>
#include "xbox_joystick.hpp"

void xbox_handler(xboxControllerEvents event) {
	printf("HANDLER IS CALLED \n");
	if(event == A_PRESS) {
		printf("A_PRESS \n");
	} else if (event == A_RELEASE) {
		printf("A_RELEASE \n");
	} else {
		printf("NOT A \n");
	}
}

int main() {
	initializeXboxJoystick("/dev/input/js0");
	setXboxEventHandler(xbox_handler);
	while(1) {
		processJoystickEvents();
	}
	return 0;
}
