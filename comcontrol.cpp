#include "comcontrol.h"

COMControl::COMControl(QObject *parent) : QObject(parent)
{

}

COMControl::~COMControl()
{
    delete SerialPort;
    delete Setting;
}

void COMControl::WriteData(const QByteArray &data)
{
    SerialPort->write(data);
}

void COMControl::ReadData()
{

}

void COMControl::OpenPort()
{

}

void COMControl::ClosePort()
{
    if (SerialPort->isOpen())
        SerialPort->close();
}
