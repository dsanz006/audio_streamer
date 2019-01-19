#ifndef ClientGui_h
#define ClientGui_h

//////////////////////////////////////////////////////////////////////////////

#include "../JuceLibraryCode/JuceHeader.h"

#include "StreamClient.h"

//////////////////////////////////////////////////////////////////////////////

class ClientGui : public Component, public Timer
{
private:
    StreamClient *m_pStreamClient;
    
    bool m_flagIsPlaying;
    uint32_t m_nPrintCounter;
    Label m_lab_connectionStatus;
    
public:
    ClientGui(StreamClient *pStreamClient);
    ~ClientGui(void);
    
    void timerCallback(void) override;
    
    void paint(Graphics& g) override;
    void resized(void) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClientGui)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* ClientGui_h */
