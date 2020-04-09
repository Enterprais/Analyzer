#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Data::getInstance();
    ui->setupUi(this);
    connect(ui->Record, &QPushButton::clicked, this, &MainWindow::AddData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddData()
{
    Data::getInstance().GetDataStream(0)->AddDataToStream(random.generate()%100);
}

