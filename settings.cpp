#include "settings.h"
#include "ui_settings.h"
#include <QHostAddress>
#include <QSerialPort>
#include <QMessageBox>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->cb_baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->cb_baudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->cb_baudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->cb_baudrate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->cb_databits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->cb_databits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->cb_databits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->cb_databits->addItem(QStringLiteral("8"), QSerialPort::Data8);

    ui->cb_parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->cb_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->cb_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->cb_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->cb_parity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->cb_stopbits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->cb_stopbits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->cb_stopbits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->cb_flow->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->cb_flow->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->cb_flow->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::updateSettings()
{
    s_settings ss;
    e_setting es;

    ss.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->cb_baudrate->itemData(ui->cb_baudrate->currentIndex()).toInt());
    ss.dataBits = static_cast<QSerialPort::DataBits>(
                ui->cb_databits->itemData(ui->cb_databits->currentIndex()).toInt());
    ss.parity = static_cast<QSerialPort::Parity>(
                ui->cb_parity->itemData(ui->cb_parity->currentIndex()).toInt());
    ss.stopBits = static_cast<QSerialPort::StopBits>(
                ui->cb_stopbits->itemData(ui->cb_stopbits->currentIndex()).toInt());
    ss.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->cb_flow->itemData(ui->cb_flow->currentIndex()).toInt());

    QHostAddress adr1, adr2;
    if(!adr1.setAddress(ui->le_ip->text()))
    {
        QMessageBox::critical(this, tr("Ошибка ввода"), "IP-адрес устройства введен некорректно");
        return;
    }

    if(!adr2.setAddress(ui->le_ip_d->text()))
    {
        QMessageBox::critical(this, tr("Ошибка ввода"), "IP-адрес датчика введен некорректно");
        return;
    }

    quint16 p = ui->le_port->text().toUInt();

    if(p <= 0 || p > 65000)
    {
        QMessageBox::critical(this, tr("Ошибка ввода"),
                              "Порт введен некорректно. Значение должно быть от 1 до 65000");
        return;
    }

    es.Port = p;
    es.DAddress = adr2;
    es.MyAddress = adr1;

    ((MainWindow*)(parentWidget()))->GetComCon()->SetSetting(ss);
    ((MainWindow*)(parentWidget()))->GetEthCon()->SetSetting(es);
};


void Settings::on_buttonBox_accepted()
{
    updateSettings();
}

void Settings::showEvent(QShowEvent *)
{
    s_settings ss = ((MainWindow*)(parentWidget()))->GetComCon()->GetSetting();
    e_setting es = ((MainWindow*)(parentWidget()))->GetEthCon()->GetSetting();

    ui->le_port->setText(QString::number(es.Port));
    ui->le_ip->setText(es.MyAddress.toString());
    ui->le_ip_d->setText(es.DAddress.toString());

    ui->cb_flow->setCurrentIndex(ui->cb_flow->findData(ss.flowControl));
    ui->cb_baudrate->setCurrentIndex(ui->cb_baudrate->findData(ss.baudRate));
    ui->cb_parity->setCurrentIndex(ui->cb_parity->findData(ss.parity));
    ui->cb_databits->setCurrentIndex(ui->cb_databits->findData(ss.dataBits));
    ui->cb_stopbits->setCurrentIndex(ui->cb_stopbits->findData(ss.stopBits));
}
