#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Data::getInstance();
    ui->setupUi(this);
    connect(ui->Record, &QPushButton::clicked, this, &MainWindow::RecordStart);
    connect(ui->Monitor, &QPushButton::clicked, this, &MainWindow::Monitor);

    connect(ui->b_scanLeftX, &QPushButton::clicked, ui->widget,  &Plotter::rXDown);
    connect(ui->b_scanRightX, &QPushButton::clicked, ui->widget,  &Plotter::rXUp);

    connect(ui->b_scanLeftY, &QPushButton::clicked, ui->widget,  &Plotter::rYDown);
    connect(ui->b_scanRightY, &QPushButton::clicked, ui->widget,  &Plotter::rYUp);

    connect(ui->b_Connect, &QPushButton::clicked, this, &MainWindow::Connect);
    connect(ui->b_export, &QPushButton::clicked, this, &MainWindow::Export);
    connect(ui->b_set, &QPushButton::clicked, this, &MainWindow::OpenSetting);

    connect(ui->b_clear, &QPushButton::clicked, &Data::getInstance(), &Data::ClearStreams);
    connect(ui->b_clear, &QPushButton::clicked, ui->widget, &Plotter::Update);
    connect(ui->b_clear, &QPushButton::clicked, this, &MainWindow::ClearBuffer);

    connect(ui->c_grid, &QPushButton::toggled, ui->widget,  &Plotter::setGrid);

    connect(ui->widget, &Plotter::ScanChanged, this, &MainWindow::UpdateScan);

    connect(ui->CB_ActiveCh, &QComboBox::currentTextChanged, this, &MainWindow::ChangeChannel);

    ui->CB_ConType->addItem("COM");
    ui->CB_ConType->addItem("Ethernet");
    ui->CB_ConType->addItem("USB");

    ui->CB_ChannelNum->addItem("1");
    ui->CB_ChannelNum->addItem("2");
    ui->CB_ChannelNum->addItem("3");
    ui->CB_ChannelNum->addItem("4");

    ui->CB_export->addItem("jpeg");
    ui->CB_export->addItem("xlsx");
    ui->CB_export->addItem("wav");

    isConnected = false;
    isWrite = false;
    isMonitor = false;
    CurChanWrite = 0;
    PointRec = 0;

    Ethcontr = new Ethcontrol(this);
    COMcontr = new COMControl(this);
    SetDialog = new Settings(this);

    UpdateScan(50, 500);
    UpdateState();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete COMcontr;
    delete Ethcontr;
    delete SetDialog;
}

void MainWindow::RecordStart()
{
    isMonitor = false;
    if(isWrite)
    {

        ui->Record->setText("Запись");
        ui->b_export->setEnabled(true);
        isWrite = false;
        ui->Monitor->setEnabled(true);
        CurChanWrite = 0;
        switch (Ctype)
        {
            case MainWindow::ConType::Com:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Usb:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Eth:
            {
                Ethcontr->WriteData(QByteArray::number(ConChannels+1));

            }break;
        }
    }
    else
    {
        ui->Record->setText("Стоп");
        isWrite = true;
        ui->Monitor->setEnabled(false);
        ui->b_export->setEnabled(false);
        CurChanWrite = 0;
        switch (Ctype)
        {
            case MainWindow::ConType::Com:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Usb:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Eth:
            {
                Ethcontr->WriteData(QByteArray::number(ConChannels+1));
            }break;
        }
    }
}

void MainWindow::Monitor()
{
    if(isMonitor)
    {
        isMonitor = false;
        ui->b_export->setEnabled(true);
        ui->Monitor->setText("Мониторинг");
        CurChanWrite = 0;
        switch (Ctype)
        {
            case MainWindow::ConType::Com:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Usb:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Eth:
            {
                Ethcontr->WriteData(QByteArray::number(ConChannels+1));
            }break;

        }
    }
    else
    {
        isMonitor = true;
        ui->b_clear->clicked();
        ui->Monitor->setText("Остановить\nмониторинг");
        CurChanWrite = 0;
        switch (Ctype)
        {
            case MainWindow::ConType::Com:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Usb:
            {
                COMcontr->WriteData(QByteArray::number(ConChannels+1));
                COMcontr->SerialPort->clear(QSerialPort::Direction::Input);
            }break;
            case MainWindow::ConType::Eth:
            {
                Ethcontr->WriteData(QByteArray::number(ConChannels+1));
            }break;
        }
    }
}

void MainWindow::ClearBuffer()
{
    PointRec = 0;
    ui->l_points->setText(QString::number(PointRec));

}

