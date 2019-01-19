#ifndef StreamServer_h
#define StreamServer_h

//////////////////////////////////////////////////////////////////////////////

#include "AudioStreamFifo.h"

//////////////////////////////////////////////////////////////////////////////

#define SERVER_MAX_CONNECTIONS      10
#define MAX_SOCKET_WRITE_FLOAT_SIZE 512 // 2048 bytes

#define SOCKETS_LIMIT               10

//////////////////////////////////////////////////////////////////////////////

class StreamServer : public Thread
{
private:
    AudioStreamFifo *m_fifoAudioToServer;
    
    StreamingSocket m_serverSocket;
    StreamingSocket *m_pArraySockets[SOCKETS_LIMIT];
    
    uint32_t m_nCurrentConnections;
    uint32_t m_nMaxSimultaneousConnections;
    uint32_t m_nTotalConnections;
    
    ThreadPool m_threadPool;
    
public:
    StreamServer(void);
    ~StreamServer(void);
    
    void run(void) override;
    
    void setReadyBuffer(AudioStreamFifo *fifoAudioToServer);
    AudioStreamFifo *getReadyBuffer(void);
    void deleteSocket(int nIndex);
    
    uint32_t getCurrentConnections(void);
    uint32_t getMaxSimultaneousConnections(void);
    uint32_t getTotalConnections(void);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamServer)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* StreamServer_h */
