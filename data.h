#ifndef DATA_H
#define DATA_H
#include <vector>
#include <QObject>
#include <deque>

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
    void ClearStreams();   
    std::deque<DataStream> GetData();

private:
    Data();
    Data(const Data&);
    Data& operator= (Data&);

    std::deque<DataStream> DataStreams;

signals:
    void DataChanged();

public slots:
    DataStream* GetDataStream(int Number);

};


#endif // DATA_H
