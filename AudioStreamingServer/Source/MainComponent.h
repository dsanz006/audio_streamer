//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamServer.h"
#include "AudioStreamFifo.h"

//////////////////////////////////////////////////////////////////////////////

#define AUDIO_BUFFER_SIZE_SAMPLES   (1024U*32U) // 46 would be around 1s of audio
#define AUDIO_TO_SERVER_BUFFER_SIZE	(AUDIO_BUFFER_SIZE_SAMPLES*2U) // stereo

//////////////////////////////////////////////////////////////////////////////

class MainComponent : public AudioAppComponent
{
private:
    AudioStreamFifo *m_pFifoAudioToServer;
    StreamServer m_streamServer;
    
    uint8_t m_nLeftChannel;
    uint8_t m_nChannelCount;
    
public:
    MainComponent(void);
    ~MainComponent(void);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources(void) override;
    
    void setLeftChannel(uint8_t nLeftChannel);
    
    StreamServer *getStreamServer(void);
    uint8_t getLeftChanel(void);
    uint8_t getChannelCount(void);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//////////////////////////////////////////////////////////////////////////////
