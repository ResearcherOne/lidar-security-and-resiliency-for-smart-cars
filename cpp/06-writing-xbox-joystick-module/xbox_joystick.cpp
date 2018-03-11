#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include "xbox_joystick.hpp"

int fd;
struct js_event jse;
void (*joystickEventHandler)(xboxControllerEvents);

int open_joystick(char *device_name) {
  int fd = -1;

  if (device_name == NULL) {
    return fd;
  }

  fd = open(device_name, O_RDONLY | O_NONBLOCK);

  if (fd < 0) {
    printf("Could not locate joystick device %s\n", device_name);
    //exit(1);
  }

  return fd;
}

void print_device_info(int fd) {
	int axes=0, buttons=0;
	char name[128];

	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(sizeof(name)), &name);

	printf("%s\n  %d Axes %d Buttons\n", name, axes, buttons);
}

void process_event(struct js_event jse) {
	printf("EVENT IS RECEIVED \n");
	if (jse.type == 1 && jse.value > 0) {
		xboxControllerEvents xboxEvent;
		if(jse.number == 0) {
			joystickEventHandler(A_PRESS);
		} else if (jse.number == 1) {
			joystickEventHandler(B_PRESS);
		} else if (jse.number == 2) {
			joystickEventHandler(X_PRESS);
		} else if (jse.number == 3) {
			joystickEventHandler(Y_PRESS);
		} else if (jse.number == 11) {
			joystickEventHandler(LEFTWARD_PRESS);
		} else if (jse.number == 12) {
			joystickEventHandler(RIGHTWARD_PRESS);
		} else if (jse.number == 13) {
			joystickEventHandler(UPWARD_PRESS);
		} else if (jse.number == 14) {
			joystickEventHandler(DOWNWARD_PRESS);
		} else if (jse.number == 5) {
			joystickEventHandler(RB_PRESS);
		} else if (jse.number == 4) {
			joystickEventHandler(LB_PRESS);
		} else if (jse.number == 7) {
			joystickEventHandler(START_PRESS);
		} else if (jse.number == 6) {
			joystickEventHandler(BACK_PRESS);
		} else if (jse.number == 9) {
			joystickEventHandler(LEFT_JOYSTICK_CLICK_PRESS);
		} else if (jse.number == 10) {
			joystickEventHandler(RIGHT_JOYSTICK_CLICK_PRESS);
		}
	} else if (jse.type == 1 && jse.value == 0) {
		xboxControllerEvents xboxEvent;
		if(jse.number == 0) {
			joystickEventHandler(A_RELEASE);
		} else if (jse.number == 1) {
			joystickEventHandler(B_RELEASE);
		} else if (jse.number == 2) {
			joystickEventHandler(X_RELEASE);
		} else if (jse.number == 3) {
			joystickEventHandler(Y_RELEASE);
		} else if (jse.number == 11) {
			joystickEventHandler(LEFTWARD_RELEASE);
		} else if (jse.number == 12) {
			joystickEventHandler(RIGHTWARD_RELEASE);
		} else if (jse.number == 13) {
			joystickEventHandler(UPWARD_RELEASE);
		} else if (jse.number == 14) {
			joystickEventHandler(DOWNWARD_RELEASE);
		} else if (jse.number == 5) {
			joystickEventHandler(RB_RELEASE);
		} else if (jse.number == 4) {
			joystickEventHandler(LB_RELEASE);
		} else if (jse.number == 7) {
			joystickEventHandler(START_RELEASE);
		} else if (jse.number == 6) {
			joystickEventHandler(BACK_RELEASE);
		} else if (jse.number == 9) {
			joystickEventHandler(LEFT_JOYSTICK_CLICK_RELEASE);
		} else if (jse.number == 10) {
			joystickEventHandler(RIGHT_JOYSTICK_CLICK_RELEASE);
		}
	}
}


bool initializeXboxJoystick(char *fileToOpen) {
	fd = open_joystick(fileToOpen); //default value: "/dev/input/js0"
	if(fd<0){
		return false;
	} else {
		print_device_info(fd);
		return true;
	}
}
void processJoystickEvents(){
	while (read(fd, &jse, sizeof(jse)) > 0) {
      process_event(jse);
    }
}
void setXboxEventHandler(void (*callbackFunc)(xboxControllerEvents)){
	joystickEventHandler = callbackFunc;
}