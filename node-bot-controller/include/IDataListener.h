#ifndef IDATALISTENER_H
#define IDATALISTENER_H


class IDataListener
{
    public:
        virtual void onDataReceived(char *data, int numberOfBytes) = 0;
};

#endif // IDATALISTENER_H