void MainWindow::Export()
{
    if(PointRec < 1)
    {
        QMessageBox::critical(this, tr("Экспорт"),
                                     "Нет данных для экспорта");
        return;
    }
    switch (ui->CB_export->currentIndex()) {
    //jpeg
    case 0:
    {
        Exporter.ExportImage(ui->widget->GetImage());
    }break;
    //xls
    case 1:{
        Exporter.ExportXls(Data::getInstance().GetData());
    } break;
    //wav
    case 2:
    {
        Exporter.ExportWav(Data::getInstance().GetData());
    }break;
    default: break;

    }

}

void MainWindow::AddData(int data)
{
    if(CurChanWrite > 4)
        CurChanWrite = 0;

    if(CurChanWrite != 4)
        if(Data::getInstance().GetDataStream(CurChanWrite) != nullptr)
        {
            Data::getInstance().GetDataStream(CurChanWrite)->AddDataToStream(data);
            if(!isMonitor)
                PointRec++;
        }


    CurChanWrite++;
    ui->l_points->setText(QString::number(PointRec));
}

void MainWindow::Connect()
{
    if(!isConnected)
    {
        if(ui->CB_ConType->currentText() == "COM" ||  ui->CB_ConType->currentText() == "USB")
            OpenSerialPort();
        else
            OpenEthPort();
    }
    else
    {
        Disconnect();
    }

    UpdateState();

}

void MainWindow::Disconnect()
{
    CloseEthPort();
    CloseSerialPort();
}

void MainWindow::OpenSerialPort()
{
    if(COMcontr->OpenPort())
    {
        isConnected = true;
        ConChannels = ui->CB_ChannelNum->currentText().toInt() - 1;
        ui->CB_ActiveCh->clear();
        for (int i = 0; i <= ConChannels; i++) {
            ui->CB_ActiveCh->addItem(QString::number(i+1));
        }
        CurChanWrite = 0;
        Ctype = ConType::Com;
        ui->b_clear->clicked();
    }

    else
        QMessageBox::critical(this, tr("Ошибка подключения"),
                              "Устройство не найдено! Проверьте настройки и соединение");
}

void MainWindow::OpenEthPort()
{
    if(Ethcontr->OpenPort())
    {
        isConnected = true;
        ConChannels = ui->CB_ChannelNum->currentText().toInt() - 1;
        ui->CB_ActiveCh->clear();
        for (int i = 0; i <= ConChannels; i++) {
            ui->CB_ActiveCh->addItem(QString::number(i+1));
        }
        CurChanWrite = 0;
        Ctype = ConType::Eth;
        ui->b_clear->clicked();
    }

    else
        QMessageBox::critical(this, tr("Ошибка подключения"),
                              "Устройство не найдено! Проверьте настройки и соединение");
}

void MainWindow::CloseSerialPort()
{
    if(COMcontr->ClosePort())
        isConnected = false;
    CurChanWrite = 0;
    isWrite = false;
}

void MainWindow::CloseEthPort()
{
    if(Ethcontr->ClosePort(ConChannels+1))
    {
        isConnected = false;
    }
    CurChanWrite = 0;
    isWrite = false;
}

void MainWindow::UpdateState()
{
    if(isConnected)
    {
         ui->b_Connect->setText("Отключить");
         ui->CB_ConType->setEnabled(false);
         ui->CB_ChannelNum->setEnabled(false);
         ui->Record->setEnabled(true);
         ui->Monitor->setEnabled(true);
         ui->b_set->setEnabled(false);
    }
    else
    {
        ui->b_Connect->setText("Подключить");
        ui->Record->setText("Запись");
        ui->CB_ConType->setEnabled(true);
        ui->CB_ChannelNum->setEnabled(true);
        ui->Record->setEnabled(false);
        ui->Monitor->setEnabled(false);
        ui->b_set->setEnabled(true);
    }

}

void MainWindow::ChangeChannel(QString index)
{
    ui->widget->RepaintPlot(Data::getInstance().GetDataStream(index.toInt()-1));
}

void MainWindow::UpdateScan(int X, int Y)
{
    ui->l_scanX->setText(QString::number(X) + " мс");
    ui->l_scanY->setText(QString::number(Y) + " ед");
}

bool MainWindow::GetWrite()
{
    return  (isWrite || isMonitor);
}

void MainWindow::OpenSetting()
{
    SetDialog->exec();
}

COMControl* MainWindow::GetComCon()
{
    return  COMcontr;
}

Ethcontrol* MainWindow::GetEthCon()
{
    return Ethcontr;
}
