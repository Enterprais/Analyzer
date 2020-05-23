#ifndef EXPORTER_H
#define EXPORTER_H

#include <QWidget>
#include <data.h>

class Exporter : public QWidget
{
public:
    Exporter();
    void ExportImage(QImage img);
    void ExportXls(std::deque<DataStream> data);
    void ExportWav(std::deque<DataStream> data);
};

#endif // EXPORTER_H
