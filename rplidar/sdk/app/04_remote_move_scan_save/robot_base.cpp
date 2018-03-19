#include <iostream>
#include <stdlib.h>
#include <string>
#include "robot_base.hpp"
#include <sstream>

RobotBase::RobotBase()
{

}
void RobotBase::sendCommand(std::string command)
{
	system(("sudo echo \""+command+"\" | sudo tee /dev/ttyUSB"+this->usb_no).c_str());
}
void RobotBase::setUSB(int usb_no) //use remote control for doing so
{
	std::stringstream convert;
	convert << usb_no;
	this->usb_no = convert.str();
	//system(("sudo screen /dev/ttyUSB"+this->usb_no).c_str());
}
void RobotBase::right()
{
	sendCommand("set_right");
}
void RobotBase::left()
{
	sendCommand("set_left");
}
void RobotBase::forward()
{
	sendCommand("set_forward");
}
void RobotBase::backward()
{
	sendCommand("set_backward");
}
void RobotBase::move()
{
	sendCommand("move");
}
void RobotBase::stop()
{
	sendCommand("stop");
}
void RobotBase::makeBuzzerNoise() // remote control, button X
{
	sendCommand("buzzer_noise");
}