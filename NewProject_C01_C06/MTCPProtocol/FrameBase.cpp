
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>

#include "FrameBase.h"
#include "MTCP_Rosaline.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

#define ASSERT assert

const std::size_t MP_HEAD_SIZE = sizeof(tMTCP_header);
const std::size_t MP_PALOAD_CHECKSUM_SIZE = 1;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CFrameBase::CFrameBase(void) : m_session(std::shared_ptr<MTcpClient>(new MTcpClient)) {}

int CFrameBase::open(const std::string& address, uint16_t port, int timeout)
{
    return m_session->Connect(address.c_str(), port, timeout);
}

bool CFrameBase::isOpen(void)
{
    return m_session->isOpened();
}

void CFrameBase::close(void)
{
    m_session->Close();
}

CFrameBase::~CFrameBase()
{
    cLogHandle::printer(true, "Delete CFrameBase object\n");
}

int CFrameBase::mp_packet_header(INOUT void* buffer, IN uint32_t MTCP, IN uint32_t CTRL, IN uint32_t PLEN,
                                 IN uint16_t ERRC, IN uint8_t SEQN)
{
    ASSERT(buffer);

    tMTCP_header* ptMTCP_header = (tMTCP_header*)(buffer);

    ptMTCP_header->MTCP = (MTCP_PREAMBLE);

    // ptMTCP_header->ERRC = htons(ERRC);
    // ptMTCP_header->CTRL = htonl(CTRL);
    // ptMTCP_header->PLEN = htonl(PLEN);

    ptMTCP_header->MTCP = (MTCP);
    ptMTCP_header->ERRC = (ERRC);
    ptMTCP_header->CTRL = (CTRL);

    if (PLEN > 0) {
        ptMTCP_header->PLEN = (PLEN + 1);
    } else {
        ptMTCP_header->PLEN = (PLEN);
    }

    // ptMTCP_header->PLEN = 0x2000002;
    ptMTCP_header->SEQN = SEQN;

    ptMTCP_header->H_CS = _MTCP_calculateCheckSum((uint8_t*)buffer, 15);

    return MP_SUCCESSFUL;
}

int CFrameBase::SendFrame(uint32_t ctrl, void* payload, int payload_size, void* data, int datasize)
{
    ASSERT(payload_size >= 0);
    ASSERT(datasize >= 0);

    cLogHandle::printer(true, "SendFrame->>begin to send data\n");
    int PACKET_SIZE = MP_HEAD_SIZE + payload_size + datasize;

    if ((payload_size + datasize) > 0) {
        PACKET_SIZE += MP_PALOAD_CHECKSUM_SIZE;
    }

    // packet
    char* buffer = new char[PACKET_SIZE + 1];
    buffer[PACKET_SIZE] = 0;

    uint32_t plen = payload_size + datasize;

    // package header
    mp_packet_header(buffer, MTCP_PREAMBLE, ctrl, plen, 0, 0);

    // copy payload
    if (payload_size > 0) {
        memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)payload, payload_size);
        cLogHandle::printer(true, "SendFrame->>Add valid REQ data,datasize:%d\n", payload_size);
    }

    // copy data
    if (datasize > 0) {
        memcpy((void*)(buffer + MP_HEAD_SIZE + payload_size), (void*)data, datasize);
        cLogHandle::printer(true, "SendFrame->>Add valid CSV data,datasize:%d\n", datasize);
    }

    // calc check sum
    if ((payload_size + datasize) > 0) {
        cLogHandle::printer(true, "SendFrame->>Header size:%d, PayloadSize:%d, DataSize:%d\n", MP_HEAD_SIZE,
                            payload_size, datasize);
        buffer[MP_HEAD_SIZE + payload_size + datasize] =
            _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), payload_size + datasize);
    }

    cLogHandle::printer(true, "SendFrame->>Begin SendFrame data buffer:%s size:%d\n", buffer, PACKET_SIZE);
    int snd_cnt = m_session->Send(buffer, PACKET_SIZE);
    delete[] buffer;
    buffer = nullptr;

    cLogHandle::printer(true, "SendFrame->>SendFrame have send:%d\n", snd_cnt);
    if (snd_cnt <= 0) {
        cLogHandle::printer(true, "SendFrame->>SendFrame have send error msg: %s\n", m_session->GetError().c_str());
    }
    if (snd_cnt != PACKET_SIZE) {
        return MP_SOCKET_FAILED;
    }

    tMTCP_payload_GENL_REQ* REQ = (tMTCP_payload_GENL_REQ*)payload;
    cLogHandle::printer(true, "SendFrame->>Sent CSV Successful,DIRECTION:%d, FILE_SIZE:%d\n", REQ->DIRECTION,
                        REQ->FILE_SIZE);
    return MP_SUCCESSFUL;
}

