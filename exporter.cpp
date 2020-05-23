#include "exporter.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <xlsxdocument.h>
#include <xlsxchart.h>
#include <xlsxchartsheet.h>
#include <xlsxformat.h>
#include <xlsxabstractooxmlfile.h>



Exporter::Exporter()
{

}

void Exporter::ExportImage(QImage img)
{
    QString filename = QFileDialog::getExistingDirectory();
    if(filename == nullptr)
        return;
    filename += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss") + ".jpg";
    if(!img.save(filename, "JPG", 95))
           QMessageBox::critical(this, "Ошибка сохранения", "Ошибка: ошибка во время сохранения файла jpg");
}

void Exporter::ExportXls(std::deque<DataStream> data)
{
    QString filename = QFileDialog::getExistingDirectory();
    if(filename == nullptr)
        return;
    filename += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");

    QXlsx::Document xlsx;

    xlsx.write(1,1,"номер измерения");
    xlsx.write(1,2,"Датчик 1");
    xlsx.write(1,3,"Датчик 2");
    xlsx.write(1,4,"Датчик 3");
    xlsx.write(1,5,"Датчик 4");

    QXlsx::Worksheet *sheet = xlsx.currentWorksheet();
    sheet->setColumnWidth(1,1,20);

    unsigned len = data[0].DataArray.size()-1;

    for (unsigned i = 0; i < len; i++)
        xlsx.write(i+2,1,i+1);
    for (unsigned i = 0; i < len; i++)
        xlsx.write(i+2,2,data[0].DataArray[i]);
    for (unsigned i = 0; i < len; i++)
        xlsx.write(i+2,3,data[1].DataArray[i]);
    for (unsigned i = 0; i < len; i++)
        xlsx.write(i+2,4,data[2].DataArray[i]);
    for (unsigned i = 0; i < len; i++)
        xlsx.write(i+2,5,data[3].DataArray[i]);

    QXlsx::Chart *chart = xlsx.insertChart(3,7, QSize(600, 400));
    chart->addSeries(QXlsx::CellRange(2,2,len+1,5));
    chart->setChartType(QXlsx::Chart::ChartType::CT_Line);

    try {
            xlsx.saveAs(filename + ".xlsx");
    }
    catch (std::exception e) {
        QMessageBox::critical(this, "Ошибка сохранения", "Ошибка: ошибка во время сохранения файла xlsx");
    }
}

void Exporter::ExportWav(std::deque<DataStream> data)
{
    QString filename = QFileDialog::getExistingDirectory();
    if(filename == nullptr)
        return;
    filename += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");

    qint16 chnum = 1;
    qint16 sps = 110;
    quint16 bps = 16;
    quint32 data_len = data[0].DataArray.size();

    quint32 h_chunkId = 0x46464952;
    quint32 h_chunkLen = ((data_len * bps) / 8) + 36;
    quint32 w_Id = 0x45564157;
    quint32 f_Id = 0x20746D66;
    quint32 f_chunkLen = 16;
    quint16 f_tag = 1;
    quint16 f_channel = chnum;
    quint32 f_samplePerSec = sps;
    quint16 f_blockAlign = (chnum * bps) / 8;
    quint32 f_avgBytePerSec = sps * f_blockAlign;
    quint16 f_bitsPerSample = bps;
    quint32 d_id = 0x61746164;
    quint32 d_len = ((data_len * bps) / 8);

    try {
        for (uint j = 0; j < data.size(); j++)
        {
            QFile file;
            file.setFileName(filename + "-CH" + QString::number(j) + ".wav");
            file.open(QFile::OpenModeFlag::WriteOnly);
            file.write((char*)(&h_chunkId), sizeof(h_chunkId));
            file.write((char*)(&h_chunkLen), sizeof(h_chunkLen));
            file.write((char*)(&w_Id), sizeof(w_Id));
            file.write((char*)(&f_Id), sizeof(f_Id));
            file.write((char*)(&f_chunkLen), sizeof(f_chunkLen));
            file.write((char*)(&f_tag), sizeof(f_tag));
            file.write((char*)(&f_channel), sizeof(f_channel));
            file.write((char*)(&f_samplePerSec), sizeof(f_samplePerSec));
            file.write((char*)(&f_avgBytePerSec), sizeof(f_avgBytePerSec));
            file.write((char*)(&f_blockAlign), sizeof(f_blockAlign));
            file.write((char*)(&f_bitsPerSample), sizeof(f_bitsPerSample));
            file.write((char*)(&d_id), sizeof(d_id));
            file.write((char*)(&d_len), sizeof(d_len));

            for(uint i = 0; i < data_len; i++)
            {
                quint16 tmp = data[0].DataArray[i];
                file.write((char*)(&tmp), sizeof(tmp));
            }
            file.close();
        }
    } catch (std::exception e) {
         QMessageBox::critical(this, "Ошибка сохранения", "Ошибка: ошибка во время сохранения файла wav");
    }

}

