#ifndef AUDIOSPEAKERS_H
#define AUDIOSPEAKERS_H

#include <QAudioOutput>

#include "AudioDevice.h"


namespace Fhenix
{

    class AudioSpeakers : public AudioDevice
    {
        Q_OBJECT
    public:
        AudioSpeakers();
        AudioSpeakers(const QAudioDeviceInfo &device, CodecInterface *codec, QObject *parent);

        virtual bool initialize();

   public slots:
        void play(const QByteArray& data);

    private:
        QAudioOutput* mSpeakers;
    };

}

#endif // AUDIOSPEAKERS_H
