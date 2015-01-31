#ifndef IROBOT_H
#define IROBOT_H


class IRobot
{
    public:
        virtual void setVelocity(int leftVelocity, int rightVelocity) = 0;
};

#endif // IROBOT_H
