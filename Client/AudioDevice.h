#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QObject>
#include "CodecInterface.h"

#include <QAudioDeviceInfo>
#include <QIODevice>


namespace Fhenix
{
    class AudioDevice : public QObject
    {
        Q_OBJECT
    public:
        explicit AudioDevice(QObject *parent = 0);

        explicit AudioDevice(const QAudioDeviceInfo& device, CodecInterface* codec, QObject *parent = 0);

        virtual bool initialize() = 0;

        bool initialized() const;

        void mute();

        bool muted() const;

        void setCodec(CodecInterface* codec);

        void setDevice(const QAudioDeviceInfo& device);

        void unmute();



    signals:

    public slots:

    protected:
        CodecInterface* mCodec;

        QAudioDeviceInfo mDevice;

        bool mInitialized;

        QIODevice* mIODevice;

        bool mMuted;



    };

}

#endif // AUDIODEVICE_H
