//////////////////////////////////////////////////////////////////////////////

#include "ServerGui.h"

//////////////////////////////////////////////////////////////////////////////

ServerGui::ServerGui(StreamServer *pStreamServer)
{
    m_pStreamServer = pStreamServer;
    
    setSize (800, 600);
    
    addAndMakeVisible(m_lab_nCurrentConnections);
    m_lab_nCurrentConnections.setColour(Label::backgroundColourId, Colours::black);
    m_lab_nCurrentConnections.setColour(Label::textColourId, Colours::white);
    m_lab_nCurrentConnections.setFont(Font (16.0f, Font::bold));
    m_lab_nCurrentConnections.setJustificationType(Justification::centred);
    m_lab_nCurrentConnections.setEditable(false);
    m_lab_nCurrentConnections.setText(String("hihi"), dontSendNotification);
    
    addAndMakeVisible(m_lab_nMaxSimultaneousConnections);
    m_lab_nMaxSimultaneousConnections.setColour(Label::backgroundColourId, Colours::black);
    m_lab_nMaxSimultaneousConnections.setColour(Label::textColourId, Colours::white);
    m_lab_nMaxSimultaneousConnections.setFont(Font (16.0f, Font::bold));
    m_lab_nMaxSimultaneousConnections.setJustificationType(Justification::centred);
    m_lab_nMaxSimultaneousConnections.setEditable(false);
    m_lab_nMaxSimultaneousConnections.setText(String("hihi"), dontSendNotification);
    
    addAndMakeVisible(m_lab_nTotalConnections);
    m_lab_nTotalConnections.setColour(Label::backgroundColourId, Colours::black);
    m_lab_nTotalConnections.setColour(Label::textColourId, Colours::white);
    m_lab_nTotalConnections.setFont(Font (16.0f, Font::bold));
    m_lab_nTotalConnections.setJustificationType(Justification::centred);
    m_lab_nTotalConnections.setEditable(false);
    m_lab_nTotalConnections.setText(String("hihi"), dontSendNotification);
    
    startTimerHz(2);
}

ServerGui::~ServerGui(void)
{
    stopTimer();
}

//////////////////////////////////////////////////////////////////////////////

void ServerGui::timerCallback()
{
    jassert(m_pStreamServer != nullptr);
    
    String strLabel = String("connected: ") + String(m_pStreamServer->getCurrentConnections());
    m_lab_nCurrentConnections.setText(strLabel, dontSendNotification);
    strLabel = String("maximum: ") + String(m_pStreamServer->getMaxSimultaneousConnections());
    m_lab_nMaxSimultaneousConnections.setText(strLabel, dontSendNotification);
    strLabel = String("total: ") + String(m_pStreamServer->getTotalConnections());
    m_lab_nTotalConnections.setText(strLabel, dontSendNotification);
}

//////////////////////////////////////////////////////////////////////////////

void ServerGui::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ServerGui::resized(void)
{
    m_lab_nCurrentConnections.setBounds(10, 10, getWidth()-20, 30);
    m_lab_nMaxSimultaneousConnections.setBounds(10, 50, getWidth()-20, 30);
    m_lab_nTotalConnections.setBounds(10, 90, getWidth()-20, 30);
}

//////////////////////////////////////////////////////////////////////////////
