#ifndef StreamServerConnection_h
#define StreamServerConnection_h

//////////////////////////////////////////////////////////////////////////////

#include "MainComponent.h"

//////////////////////////////////////////////////////////////////////////////

class StreamServerConnection : public ThreadPoolJob
{
private:
    String m_strJobName;
    
    StreamingSocket *m_pSocket;
    int m_nConnectionIndex;
    StreamServer *m_pStreamServer;
    AudioStreamFifo *m_pStreamFifo;
    
    
    float *m_pFifoReadPtr;
    uint32_t m_nFifoAmountFloatsRemaining;
    
public:
    StreamServerConnection(const String& strJobName);
    ~StreamServerConnection(void);
    
    void setParams(StreamingSocket *pSocket, int nConnectionIndex, StreamServer *pStreamServer);
    
    JobStatus runJob(void) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamServerConnection)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* StreamServerConnection_h */
