//////////////////////////////////////////////////////////////////////////////

#include "StreamServerConnection.h"

//////////////////////////////////////////////////////////////////////////////

StreamServerConnection::StreamServerConnection(const String& strJobName) : ThreadPoolJob(strJobName)
{
    m_strJobName = String(strJobName);
    
    m_pSocket = nullptr;
    m_nConnectionIndex = -1;
    m_pStreamServer = nullptr;
    m_pStreamFifo = nullptr;
    
    m_pFifoReadPtr = nullptr;
    m_nFifoAmountFloatsRemaining = 0;
}

StreamServerConnection::~StreamServerConnection(void)
{
    jassert(m_pSocket == nullptr);
    
    printf("job %d is being removed...\n", m_nConnectionIndex);
}

//////////////////////////////////////////////////////////////////////////////

void StreamServerConnection::setParams(juce::StreamingSocket *pSocket, int nConnectionIndex, StreamServer *pStreamServer)
{
    jassert(pSocket != nullptr);
    jassert(pStreamServer != nullptr);
    
    m_pSocket = pSocket;
    m_nConnectionIndex = nConnectionIndex;
    m_pStreamServer = pStreamServer;
    m_pStreamFifo = m_pStreamServer->getReadyBuffer();
    jassert(m_pStreamFifo != nullptr);
    
    printf("job ready for connection %d (fd=%d)\n", m_nConnectionIndex, m_pSocket->getRawSocketHandle());
}

//////////////////////////////////////////////////////////////////////////////

StreamServerConnection::JobStatus StreamServerConnection::runJob(void)
{
    if (m_pStreamFifo != m_pStreamServer->getReadyBuffer())
    {
        jassert(m_nFifoAmountFloatsRemaining == 0);
        
        m_pStreamFifo = m_pStreamServer->getReadyBuffer();
        
        m_pFifoReadPtr = m_pStreamFifo->getRawReadPointer();
        m_nFifoAmountFloatsRemaining = m_pStreamFifo->getCount();
    }
    else if (m_nFifoAmountFloatsRemaining > 0)
    {
        uint32_t nSocketWriteFloatSize = m_nFifoAmountFloatsRemaining > MAX_SOCKET_WRITE_FLOAT_SIZE ? MAX_SOCKET_WRITE_FLOAT_SIZE : m_nFifoAmountFloatsRemaining;
        
        if (m_pSocket->write((void *)m_pFifoReadPtr, nSocketWriteFloatSize*sizeof(float)) < 0)
            goto exit_job;
        
        m_pFifoReadPtr += nSocketWriteFloatSize;
        m_nFifoAmountFloatsRemaining -= nSocketWriteFloatSize;

    }
    else
        Thread::sleep(1);
    
    if (shouldExit())
        goto exit_job;
    else
        return jobNeedsRunningAgain;
    
exit_job:
    printf("thread: finished job for connection %d (fd=%d)\n", m_nConnectionIndex, m_pSocket->getRawSocketHandle());
    m_pSocket->close();
    m_pSocket = nullptr;
    m_pStreamServer->deleteSocket(m_nConnectionIndex);
    
    return jobHasFinished;
}

//////////////////////////////////////////////////////////////////////////////s
