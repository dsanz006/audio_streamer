//////////////////////////////////////////////////////////////////////////////

#include "MainComponent.h"

//////////////////////////////////////////////////////////////////////////////

static float g_bufferClientToAudio[CLIENT_TO_AUDIO_BUFFER_SIZE];
AudioStreamFifo g_fifoClientToAudio(g_bufferClientToAudio, sizeof(g_bufferClientToAudio)/sizeof(float));

//not used for GUI, using ServerGui instead
MainComponent m_mainComponent;

//////////////////////////////////////////////////////////////////////////////

MainComponent::MainComponent()
{
    m_flagPaused = false;
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
    
    //start client thread
    m_streamClient.startThread(0); //lowest priority
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//////////////////////////////////////////////////////////////////////////////

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    AudioIODevice* device = deviceManager.getCurrentAudioDevice();
    String deviceName = device->getName();
    BigInteger activeInputChannels  = device->getActiveInputChannels();
    BigInteger activeOutputChannels = device->getActiveOutputChannels();
    
    auto maxInputChannels  = activeInputChannels .getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;
    
    printf("Device: %s, (ins: %d, outs: %d)\n", deviceName.toRawUTF8(), maxInputChannels, maxOutputChannels);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (g_fifoClientToAudio.getCount() >= AUDIO_START_BUFFER_SIZE)
    {
        int startSample = bufferToFill.startSample;
        int numSamples  = bufferToFill.numSamples;
    
        float *oBufL = bufferToFill.buffer->getWritePointer(0, startSample);
        float *oBufR = bufferToFill.buffer->getWritePointer(1, startSample);

        if (!g_fifoClientToAudio.read(oBufL, (uint32_t)numSamples))
            jassert(false);
        if (!g_fifoClientToAudio.read(oBufR, (uint32_t)numSamples))
            jassert(false);
        
        if (m_flagPaused)
            bufferToFill.clearActiveBufferRegion();
            
    }
    else
        bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//////////////////////////////////////////////////////////////////////////////

StreamClient *MainComponent::getStreamClient(void)
{
    return &m_streamClient;
}

void MainComponent::setPaused(bool flagPaused)
{
    m_flagPaused = flagPaused;
}

//////////////////////////////////////////////////////////////////////////////
