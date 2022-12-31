#ifndef OPUSCODEC_H
#define OPUSCODEC_H

#include "CodecInterface.h"
#include <opus.h>


namespace Fhenix
{

class OpusCodec : public CodecInterface
{
public:
    OpusCodec();
    virtual  QByteArray encode(QByteArray data);
    virtual  QByteArray decode(QByteArray data);
    virtual  bool initialize(int *error = nullptr);

private:
    // Opus encoder;
    OpusEncoder* mEnc;

    // Opus decoder;
    OpusDecoder* mDec;

};

}

#endif // OPUSCODEC_H
