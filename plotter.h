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


private:
    Ui::Plotter *ui;
    QPainter *painter;
    QImage *image;

    float MsPerPixel;
    float AmpPerPixel;
    int CurrentNumberStream;

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
