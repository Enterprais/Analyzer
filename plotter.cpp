#include "plotter.h"
#include "ui_plotter.h"
#include <QPainter>
#include <QDebug>
#include <data.h>
#include <algorithm>
#include <mainwindow.h>


Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);
    image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
    painter = new QPainter();

    CurrentNumberStream = 0;
    MsPerBlock = ScanX[CurX];
    AmpPerBlock = ScanY[CurY];
    BlockSize = 150;
    isActive = true;
    UpdateGraph = true;
    isGrid = true;

    connect(&Data::getInstance(), &Data::DataChanged, this, &Plotter::Update);
    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &Plotter::MoveSlider);

    CurrentStream = Data::getInstance().GetDataStream(0);

}

void Plotter::paintEvent(QPaintEvent *)
{
    if(!isActive || CurrentStream == nullptr)
        return;

    painter->begin(image);
    int DotsOnScreen = image->width() / ((CurrentStream->ClockRate / MsPerBlock) * BlockSize) + 1;

    if ((CurrentStream->ClockRate / MsPerBlock) * BlockSize * CurrentStream->DataArray.size() <= image->width())
    {
        ui->horizontalScrollBar->hide();
        ui->horizontalScrollBar->setMaximum(0);
    }
    else
    {
        ui->horizontalScrollBar->show();
        ui->horizontalScrollBar->setMaximum(CurrentStream->DataArray.size() - DotsOnScreen);
        if(UpdateGraph)
            ui->horizontalScrollBar->setSliderPosition(CurrentStream->DataArray.size() - DotsOnScreen);
    }


    painter->fillRect(0,0,width(),height(),QColor(255,255,255));
    if(isGrid)
        PaintGrid();
    PaintGraph();

    painter->end();

    painter->begin(this);
    painter->drawImage(0,0,*image);
    painter->end();
}

void Plotter::MoveSlider(int value)
{
    if(value == ui->horizontalScrollBar->maximum())
        UpdateGraph = true;
    else
        UpdateGraph = false;
    this->update();

}

void Plotter::RepaintPlot(DataStream *stream)
{
    CurrentStream = stream;
    this->update();
}

void Plotter::PaintGraph()
{

    if(CurrentStream->DataArray.size() <= 1)
        return;

    int DotsOnScreen = image->width() / ((CurrentStream->ClockRate / MsPerBlock)  * BlockSize) + 1;

    int StartPos = 0;
    int EndPos = 0;

    StartPos = ui->horizontalScrollBar->sliderPosition();
    EndPos = clamp(StartPos + DotsOnScreen, 0, CurrentStream->DataArray.size());

    QPen currentPen = painter->pen();

    float ShiftX = (CurrentStream->ClockRate / MsPerBlock) * BlockSize;

    painter->setPen(QPen(Qt::GlobalColor::blue, 1));
    uint ShiftXNum = 0;
    for (int i = StartPos; i < EndPos - 1; i++)
    {
        painter->drawLine(ShiftX*ShiftXNum,
                          image->height()/2 - StrPnt[i]/AmpPerBlock * BlockSize,
                          ShiftX*(ShiftXNum+1),
                          image->height()/2 - StrPnt[i+1]/AmpPerBlock * BlockSize);
        ShiftXNum++;
    }
    painter->setPen(currentPen);
}

void Plotter::PaintGrid()
{
    QPen currentPen = painter->pen();
;
    painter->setPen(QPen(Qt::GlobalColor::lightGray, 2, Qt::PenStyle::DashLine));
    //горизонтальные линии
    for (int i = 1; i <= image->width() / BlockSize; i++)
    {
        painter->drawLine(i * BlockSize, image->height(), i * BlockSize, 0);
    }
    //вертикальные линии
    for (int i = 1; i <= image->height() / (BlockSize * 2); i++)
    {
        painter->drawLine(0, image->height()/2 + BlockSize * i,
                          image->width(), image->height()/2 + BlockSize * i);
        painter->drawLine(0, image->height()/2 + BlockSize * -i,
                          image->width(), image->height()/2 + BlockSize * -i);
    }

    painter->setPen(QPen(Qt::GlobalColor::black, 2));
    painter->drawLine(0, image->height()/2, image->width(), image->height()/2);

    painter->setPen(currentPen);
}

void Plotter::resizeEvent(QResizeEvent *)
{
    image = new QImage(this->width(), this->height(), QImage::Format_RGB16);
}

Plotter::~Plotter()
{
    delete ui;
    delete painter;
    delete image;
}

int Plotter::clamp(int n, int lower, int upper) {
  return std::min(upper, std::max(n, lower));
}

void Plotter::rXUp()
{
    CurX++;
    if(CurX > 9)
        CurX = 9;

    this->MsPerBlock = ScanX[CurX];
    emit ScanChanged(ScanX[CurX], ScanY[CurY]);
    update();
}

void Plotter::rXDown()
{
    CurX--;
    if(CurX < 0)
        CurX = 0;

    this->MsPerBlock = ScanX[CurX];
    emit ScanChanged(ScanX[CurX], ScanY[CurY]);
    update();
}

void Plotter::rYUp()
{
    CurY++;
    if(CurY > 13)
        CurY = 13;

    this->AmpPerBlock = ScanY[CurY];
    emit ScanChanged(ScanX[CurX], ScanY[CurY]);
    update();
}

void Plotter::rYDown()
{
    CurY--;
    if(CurY < 0)
        CurY = 0;

    this->AmpPerBlock = ScanY[CurY];
    emit ScanChanged(ScanX[CurX], ScanY[CurY]);
    update();
}

QImage Plotter::GetImage()
{
    return *image;
}

void Plotter::setGrid(bool sw)
{
    isGrid = sw;
    this->update();
}

void Plotter::Update()
{
    this->update();
}
