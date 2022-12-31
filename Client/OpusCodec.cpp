#include "OpusCodec.h"
#include <QDebug>

namespace Fhenix
{

OpusCodec::OpusCodec() : CodecInterface()
{
    mName = "opus";
}

bool OpusCodec::initialize(int *error)
{
    int encoderError = 0;
    int decoderError = 0;

    mEnc = opus_encoder_create(mSampleRate, mChannelCount, OPUS_APPLICATION_VOIP, &encoderError);
    mDec = opus_decoder_create(mSampleRate, mChannelCount, &decoderError);

    qDebug() << QString("Encoder error : %1 / Decoded error %2").arg(encoderError).arg(decoderError);

    mInitialized = true;

    return true;

}


QByteArray OpusCodec::encode(QByteArray data)
{
    unsigned char* outputData = new unsigned char[data.size()];
    int bytesPerFrame = (16 * mChannelCount) / 8;

    //opus_int16* inputOpus = (opus_int16*)voiceData;

    int lenEncoded = opus_encode(mEnc, (opus_int16*)data.data(), 480 * 4, outputData,  data.size());

    QByteArray output((char*)outputData, lenEncoded);

    return output;
}

QByteArray OpusCodec::decode(QByteArray data)
{
    int i = data.size();
    opus_int16* outputData = new opus_int16[(480 * 4) * 2 * sizeof(opus_int16)];

    // lenEncoded -> no
    // len no encoded ?

    int frameSize = opus_decode(mDec, (unsigned char*)data.data(), data.size(), outputData, 480 * 4, 0);

    //char* finalvoice = (char*)outputOpus;
    QByteArray output((char*)outputData, frameSize * 4);

    return output;
}

}

