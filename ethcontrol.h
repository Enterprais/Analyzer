#ifndef ETHCONTROL_H
#define ETHCONTROL_H

#include <QObject>
#include <QWidget>
#include <QUdpSocket>

struct e_setting
{
    quint16 Port;
    QHostAddress MyAddress;
    QHostAddress DAddress;
};

class Ethcontrol : public QWidget
{
    Q_OBJECT
public:
    explicit Ethcontrol(QWidget *parent = nullptr);
    ~Ethcontrol();
    bool OpenPort();
    bool ClosePort(int ch);
    void SetSetting(e_setting set);
    void WriteData(const QByteArray &data);
    e_setting GetSetting();


private:
    void CheckCon();
    void ReadData();

    QByteArray buffer;
    QUdpSocket *socket;
    e_setting setting;

};

#endif // ETHCONTROL_H
