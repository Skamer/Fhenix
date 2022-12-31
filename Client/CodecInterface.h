#ifndef CODECINTERFACE_H
#define CODECINTERFACE_H

#include <QByteArray>
#include <QString>


namespace Fhenix
{

enum class CodecMode
{
    Music,
    Voice
};

class CodecInterface
{
public:
    CodecInterface();

public:
    virtual QByteArray encode(QByteArray data) = 0;
    virtual QByteArray decode(QByteArray data) = 0;
    virtual bool initialize(int* error = nullptr) = 0;

    bool initialized() const;

    int channelCount() const;
    CodecMode mode() const;
    QString name() const;
    unsigned int quality() const;
    int sampleRate() const;

    void setChannelCount(const int channelCount);

    void setMode(const CodecMode mode);

    void setQuality(const unsigned int quality);

    void setSampleRate(const int sampleRate);

    void setSampleSize(const int sampleSize);
protected:

    int mChannelCount;

    bool mInitialized;

    CodecMode mMode;

    QString mName;

    unsigned int mQuality;

    int mSampleRate;

    int mSampleSize;




};

}

#endif // CODECINTERFACE_H
