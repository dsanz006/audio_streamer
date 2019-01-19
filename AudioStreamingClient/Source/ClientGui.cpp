//////////////////////////////////////////////////////////////////////////////

#include "ClientGui.h"
#include "MainComponent.h"

//////////////////////////////////////////////////////////////////////////////

extern AudioStreamFifo g_fifoClientToAudio;
extern MainComponent m_mainComponent;

//////////////////////////////////////////////////////////////////////////////

ClientGui::ClientGui(StreamClient *pStreamClient)
{
    m_pStreamClient = pStreamClient;
    
    m_flagPaused = false;
    m_flagStarted = false;
    m_nPrintCounter = 0;
    
    setSize(800, 600);
    
    addAndMakeVisible(m_but_play);
    m_but_play.setButtonText("pause");
    m_but_play.addListener(this);
    
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
    m_but_play.removeListener(this);
}

//////////////////////////////////////////////////////////////////////////////

void ClientGui::timerCallback()
{
    jassert(m_pStreamClient != nullptr);
    
    if (!m_flagStarted)
    {
        if(g_fifoClientToAudio.getCount() >= AUDIO_START_BUFFER_SIZE)
            m_flagStarted = true;
        else
            return;
    }
    
    if (m_flagPaused)
    {
        m_but_play.setButtonText("play");
        
        if (m_nPrintCounter == 0)
            m_lab_connectionStatus.setText(String("paused"), dontSendNotification);
        else if (m_nPrintCounter == 6)
            m_lab_connectionStatus.setText(String("..."), dontSendNotification);
        
        m_nPrintCounter++;
        
        if (m_nPrintCounter == 12)
            m_nPrintCounter = 0;
    }
    else
    {
        m_but_play.setButtonText("pause");
        
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
}

void ClientGui::buttonClicked(Button* button)
{
    if (button == &m_but_play)
    {
        m_flagPaused = !m_flagPaused;
        m_mainComponent.setPaused(m_flagPaused);
        
        m_nPrintCounter = 0;
    }
}

//////////////////////////////////////////////////////////////////////////////

void ClientGui::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ClientGui::resized(void)
{
    m_lab_connectionStatus.setBounds(10, 10, getWidth()-20, 30);
    m_but_play.setBounds(10, 50, getWidth()-20, 60);
}

//////////////////////////////////////////////////////////////////////////////
