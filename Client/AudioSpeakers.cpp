#include "AudioSpeakers.h"
#include <QDebug>


namespace Fhenix
{

AudioSpeakers::AudioSpeakers() : AudioDevice()
{
}

AudioSpeakers::AudioSpeakers(const QAudioDeviceInfo &device, CodecInterface *codec, QObject *parent) :
    AudioDevice(device, codec, parent)
{

}

bool AudioSpeakers::initialize()
{
    if(!mCodec)
        return false;

    QAudioFormat format;
    format.setSampleRate(mCodec->sampleRate());
    format.setChannelCount(mCodec->channelCount());
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    if(!mDevice.isFormatSupported(format))
    {
        qDebug() << "[Microphone] Format not supported, trying to use the nearest.";
        format = mDevice.nearestFormat(format);
    }

    mSpeakers = new QAudioOutput(mDevice, format, this);

    mIODevice = mSpeakers->start();

    mInitialized = true;
    return true;

}

void AudioSpeakers::play(const QByteArray &data)
{
    if(mMuted)
        return;

    if(!mCodec)
        return;

    if(!mInitialized)
        return;

    QByteArray decodedData = mCodec->decode(data);

    int bytesWritten = mIODevice->write(decodedData.data(), decodedData.size());
}


}
