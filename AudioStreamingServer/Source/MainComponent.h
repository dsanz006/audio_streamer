//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamServer.h"
#include "AudioStreamFifo.h"

//////////////////////////////////////////////////////////////////////////////

#define AUDIO_BUFFER_SIZE_SAMPLES   (1024U*32U) // 46 would be around 1s of audio
#define AUDIO_TO_SERVER_BUFFER_SIZE	(AUDIO_BUFFER_SIZE_SAMPLES*2U) // stereo

//////////////////////////////////////////////////////////////////////////////

class MainComponent   : public AudioAppComponent
{
private:
    AudioStreamFifo *m_fifoAudioToServer;
    StreamServer m_streamServer;
    
public:
    MainComponent(void);
    ~MainComponent(void);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources(void) override;
    
    StreamServer *getStreamServer(void);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//////////////////////////////////////////////////////////////////////////////