int CFrameBase::RecvResponseEx(void* rspBuffer, int rspLen, std::string& csvString)
{
    // Read header
    cLogHandle::printer(true, "RecvResponseEx->>Begin receive RSP data header from MTCP\n");
    tMTCP_header header;
    int recvDataLen = 0;
    int rcv_cnt = m_session->Recieve((char*)&header, sizeof(header));
    cLogHandle::printer(true, "RecvResponseEx->>Recieve header data size:%d\n", rcv_cnt);
    if (rcv_cnt <= 0) {
        // m_session->Close();
        cLogHandle::printer(true, "RecvResponseEx->>Recieve header data error msg: %s\n",
                            m_session->GetError().c_str());
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 400;
        char sTemp[] = "Receive data header fail";
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        cLogHandle::printer(true, "%s\n", sTemp);
        return -400;
    }

    if (rcv_cnt != sizeof(header)) {
        // m_session->Close();
        char sTemp[] = "header size not match";
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 401;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return -401;
    }
    cLogHandle::printer(true, "RecvResponseEx->>Begin check CheckSum in header\n");
    // check checksum
    uint8_t h_cs = _MTCP_calculateCheckSum((uint8_t*)&header, sizeof(header) - 1);

    cLogHandle::printer(true, "RecvResponseEx->>Header checkSum extected:%d received: %d\n", header.H_CS, h_cs);
    if (h_cs != header.H_CS) {
        cLogHandle::printer(true, "RecvResponseEx->>Header CheckSum check fail\n");
    }

    // check error
    if (kMTCP_ERR_OK != header.ERRC) {
        char sTemp[40];
        sprintf_s(sTemp, "header error code:%d\0", header.ERRC);
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = header.ERRC;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return header.ERRC * (-1);
    }

    cLogHandle::printer(true, "RecvResponseEx->>Receive header from MTCP server successful\n");

    // successful!!!
    if (header.PLEN <= 0) {
        char sTemp[128];
        sprintf_s(sTemp, "RecvResponseEx->>header.PLEN less than zero :%d\0", header.PLEN);
        cLogHandle::printer(true, "%s\n", sTemp);
        return MP_SUCCESSFUL;
    }

    // Read RSP payload
    cLogHandle::printer(true, "RecvResponseEx->>receive RSP payload from MTCP\n");
    int payLen = rspLen;
    rcv_cnt = m_session->Recieve((char*)rspBuffer, rspLen);
    recvDataLen = recvDataLen + rcv_cnt;
    cLogHandle::printer(true, "RecvResponseEx->>recv RSP payload data(expected:%d, received:%d )\n", payLen, rcv_cnt);

    if (rcv_cnt <= 0) {
        // m_session->Close();
        cLogHandle::printer(true, "RecvResponseEx->>Recieve payload data error msg: %s\n",
                            m_session->GetError().c_str());
        char sTemp[] = "Receive RSP payload fail";
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 402;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return -402;
    }

    if (payLen != rcv_cnt) {
        // m_session->Close();
        char sTemp[] = "Receive RSP payload error";
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 403;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return -403;
    }

    // read csv buffer
    cLogHandle::printer(true, "RecvResponseEx->>receive RSP payload from MTCP successful\n");
    int payCSVLen = header.PLEN - 1 - rspLen;

    char* csvCString = NULL;
    if (payCSVLen <= 0) {
        char sTemp[128];
        sprintf_s(sTemp, "RecvResponseEx->>no csv file return from MTCP:%d\0", payCSVLen);
        payCSVLen = 0;
        cLogHandle::printer(true, "%s\n", sTemp);
    } else {
        csvCString = new char[payCSVLen + 1];
        csvCString[payCSVLen] = 0;
        rcv_cnt = m_session->Recieve(csvCString, payCSVLen);
        cLogHandle::printer(true, "RecvResponseEx->>recv RSP payload CSV data(expected:%d, received:%d )\n", payCSVLen,
                            rcv_cnt);
        recvDataLen = recvDataLen + rcv_cnt;
        if (rcv_cnt > 0) {
            // csvString = std::string(csvCString, rcv_cnt);
            csvString.assign(csvCString, rcv_cnt);
        } else {
            cLogHandle::printer(true, "RecvResponseEx->>Recieve csv data error msg: %s\n",
                                m_session->GetError().c_str());
        }

        if (payCSVLen != rcv_cnt) {
            // m_session->Close();
            char sTemp[] = "Receive CSV data error";
            cLogHandle::printer(true, "%s\n", sTemp);
            ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 404;
            memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
            return -404;
        }

        cLogHandle::printer(true, "RecvResponseEx->>recv payload CSV data successful\n");
    }

    // read check sum
    cLogHandle::printer(true, "RecvResponseEx->>Check RSP payload data CheckSum\n");
    char* payloaddata = new char[payCSVLen + rspLen + 1];
    payloaddata[payCSVLen + rspLen] = 0;
    memcpy((void*)(payloaddata), (void*)rspBuffer, rspLen);
    if (payCSVLen > 0) {
        memcpy((void*)(payloaddata + rspLen), (void*)csvCString, payCSVLen);
    }
    /*delete[] csvCString;
    csvCString = nullptr;*/
    uint8_t checksum = _MTCP_calculateCheckSum((uint8_t*)payloaddata, payCSVLen + rspLen);
    delete[] payloaddata;
    payloaddata = nullptr;
    uint8_t p_cs = 0;
    rcv_cnt = m_session->Recieve((char*)&p_cs, 1);

    cLogHandle::printer(true, "RecvResponseEx->>CheckSum expected:%d ,received:%d\n", checksum, p_cs);

    if (rcv_cnt <= 0) {
        cLogHandle::printer(true, "RecvResponseEx->>Recieve checkSum error msg: %s\n", m_session->GetError().c_str());
    }
    if (1 != rcv_cnt) {
        char sTemp[] = "payload checksum error";
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 405;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return -405;
    }

    if (checksum != p_cs) {
        char sTemp[] = "payload CheckSum fail";
        cLogHandle::printer(true, "%s\n", sTemp);
        ((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRC = 406;
        memcpy(((tMTCP_payload_GENL_RSP*)rspBuffer)->T_ERRS, sTemp, strlen(sTemp));
        return -406;
    }

    return recvDataLen;
}

void CFrameBase::DBG_HEX(const char* caption, const char* buffer, size_t size)
{
    return;

    ASSERT(caption);
    ASSERT(buffer);
    ASSERT(size > 0);

    const char HEX[] = "0123456789ABCDEF";

    std::string str;

    for (int i = 0; i < size; i++) {
        str += "0x";
        str += HEX[(int(buffer[i] & 0x000000ff)) >> 4];
        str += HEX[buffer[i] & 0x0f];

        if ((i > 0) && (7 == (i % 8))) {
            str += "\n";
        } else {
            if (i < size - 1) {
                str += " ";
            }
        }
    }

    // cLogHandle::printer(true,"%s:\n%s\n", caption, str.c_str());
}

//////////////////////////////////////////////////////////////////////////
// end file(s)!!!
//////////////////////////////////////////////////////////////////////////
