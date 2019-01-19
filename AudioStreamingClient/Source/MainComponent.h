//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamClient.h"
#include "AudioStreamFifo.h"

//////////////////////////////////////////////////////////////////////////////

#define AUDIO_BUFFER_SIZE_SAMPLES   (1024U*32U*8U) //8 times that of server
#define CLIENT_TO_AUDIO_BUFFER_SIZE	(AUDIO_BUFFER_SIZE_SAMPLES*2U) //stereo

#define AUDIO_START_BUFFER_SIZE_SAMPLES (1024U*32U*4U) //4 times that of server
#define AUDIO_START_BUFFER_SIZE         (AUDIO_START_BUFFER_SIZE_SAMPLES*2U) //stereo

//////////////////////////////////////////////////////////////////////////////

class MainComponent   : public AudioAppComponent
{
private:
    StreamClient m_streamClient;
public:
    MainComponent(void);
    ~MainComponent(void);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources(void) override;
    
    StreamClient *getStreamClient(void);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//////////////////////////////////////////////////////////////////////////////
