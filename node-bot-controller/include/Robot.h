#ifndef ROBOT_H
#define ROBOT_H

#include "IRobot.h"
#include "Motor.h"

class Robot : public IRobot
{
    public:
        Robot();
        virtual ~Robot();

        void setVelocity(int leftVelocity, int rightVelocity);
    protected:
        Motor leftMotor;
        Motor rightMotor;
    private:
};

#endif // ROBOT_H
