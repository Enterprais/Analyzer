#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ethcontrol.h"
#include "comcontrol.h"
#include "mainwindow.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    void updateSettings();

private slots:
    void on_buttonBox_accepted();
    void showEvent(QShowEvent *) override;

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
