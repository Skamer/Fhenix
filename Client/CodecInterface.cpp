#include "CodecInterface.h"

namespace Fhenix
{

CodecInterface::CodecInterface() :
    mQuality(10),
    mChannelCount(2),
    mName(""),
    mInitialized(false)
{

}


bool CodecInterface::initialized() const
{
    return mInitialized;
}

int CodecInterface::channelCount() const
{
    return mChannelCount;
}

CodecMode CodecInterface::mode() const
{
    return mMode;
}

QString CodecInterface::name() const
{
    return mName;
}


unsigned int CodecInterface::quality() const
{
    return mQuality;
}

int CodecInterface::sampleRate() const
{
    return mSampleRate;
}

void CodecInterface::setChannelCount(const int channelCount)
{
    mChannelCount = channelCount;
}

void CodecInterface::setMode(const CodecMode mode)
{
    mMode = mode;
}

void CodecInterface::setQuality(const unsigned int quality)
{
    mQuality = quality;

    if(mQuality > 10)
        mQuality = 10;

    if(mQuality < 0)
        mQuality = 0;
}

void CodecInterface::setSampleRate(const int sampleRate)
{
    mSampleRate = sampleRate;
}

void CodecInterface::setSampleSize(const int sampleSize)
{

    mSampleSize = sampleSize;
}

}


