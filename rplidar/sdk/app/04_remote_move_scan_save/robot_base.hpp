#ifndef ROBOT_BASE_H
#define ROBOT_BASE_H

#include <stdlib.h>
#include <string>

class RobotBase
{
	private:
		std::string usb_no;
		void sendCommand(std::string command);
	public:
		RobotBase();
		void setUSB(int usb_no);
		void right();
		void left();
		void forward();
		void backward();
		void move();
		void stop();
		void makeBuzzerNoise();
};

#endif