#include "Robot.h"
#include <iostream>

using namespace std;

Robot::Robot()
{
    // Initialize all of the values
    setVelocity(0, 0);
}

Robot::~Robot()
{
    //dtor
}

void Robot::setVelocity(int leftVelocity, int rightVelocity)
{
	cout << "Robot: Setting velocity to " << leftVelocity << " | " << rightVelocity << endl;

	this->leftMotor.setVelocity(leftVelocity);
	this->rightMotor.setVelocity(rightVelocity);
}
