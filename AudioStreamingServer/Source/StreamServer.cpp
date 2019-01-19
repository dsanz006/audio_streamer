//////////////////////////////////////////////////////////////////////////////

#include "MainComponent.h"
#include "StreamServer.h"
#include "StreamServerConnection.h"

//////////////////////////////////////////////////////////////////////////////

extern AudioStreamFifo g_fifoAudioToServer;

//////////////////////////////////////////////////////////////////////////////

static void error(const char *msg)
{
    perror(msg);
    jassert(false);
    exit(1);
}

//////////////////////////////////////////////////////////////////////////////

StreamServer::StreamServer(void) : Thread ("Server Thread"), m_threadPool(2)
{
    m_fifoAudioToServer = nullptr;
    
    for (int i=0; i<SOCKETS_LIMIT; i++)
        m_pArraySockets[i] = nullptr;
    
    m_nCurrentConnections = 0;
    m_nMaxSimultaneousConnections = 0;
    m_nTotalConnections = 0;
    
    m_threadPool.setThreadPriorities(1); //2nd lowest priority
    
    DBG("Server thread created!\n");
}

StreamServer::~StreamServer(void)
{
    m_threadPool.removeAllJobs(true, 2000);
    
    signalThreadShouldExit();
    
    printf("goodbye...\n");
    
    for (int i=0; i<SOCKETS_LIMIT; i++)
        jassert(m_pArraySockets[i] == nullptr);
    
    // this makes waitForNextConnection() to exit with no connection, thus allowing thread to exit
    m_serverSocket.close();
    
    stopThread(2000);
}

//////////////////////////////////////////////////////////////////////////////

void StreamServer::setReadyBuffer(AudioStreamFifo *fifoAudioToServer)
{
    m_fifoAudioToServer = fifoAudioToServer;
}

AudioStreamFifo *StreamServer::getReadyBuffer(void)
{
    return m_fifoAudioToServer;
}

void StreamServer::deleteSocket(int nIndex)
{
    jassert(m_pArraySockets[nIndex] != nullptr);
    jassert(!m_pArraySockets[nIndex]->isConnected());
    
    printf("deleting connection %d\n", nIndex);
    
    delete(m_pArraySockets[nIndex]);
    m_pArraySockets[nIndex] = nullptr;
    
    m_nCurrentConnections--;
}

//////////////////////////////////////////////////////////////////////////////

uint32_t StreamServer::getCurrentConnections(void)
{
    return m_nCurrentConnections;
}

uint32_t StreamServer::getMaxSimultaneousConnections(void)
{
    return m_nMaxSimultaneousConnections;
}

uint32_t StreamServer::getTotalConnections(void)
{
    return m_nTotalConnections;
}

//////////////////////////////////////////////////////////////////////////////

void StreamServer::run(void)
{
    if (!m_serverSocket.createListener(5000))
        error("could not listen\n");
    
    printf("binded and listening!\n");
    
    while(!threadShouldExit())
    {
        int nIndex = 0;
        //find free socket
        for (nIndex=0; nIndex<SOCKETS_LIMIT; nIndex++)
        {
            if (m_pArraySockets[nIndex] == nullptr)
                break;
        }
        if (nIndex != SOCKETS_LIMIT)
        {
            m_pArraySockets[nIndex] =m_serverSocket.waitForNextConnection();
            if (m_pArraySockets[nIndex] != nullptr)
            {
                m_nCurrentConnections++;
                m_nTotalConnections++;
                m_nMaxSimultaneousConnections = m_nCurrentConnections > m_nMaxSimultaneousConnections ? m_nCurrentConnections : m_nMaxSimultaneousConnections;
                
                StreamServerConnection* pConnectionJob = new StreamServerConnection(String(m_pArraySockets[nIndex]->getRawSocketHandle()));
                
                pConnectionJob->setParams(m_pArraySockets[nIndex], nIndex, this);
                m_threadPool.addJob(pConnectionJob, true);
                
                printf("created job to handle connection %d (fd=%d)\n", nIndex, m_pArraySockets[nIndex]->getRawSocketHandle());
            }
        }
        
        wait(10);
    }
}

//////////////////////////////////////////////////////////////////////////////
