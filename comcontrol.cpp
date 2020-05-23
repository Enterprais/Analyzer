#include "comcontrol.h"
#include "mainwindow.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

COMControl::COMControl(QWidget *parent) :
    QWidget(parent),
    SerialPort(new QSerialPort(this)),
    Setting(new s_settings)
{

    Setting->name = "COM1";
    Setting->baudRate = QSerialPort::Baud38400;
    Setting->dataBits = QSerialPort::Data8;
    Setting->parity = QSerialPort::NoParity;
    Setting->stopBits = QSerialPort::OneStop;
    Setting->flowControl = QSerialPort::NoFlowControl;

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
    if(!((MainWindow*)(parentWidget()))->GetWrite())
        return;
    char *p = new char[1];
    int count = 0;
    SerialPort->peek(p, 1);
    count = p[0] + 1;
    if(SerialPort->bytesAvailable() > count )
    {
        p = new char[count];
        SerialPort->read(p, count);
    }else
    {
        delete[] p;
        return;
    }

    int data = 0;
    int step = 1;
    for (int i=1 ;i < count; i++)
    {
        data += p[i]*step;
        step *= 10;
    }

    ((MainWindow*)(parentWidget()))->AddData(data);
    delete[] p;

}

void COMControl::CheckCon()
{
    buffer.append(SerialPort->readLine());
}


bool COMControl::OpenPort()
{
    disconnect(SerialPort, &QSerialPort::readyRead, this, &COMControl::ReadData);
    connect(SerialPort, &QSerialPort::readyRead, this, &COMControl::CheckCon);
    bool con = false;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos)
    {
        SerialPort->setPortName(info.portName());
        SerialPort->setBaudRate(Setting->baudRate);
        SerialPort->setDataBits(Setting->dataBits);
        SerialPort->setParity(Setting->parity);
        SerialPort->setStopBits(Setting->stopBits);
        SerialPort->setFlowControl(Setting->flowControl);
        buffer.clear();

        if (SerialPort->open(QIODevice::ReadWrite)) {
            SerialPort->write("0");
            SerialPort->waitForReadyRead(20);
            buffer.remove(7,2);
            if(QString::fromUtf8(buffer) == "ADCv2.0")
            {
                con = true;
                disconnect(SerialPort, &QSerialPort::readyRead, this, &COMControl::CheckCon);
                connect(SerialPort, &QSerialPort::readyRead, this, &COMControl::ReadData);
                break;
            }
            QThread::msleep(10);
            ClosePort();
        }
        buffer.clear();
    }
    return con;
}

bool COMControl::ClosePort()
{
    if (SerialPort->isOpen())
    {
        WriteData("-1");
        SerialPort->close();
        return true;
    } else return false;
}

s_settings COMControl::GetSetting()
{
    return *Setting;
}

void COMControl::SetSetting(s_settings set)
{
    Setting->parity = set.parity;
    Setting->baudRate = set.baudRate;
    Setting->dataBits = set.dataBits;
    Setting->stopBits = set.stopBits;
    Setting->flowControl = set.flowControl;
}
