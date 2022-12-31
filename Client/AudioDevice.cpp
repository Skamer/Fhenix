#include "AudioDevice.h"


namespace Fhenix
{

    AudioDevice::AudioDevice(QObject *parent) :
        QObject(parent)
    {
    }

    AudioDevice::AudioDevice(const QAudioDeviceInfo &device, CodecInterface *codec, QObject *parent) :
        QObject(parent),
        mMuted(false),
        mInitialized(false),
        mIODevice(nullptr)
    {
        mDevice = device;
        mCodec = codec;
    }


    bool AudioDevice::initialized() const
    {
        return mInitialized;
    }

    void AudioDevice::mute()
    {
        mMuted = true;
    }

    bool AudioDevice::muted() const
    {
        return mMuted;
    }

    void AudioDevice::setCodec(CodecInterface *codec)
    {
        mCodec = codec;
    }

    void AudioDevice::setDevice(const QAudioDeviceInfo &device)
    {
        mDevice = device;
    }



    void AudioDevice::unmute()
    {
        mMuted = false;
    }

}
