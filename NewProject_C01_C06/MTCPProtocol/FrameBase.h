#ifndef __FRAMEBASE__H__
#define __FRAMEBASE__H__

#include "MTCProto.h"
#include "MTcpClient.h"
#include "logHandle.h"

//////////////////////////////////////////////////////////////////////////
// Frame Base
//////////////////////////////////////////////////////////////////////////
class CFrameBase
{
public:
    CFrameBase(void);
    virtual ~CFrameBase(void);

    // connections
    int open(const std::string& address, uint16_t port, int timeout = 10000);
    bool isOpen(void);
    void close(void);

protected:
    int SendFrame(uint32_t ctrl, void* payload, int payload_size, void* data, int datasize);
    // virtual int RecvResponse(void * buffer, int len);
    virtual int RecvResponseEx(void* rspBuffer, int rspLen, std::string& csvString);

    int mp_packet_header(INOUT void* buffer, IN uint32_t MTCP,
                         IN uint32_t CTRL,  //  packet control id (defined below)
                         IN uint32_t PLEN,  //  payload length (if any), CTRL specific
                         IN uint16_t ERRC,  //  valid only for backward packet, tMTCP_ERR code (defined below)
                         IN uint8_t SEQN    //  forward packet sequence number
    );

    virtual int getResponseToJson(char* buffer, int size)
    {
        return 0;
    }

public:
    void DBG_HEX(const char* caption, const char* buffer, size_t size);

private:
    std::shared_ptr<MTcpClient> m_session;
};

#endif  // _FRAME_BASE__
