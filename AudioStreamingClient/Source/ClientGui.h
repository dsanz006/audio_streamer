#ifndef ClientGui_h
#define ClientGui_h

//////////////////////////////////////////////////////////////////////////////

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamClient.h"

//////////////////////////////////////////////////////////////////////////////

class ClientGui : public Component, public Timer, public Button::Listener
{
private:
    StreamClient *m_pStreamClient;
    
    bool m_flagStarted;
    bool m_flagPaused;
    uint32_t m_nPrintCounter;
    
    Label m_lab_connectionStatus;
    TextButton m_but_play;
    
public:
    ClientGui(StreamClient *pStreamClient);
    ~ClientGui(void);
    
    void timerCallback(void) override;
    void buttonClicked(Button* button) override;
    
    void paint(Graphics& g) override;
    void resized(void) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClientGui)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* ClientGui_h */
