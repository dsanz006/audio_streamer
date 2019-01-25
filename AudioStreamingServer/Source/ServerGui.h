#ifndef ServerGui_h
#define ServerGui_h

//////////////////////////////////////////////////////////////////////////////

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamServer.h"

//////////////////////////////////////////////////////////////////////////////

class ServerGui : public Component, public Timer, public ComboBox::Listener
{
private:
    StreamServer *m_pStreamServer;
    
    Label m_lab_nCurrentConnections;
    Label m_lab_nMaxSimultaneousConnections;
    Label m_lab_nTotalConnections;
    ComboBox m_cbx_nChannels;
    
public:
    ServerGui(StreamServer *pStreamServer);
    ~ServerGui(void);
    
    void timerCallback(void) override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    
    void paint(Graphics& g) override;
    void resized(void) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ServerGui)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* ServerGui_h */
