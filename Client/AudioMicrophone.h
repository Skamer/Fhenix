#ifndef AUDIOMICROPHONE_H
#define AUDIOMICROPHONE_H

#include <QAudioInput>
#include <QThread>

#include "AudioDevice.h"
#include <atomic>

#include "NativeWindowsInput.h"



namespace Fhenix
{

    class AudioMicrophone : public AudioDevice
    {
        Q_OBJECT
    public:
        AudioMicrophone();
        AudioMicrophone(const QAudioDeviceInfo &device, CodecInterface *codec, QObject *parent);

        virtual bool initialize();

        void setSpeaking(bool speaking);

    signals:
        void newVoiceData(const QByteArray& data);

    private slots:
        void process();

    private:

        QAudioInput* mMicrophone;

        bool mSpeaking;
    };

}

#endif // AUDIOMICROPHONE_H
