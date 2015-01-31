#include "Motor.h"

Motor::Motor()
{
    setVelocity(0);
}

Motor::~Motor()
{}

void Motor::setVelocity(int velocity)
{
	// TODO: Validate the velocity

	if(this->velocity != velocity)
	{
		// TODO: Set the velocity of the motor

		this->velocity = velocity;
	}
}
