#include "plotter.h"
#include "ui_plotter.h"
#include <QPainter>
#include <QDebug>
#include <data.h>
#include <algorithm>


Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);
    image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
    painter = new QPainter();

    CurrentNumberStream = 0;
    MsPerPixel = 1;
    AmpPerPixel = 1;
    isActive = true;
    UpdateGraph = true;

    connect(&Data::getInstance(), &Data::DataChanged, this, &Plotter::RepaintPlot);
    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &Plotter::MoveSlider);

    CurrentStream = Data::getInstance().GetDataStream(0);
    CurrentStream->AddDataToStream(0);
    CurrentStream->AddDataToStream(10);
    CurrentStream->AddDataToStream(40);
    CurrentStream->AddDataToStream(30);
    CurrentStream->AddDataToStream(50);

}

void Plotter::paintEvent(QPaintEvent *)
{
    if(!isActive || CurrentStream == nullptr)
        return;

    painter->begin(image);

    if ((1000 / CurrentStream->ClockRate) / MsPerPixel * CurrentStream->DataArray.size() <= image->width())
    {
        ui->horizontalScrollBar->hide();
    }
    else
    {
        ui->horizontalScrollBar->show();
        ui->horizontalScrollBar->setMaximum(CurrentStream->DataArray.size());
        if(UpdateGraph)
            ui->horizontalScrollBar->setSliderPosition(CurrentStream->DataArray.size());
    }


    painter->fillRect(0,0,width(),height(),QColor(255,255,255));
    PaintGrid();
    PaintGraph();

    painter->end();

    painter->begin(this);
    painter->drawImage(0,0,*image);
    painter->end();
}

void Plotter::MoveSlider(int value)
{
    if(value == (int)CurrentStream->DataArray.size())
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

    int DotsOnScreen = image->width() * MsPerPixel / (1000 / CurrentStream->ClockRate) + 1;

    int StartPos = 0;
    int EndPos = 0;

    if(UpdateGraph)
    {
        EndPos = CurrentStream->DataArray.size();
        StartPos = clamp(EndPos-DotsOnScreen, 0, CurrentStream->DataArray.size());
    }
    else
    {
        if(DotsOnScreen > ui->horizontalScrollBar->sliderPosition())
            EndPos = DotsOnScreen;
        else
            EndPos = ui->horizontalScrollBar->sliderPosition();

        StartPos = clamp(EndPos-DotsOnScreen, 0, CurrentStream->DataArray.size());
    }

    QPen currentPen = painter->pen();
    float ShiftX = (1000 / CurrentStream->ClockRate) / MsPerPixel;

    uint ShiftXNum = 0;
    painter->setPen(QPen(Qt::GlobalColor::blue, 2));
    for (int i = StartPos; i < EndPos - 1; i++)
    {
        painter->drawLine(ShiftX*ShiftXNum,
                          image->height()/2 - StrPnt[i]/AmpPerPixel,
                          ShiftX*(ShiftXNum+1),
                          image->height()/2 - StrPnt[i+1]/AmpPerPixel);
        ShiftXNum++;
    }
    painter->setPen(currentPen);
}

void Plotter::PaintGrid()
{
    QPen currentPen = painter->pen();
    painter->setPen(QPen(Qt::GlobalColor::lightGray, 2));
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
    this->MsPerPixel+=1;
    update();
}

void Plotter::rXDown()
{
    this->MsPerPixel-=1;
    update();
}

void Plotter::rYUp()
{
    this->AmpPerPixel+=0.1f;
    update();
}

void Plotter::rYDown()
{
    this->AmpPerPixel-=0.1f;
    update();
}
