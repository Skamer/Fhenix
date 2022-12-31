#include "AudioMicrophone.h"
#include <QDebug>

namespace Fhenix
{

    AudioMicrophone::AudioMicrophone() : AudioDevice()
    {
    }

    AudioMicrophone::AudioMicrophone(const QAudioDeviceInfo &device, CodecInterface *codec, QObject *parent) :
        AudioDevice(device, codec, parent)
    {
        mSpeaking = true;
    }

    bool AudioMicrophone::initialize()
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

        mMicrophone = new QAudioInput(mDevice, format, this);
        mMicrophone->setNotifyInterval(40);

        //mIODevice = mMicrophone->start();

        connect(mMicrophone, SIGNAL(notify()), this, SLOT(process()));
        //connect(mIODevice, SIGNAL(readyRead()), this, SLOT(process())


        mInitialized = true;
        return true;
    }

    void AudioMicrophone::process()
    {
        //if(mMuted)
          //  return;

        if(!mCodec)
            return;

        if(!mInitialized)
            return;

        if(!mSpeaking)
            return;


        //qDebug() << "Call Audio Microphone " << NativeWindowsInput::get()->isKeyPressed(Qt::Key_A);
        //bool testA = NativeWindowsInput::get()->isKeyPressed(Qt::Key_A);


            int bytesAvailaible = mMicrophone->bytesReady();
            char* buffer = new char[bytesAvailaible];

            int bytesWritten = mIODevice->read(buffer, bytesAvailaible);

            QByteArray data(buffer, bytesWritten);

            QByteArray encodedData = mCodec->encode(data);


            emit(newVoiceData(encodedData));


    }

    void AudioMicrophone::setSpeaking(bool speaking)
    {
        if(speaking && mMicrophone->state() == QAudio::StoppedState)
        {
            mIODevice = mMicrophone->start();
        }
        else if(!speaking && speaking != mSpeaking)
        {
            mMicrophone->suspend();
        }
        else if(speaking && mMicrophone->state() == QAudio::SuspendedState)
        {
            mMicrophone->resume();
        }

        mSpeaking = speaking;

    }

}
