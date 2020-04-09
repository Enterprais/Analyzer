#ifndef COMCONTROL_H
#define COMCONTROL_H

#include <QObject>
#include <QWidget>
#include <QtSerialPort/QSerialPort>

class COMControl : public QObject
{
    Q_OBJECT
public:
    explicit COMControl(QObject *parent = nullptr);
    ~COMControl();

    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

private:
    void OpenPort();
    void ClosePort();
    void ReadData();
    void WriteData(const QByteArray &data);

    QSerialPort *SerialPort;
    Settings *Setting;

signals:

};

#endif // COMCONTROL_H
