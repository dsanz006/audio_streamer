#ifndef AudioStreamFifo_h
#define AudioStreamFifo_h

//////////////////////////////////////////////////////////////////////////////
// NOTE: Max Size is 4 GB
//////////////////////////////////////////////////////////////////////////////

class AudioStreamFifo
{
private:
    float *m_pBuffer;
    uint32_t m_nBufferSize;
    volatile uint32_t	m_nReadIndex;
    volatile uint32_t	m_nWriteIndex;
    
public:
    AudioStreamFifo(float *pBuffer, uint32_t nBufferSize) : m_pBuffer(pBuffer), m_nBufferSize(nBufferSize)
    {
        jassert(m_pBuffer);
        jassert(m_nBufferSize > 0);
        reset();
    }
    
    inline void reset(void)
    {
        m_nReadIndex = 0;
        m_nWriteIndex = 0;
    }
    
    inline uint32_t getTotalSize(void) const
    {
        return m_nBufferSize;
    }
    
    inline uint32_t getCount(void) const
    {
        int64_t nCount;
        
        nCount = (int64_t) m_nWriteIndex - (int64_t) m_nReadIndex;
        if (nCount < 0)
            nCount += (int64_t)m_nBufferSize;
        
        return (uint32_t) nCount;
    }
    
    inline bool isEmpty(void) const
    {
        return (m_nWriteIndex == m_nReadIndex);
    }
    
    inline bool isFull(void) const
    {
        const int64_t nFree = (int64_t) m_nBufferSize - (int64_t) getCount() - 1;
        return (nFree <= 0);
    }
    
    inline bool isEmptyEnough(int64_t nDataSize) const
    {
        return (nDataSize <= ((int64_t) m_nBufferSize - (int64_t) getCount() - 1));
    }
    
    inline float *getRawReadPointer(void)
    {
        return &m_pBuffer[m_nReadIndex];
    }
    
    bool write(const float *pData, uint32_t nDataSize)
    {
        jassert(pData);
        jassert(nDataSize > 0);
        
        if (!isEmptyEnough(nDataSize))
            return false;
        
        float *pBuffer = m_pBuffer;
        const uint32_t nBufferSize = m_nBufferSize;
        uint32_t nWriteIndex = m_nWriteIndex;
        
        while (nDataSize--)
        {
            pBuffer[nWriteIndex++] = *pData++;
            if (nWriteIndex == nBufferSize)
                nWriteIndex = 0;
        }
        
        m_nWriteIndex = nWriteIndex;
        return true;
    }
    
    bool read(float *pData, uint32_t nDataSize, bool flagUpdate = true)
    {
        jassert(pData);
        jassert(nDataSize > 0);
        
        if (nDataSize > getCount())
            return false;
        
        float *pBuffer = m_pBuffer;
        const uint32_t nBufferSize = m_nBufferSize;
        uint32_t nReadIndex = m_nReadIndex;
        
        while (nDataSize--)
        {
            *pData++ = pBuffer[nReadIndex++];
            if (nReadIndex == nBufferSize)
                nReadIndex = 0;
        }
        
        if (flagUpdate)
            m_nReadIndex = nReadIndex;
        
        return true;
    }
    
    inline bool write1(float nData)
    {
        const uint32_t nWriteIndex = m_nWriteIndex;
        uint32_t nWriteIndexNext = nWriteIndex + 1;
        if (nWriteIndexNext == m_nBufferSize)
            nWriteIndexNext = 0;
        
        if (nWriteIndexNext == m_nReadIndex)
            return false;
        
        m_pBuffer[nWriteIndex] = nData;
        m_nWriteIndex = nWriteIndexNext;
        return true;
    }
    
    float read1(void)
    {
        uint32_t nReadIndex = m_nReadIndex;
        const float nData = m_pBuffer[nReadIndex++];
        if (nReadIndex == m_nBufferSize)
            m_nReadIndex = 0;
        else
            m_nReadIndex = nReadIndex;
        
        return nData;
    }
};

//////////////////////////////////////////////////////////////////////////////

#endif /* AudioStreamFifo_h */
