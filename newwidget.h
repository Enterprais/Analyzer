#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QWidget>

namespace Ui {
class NewWidget;
}

class NewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewWidget(QWidget *parent = nullptr);
    ~NewWidget();

private:
    Ui::NewWidget *ui;
};

#endif // NEWWIDGET_H
