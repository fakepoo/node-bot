#ifndef CONSOLEROBOT_H
#define CONSOLEROBOT_H

#include "IRobot.h"

class ConsoleRobot : public IRobot
{
    public:
        ConsoleRobot();
        virtual ~ConsoleRobot();

        void setVelocity(int leftVelocity, int rightVelocity);

    protected:
    private:
};

#endif // CONSOLEROBOT_H
