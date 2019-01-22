//////////////////////////////////////////////////////////////////////////////

#include "MainComponent.h"
#include "StreamClient.h"

//////////////////////////////////////////////////////////////////////////////

static uint8_t g_bufferClientRecv[MAX_SOCKET_READ_FLOAT_SIZE*sizeof(float)];
extern AudioStreamFifo g_fifoClientToAudio;

//////////////////////////////////////////////////////////////////////////////

static void error(const char *msg)
{
    perror(msg);
    jassert(false);
    exit(1);
}

//////////////////////////////////////////////////////////////////////////////

StreamClient::StreamClient(void) : Thread ("Client Thread")
{
    printf("Client thread created!\n");
}

StreamClient::~StreamClient(void)
{
    signalThreadShouldExit();
    
    printf("goodbye...\n");
    
    m_clientSocket.close();
    
    stopThread(2000);
}

//////////////////////////////////////////////////////////////////////////////

void StreamClient::run(void)
{
    if (!m_clientSocket.connect(String("localhost"), 5000))
        error("could not connect socket to remote\n");
    
    jassert(m_clientSocket.isConnected());
    
    while (!threadShouldExit())
    {
        int nReadAmount = m_clientSocket.read((void *)g_bufferClientRecv, sizeof(g_bufferClientRecv), false); //non-blocking for now
        
        if (nReadAmount > 0)
        {
            if (!g_fifoClientToAudio.write((const float *)g_bufferClientRecv, (uint32_t)(nReadAmount/sizeof(float))))
                jassert(false);
        }
        else if (nReadAmount < 0)
            error("read error\n");
        
        wait(3);
    }
}

//////////////////////////////////////////////////////////////////////////////
