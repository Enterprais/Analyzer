#include "ethcontrol.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QThread>

Ethcontrol::Ethcontrol(QWidget *parent) :
    QWidget(parent),
    socket(new QUdpSocket(this))
{
    setting.Port = 5533;
    setting.MyAddress.setAddress("192.168.1.66");
    setting.DAddress.setAddress("192.168.1.10");
}

Ethcontrol::~Ethcontrol()
{
    delete socket;
}
void Ethcontrol::ReadData()
{
    QByteArray buf;
    buf.resize(socket->pendingDatagramSize());
    if(((MainWindow*)(parentWidget()))->GetWrite())
    {
        socket->readDatagram(buf.data(), buf.size());
        int data = buf.toInt();
        ((MainWindow*)(parentWidget()))->AddData(data);
    }
    else
         socket->readDatagram(buf.data(), 0);
}

void Ethcontrol::CheckCon()
{
    quint64 s = socket->pendingDatagramSize();
    if(s != 7)
    {
        socket->readDatagram(buffer.data(), 0);
        socket->writeDatagram("0", setting.DAddress, setting.Port);
        socket->waitForReadyRead(100);
    }
    buffer.resize(socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size());
}

void Ethcontrol::WriteData(const QByteArray &data)
{
    socket->writeDatagram(data, setting.DAddress, setting.Port);
}

bool Ethcontrol::OpenPort()
{
    socket->open(QUdpSocket::OpenModeFlag::ReadWrite);
    disconnect(socket, &QUdpSocket::readyRead, this, &Ethcontrol::ReadData);
    connect(socket, &QUdpSocket::readyRead, this, &Ethcontrol::CheckCon);
    bool con = false;

    if(socket->bind(setting.MyAddress, setting.Port))
    {
        buffer.clear();
        socket->writeDatagram("0", setting.DAddress, setting.Port);
        socket->waitForReadyRead(100);
        if(QString::fromUtf8(buffer) == "ADCv2.0")
        {
            con = true;
            disconnect(socket, &QUdpSocket::readyRead, this, &Ethcontrol::CheckCon);
            connect(socket, &QUdpSocket::readyRead, this, &Ethcontrol::ReadData);
        }
        else
        {
            socket->close();
        }
    }
    buffer.clear();
    return con;
}

bool Ethcontrol::ClosePort(int ch)
{
    if(((MainWindow*)(parentWidget()))->GetWrite())
      WriteData(QByteArray::number(ch));
    socket->close();
    return true;
}

e_setting Ethcontrol::GetSetting()
{
    return setting;
}

void Ethcontrol::SetSetting(e_setting set)
{
    setting.Port = set.Port;
    setting.DAddress = set.DAddress;
    setting.MyAddress = set.MyAddress;
}

