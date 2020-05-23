#include "data.h"


void DataStream::AddDataToStream(int Data)
{
    DataArray.push_back(Data);
    emit Data::getInstance().DataChanged();
}

DataStream::DataStream(float Rate)
{
    ClockRate = Rate;
}

Data::Data()
{
    CreateDataStreams(4,25);
}

DataStream* Data::GetDataStream(int Number)
{
    if(static_cast<int>(DataStreams.size()) < Number)
        return nullptr;
    else return &DataStreams[Number];
}

void Data::ClearStreams()
{
    for(unsigned long i = 0; i < DataStreams.size(); i++)
    {
        DataStreams[i].DataArray.clear();
    }

}

void Data::CreateDataStreams(int Count, float Rate)
{
    for(int i = 0; i < Count; i++)
    {
        DataStreams.push_back(DataStream(Rate));
    }
}

Data& Data::getInstance()
{
     static Data instance;
     return instance;
}

std::deque<DataStream> Data::GetData()
{
    return DataStreams;
}



