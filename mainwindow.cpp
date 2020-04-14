#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Data::getInstance();
    ui->setupUi(this);
    connect(ui->Record, &QPushButton::clicked, this, &MainWindow::AddData);
    connect(ui->b_scanLeftX, &QPushButton::clicked, ui->widget,  &Plotter::rXDown);
    connect(ui->b_scanRightX, &QPushButton::clicked, ui->widget,  &Plotter::rXUp);

    connect(ui->b_scanLeftY, &QPushButton::clicked, ui->widget,  &Plotter::rYDown);
    connect(ui->b_scanRightY, &QPushButton::clicked, ui->widget,  &Plotter::rYUp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddData()
{
    Data::getInstance().GetDataStream(0)->AddDataToStream(random.generate()%100);
}


