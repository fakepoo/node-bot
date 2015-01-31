#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
    public:
        Motor();
        virtual ~Motor();

        void setVelocity(int velocity);
    protected:
        int velocity;
    private:
};

#endif // MOTOR_H
