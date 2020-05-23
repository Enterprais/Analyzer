#ifndef COMCONTROL_H
#define COMCONTROL_H

#include <QObject>
#include <QWidget>
#include <QSerialPort>

struct s_settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class COMControl : public QWidget
{
    Q_OBJECT
public:
    explicit COMControl(QWidget *parent = nullptr);
    ~COMControl();
    bool OpenPort();
    bool ClosePort();
    void SetSetting(s_settings set);
    void WriteData(const QByteArray &data);
    s_settings GetSetting();

    QSerialPort *SerialPort;

private:
    void CheckCon();
    void ReadData();

    s_settings *Setting;
    QByteArray buffer;

signals:

};

#endif // COMCONTROL_H
