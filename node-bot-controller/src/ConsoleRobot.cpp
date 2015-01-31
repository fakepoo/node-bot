#include "ConsoleRobot.h"
#include <iostream>

using namespace std;

ConsoleRobot::ConsoleRobot()
{
}

ConsoleRobot::~ConsoleRobot()
{
}

void ConsoleRobot::setVelocity(int leftVelocity, int rightVelocity)
{
	cout << "ConsoleRobot: Setting velocity to " << leftVelocity << " | " << rightVelocity << endl;
}

