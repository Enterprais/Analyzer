#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <comcontrol.h>
#include "exporter.h"
#include "ethcontrol.h"
#include "settings.h"

class Settings;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum ConType {Com, Usb, Eth};
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddData(int data);
    void OpenSerialPort();
    void CloseSerialPort();
    void OpenEthPort();
    void CloseEthPort();
    void UpdateScan(int X, int Y);
    bool GetWrite();
    COMControl* GetComCon();
    Ethcontrol* GetEthCon();

    bool isConnected;
    bool isMonitor;
    int ConChannels;

private:

    Ui::MainWindow *ui;
    int CurChanWrite;
    int PointRec;
    bool isWrite;
    COMControl *COMcontr;
    Ethcontrol *Ethcontr;
    Exporter Exporter;
    Settings *SetDialog;
    ConType Ctype;

    void RecordStart();
    void Monitor();
    void ClearBuffer();
    void Connect();
    void OpenSetting();
    void Disconnect();
    void UpdateState();
    void Export();
    void ChangeChannel(QString index);

};
#endif // MAINWINDOW_H
