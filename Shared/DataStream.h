#ifndef FHENIX_DATASTREAM_H
#define FHENIX_DATASTREAM_H

#include <QDataStream>

namespace Fhenix
{
    namespace Shared
    {
        class DataStream : public QDataStream
        {
        public:
            DataStream()
            {
                setVersion(QDataStream::Qt_5_3);
            }

            DataStream(const QByteArray& array) : QDataStream(array)
            {
                setVersion(QDataStream::Qt_5_3);
            }
            DataStream(QByteArray* array, QIODevice::OpenMode mode) : QDataStream(array, mode)
            {
                setVersion(QDataStream::Qt_5_3);
            }

            DataStream(QIODevice* device) : QDataStream(device)
            {
                setVersion(QDataStream::Qt_5_3);
            }


            void pack()
            {
                return;
            }

        template<typename T, typename ...Params>
            void pack(const T &item, Params&&... params)
            {
                *(this) << item;
                //pack(std::forwar)
                pack(params...);
            }

        signals:

        public slots:

        };
    }

}

#endif // DATASTREAM_H
