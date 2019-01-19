//////////////////////////////////////////////////////////////////////////////

#include "MainComponent.h"

//////////////////////////////////////////////////////////////////////////////

static float g_bufferAudioToServer1[AUDIO_TO_SERVER_BUFFER_SIZE];
static float g_bufferAudioToServer2[AUDIO_TO_SERVER_BUFFER_SIZE];
AudioStreamFifo g_fifoAudioToServer1(g_bufferAudioToServer1, sizeof(g_bufferAudioToServer1)/sizeof(float));
AudioStreamFifo g_fifoAudioToServer2(g_bufferAudioToServer2, sizeof(g_bufferAudioToServer2)/sizeof(float));

//not used for GUI, using ServerGui instead
MainComponent m_mainComponent;

//////////////////////////////////////////////////////////////////////////////

MainComponent::MainComponent(void)
{
    //to avoid SIGPIPE when trying to write to a closed socket (the above breakpoint too)
    signal(SIGPIPE, SIG_IGN);
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (14, 12);
    
    //audio fifo: double buffering
    m_fifoAudioToServer = &g_fifoAudioToServer1;
    
    //start server thread
    m_streamServer.startThread(0); //lowest priority
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
    int startSample = bufferToFill.startSample;
    int numSamples  = bufferToFill.numSamples;
    
    const float *iBufL = bufferToFill.buffer->getReadPointer(/*12*/0, startSample);
    const float *iBufR = bufferToFill.buffer->getReadPointer(/*13*/1, startSample);
    
    if (!m_fifoAudioToServer->isEmptyEnough((int64_t)2*numSamples))
    {
        m_streamServer.setReadyBuffer(m_fifoAudioToServer);
        
        if (m_fifoAudioToServer == &g_fifoAudioToServer1)
            m_fifoAudioToServer = &g_fifoAudioToServer2;
        else
            m_fifoAudioToServer = & g_fifoAudioToServer1;
        
        m_fifoAudioToServer->reset();
    }
    if (!m_fifoAudioToServer->write(iBufL, (uint32_t)numSamples))
        jassert(false);
    if (!m_fifoAudioToServer->write(iBufR, (uint32_t)numSamples))
        jassert(false);
    
    /* simple audio in->out example
     
    int startSample = bufferToFill.startSample;
    
    const float *iBufL = bufferToFill.buffer->getReadPointer(0, startSample);
    const float *iBufR = bufferToFill.buffer->getReadPointer(1, startSample);
    
    float *oBufL = bufferToFill.buffer->getWritePointer(0, startSample);
    float *oBufR = bufferToFill.buffer->getWritePointer(1, startSample);
    
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        oBufL[sample] = iBufL[sample];
        oBufR[sample] = iBufR[sample];
    }
     */
    
    //no audio out
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//////////////////////////////////////////////////////////////////////////////

StreamServer *MainComponent::getStreamServer(void)
{
    return &m_streamServer;
}

//////////////////////////////////////////////////////////////////////////////
