#ifndef DATA_H
#define DATA_H
#include <vector>
#include <QObject>

struct DataStream
{
public:
    float ClockRate;
    std::vector<int> DataArray;

    DataStream(float Rate);
    void AddDataToStream (int Data);
};

class Data: public QObject
{
    Q_OBJECT;
public:
    static Data& getInstance();
    void CreateDataStreams (int Count, float Rate);

private:
    Data();
    Data(const Data&);
    Data& operator= (Data&);
    void ClearStreams();

    std::vector<DataStream> DataStreams;

signals:
    void DataChanged(DataStream*);

public slots:
    DataStream* GetDataStream(int Number);

};


#endif // DATA_H
