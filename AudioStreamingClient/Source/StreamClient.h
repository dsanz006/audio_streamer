#ifndef StreamClient_h
#define StreamClient_h

//////////////////////////////////////////////////////////////////////////////

#define MAX_SOCKET_READ_FLOAT_SIZE 1024*8 // 32 KB

//////////////////////////////////////////////////////////////////////////////

class StreamClient : public Thread
{
private:
    StreamingSocket m_clientSocket;
    
public:
    StreamClient(void);
    ~StreamClient(void);
    
    void run(void) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamClient)
};

//////////////////////////////////////////////////////////////////////////////

#endif /* StreamClient_h */
