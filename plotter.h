#ifndef PLOTTER_H
#define PLOTTER_H
#define StrPnt CurrentStream->DataArray

#include <QWidget>
#include "data.h"

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();

public slots:
    void RepaintPlot(DataStream *stream);
    void MoveSlider(int value);
    void rXDown();
    void rXUp();
    void rYDown();
    void rYUp();
    void Update();
    void setGrid(bool sw);
    QImage GetImage();

signals:
    void ScanChanged(int X, int Y);

private:
    Ui::Plotter *ui;
    QPainter *painter;
    QImage *image;

    int BlockSize;
    float MsPerBlock;
    float AmpPerBlock;
    int CurrentNumberStream;

    int ScanX[10] = {5, 10, 25, 50, 100, 250, 500, 1000, 2000, 5000};
    int CurX = 3;
    int ScanY[14] = {1, 5, 10, 25, 50, 100, 250, 500, 1000, 2000, 5000, 10000, 50000, 100000};
    int CurY = 7;

    bool isGrid;
    bool isActive;
    bool UpdateGraph;

    DataStream *CurrentStream;

    void PaintGrid();
    void PaintGraph();
    int clamp(int n, int lower, int upper);

protected:
     void paintEvent(QPaintEvent *) override;
     void resizeEvent(QResizeEvent *event) override;
};

#endif // PLOTTER_H
