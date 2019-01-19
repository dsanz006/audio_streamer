//////////////////////////////////////////////////////////////////////////////

#include "ClientGui.h"
#include "MainComponent.h"

//////////////////////////////////////////////////////////////////////////////

extern AudioStreamFifo g_fifoClientToAudio;

//////////////////////////////////////////////////////////////////////////////

ClientGui::ClientGui(StreamClient *pStreamClient)
{
    m_pStreamClient = pStreamClient;
    
    m_flagIsPlaying = false;
    m_nPrintCounter = 0;
    
    setSize (800, 600);
    
    addAndMakeVisible(m_lab_connectionStatus);
    m_lab_connectionStatus.setColour(Label::backgroundColourId, Colours::black);
    m_lab_connectionStatus.setColour(Label::textColourId, Colours::white);
    m_lab_connectionStatus.setFont(Font (16.0f, Font::bold));
    m_lab_connectionStatus.setJustificationType(Justification::centred);
    m_lab_connectionStatus.setEditable(false);
    m_lab_connectionStatus.setText(String("loading..."), dontSendNotification);
    
    startTimerHz(2);
}

ClientGui::~ClientGui(void)
{
    stopTimer();
}

//////////////////////////////////////////////////////////////////////////////

void ClientGui::timerCallback()
{
    jassert(m_pStreamClient != nullptr);
    
    if (!m_flagIsPlaying)
    {
        if(g_fifoClientToAudio.getCount() >= AUDIO_START_BUFFER_SIZE)
            m_flagIsPlaying = true;
        else
            return;
    }
    
    if (m_nPrintCounter == 0)
        m_lab_connectionStatus.setText(String("audio"), dontSendNotification);
    else if (m_nPrintCounter == 6)
        m_lab_connectionStatus.setText(String("is"), dontSendNotification);
    else if (m_nPrintCounter == 12)
        m_lab_connectionStatus.setText(String("coming"), dontSendNotification);
    else if (m_nPrintCounter == 18)
        m_lab_connectionStatus.setText(String("..."), dontSendNotification);
    
    m_nPrintCounter++;
    
    if (m_nPrintCounter == 24)
        m_nPrintCounter = 0;
}

//////////////////////////////////////////////////////////////////////////////

void ClientGui::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ClientGui::resized(void)
{
    m_lab_connectionStatus.setBounds(10, 10, getWidth()-20, 30);
}

//////////////////////////////////////////////////////////////////////////////
