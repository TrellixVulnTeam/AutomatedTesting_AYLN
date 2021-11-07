// MtcpProtocol.cpp : 定义 DLL 应用程序的导出函数。
//

#define _DLL_EXPORTS
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "MtcpProtocol.h"
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include "MTCProto.h"
#include "MTcpClient.h"
#include "logHandle.h"
#include "MTCP_Rosaline.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

#define ASSERT assert

const std::size_t MP_HEAD_SIZE = sizeof(tMTCP_header);
const std::size_t MP_PALOAD_CHECKSUM_SIZE = 1;

class GENL : public DynamicMtcp
{
public:
	GENL(const std::string& csvPath);
	virtual ~GENL(void);

	virtual MTCPAPI_FUNC(int) open(const std::string& address, uint16_t port, int timeout = 10000);
	virtual MTCPAPI_FUNC(bool) isOpen(void);
	virtual MTCPAPI_FUNC(void) close(void);
	virtual MTCPAPI_FUNC(void) DBG_HEX(const char* caption, const char* buffer, size_t size);

	virtual MTCPAPI_FUNC(const char*) SendGENL(const std::string& filename, DirectionType direction);
	virtual MTCPAPI_FUNC(int) rspType(void);
	virtual MTCPAPI_FUNC(void) setTestCSVPath(const std::string& csvPath);
	virtual MTCPAPI_FUNC(void) setLogPath(const std::string& logPath) const;
	virtual MTCPAPI_FUNC(const char*) version() const;

	virtual MTCPAPI_FUNC(const std::string&) csvFile(void) const;
	virtual MTCPAPI_FUNC(int) csvSize(void) const;
	virtual MTCPAPI_FUNC(void) destroy();

private:
	int SendFrame(uint32_t ctrl, void* payload, int payload_size, void* data, int datasize);
	// virtual int RecvResponse(void * buffer, int len);
	int RecvResponseEx(void* rspBuffer, int rspLen, std::string& csvString);

	int mp_packet_header(INOUT void* buffer, IN uint32_t MTCP,
		IN uint32_t CTRL,  //  packet control id (defined below)
		IN uint32_t PLEN,  //  payload length (if any), CTRL specific
		IN uint16_t ERRC,  //  valid only for backward packet, tMTCP_ERR code (defined below)
		IN uint8_t SEQN    //  forward packet sequence number
	);

	int getResponseToJson(char* buffer, int size)
	{
		return 0;
	}

	int readFile(const char* filepath);
	int saveFile(const char* filepath, const char* file, int length);
	void clear(void);

private:
	std::shared_ptr<MTcpClient> m_session;
	tMTCP_payload_GENL_REQ m_GENL_REQ;
	tMTCP_payload_GENL_RSP m_GENL_RSP;
	char* pivotRawdata;
	std::string csv_File;
	std::string testCSVPath;
};

GENL::GENL(const std::string& csvPath) : DynamicMtcp(), testCSVPath(csvPath), m_session(std::shared_ptr<MTcpClient>(new MTcpClient))
{
	memset(&m_GENL_REQ, 0, sizeof(m_GENL_REQ));
	memset(&m_GENL_RSP, 0, sizeof(m_GENL_RSP));
	m_GENL_REQ.FILE_TYPE = 0;
	pivotRawdata = nullptr;
}

GENL::~GENL(void)
{
	if (pivotRawdata != nullptr) {
		delete[] pivotRawdata;
		pivotRawdata = nullptr;
	}
	cLogHandle::printer(true, "Delete GENL object\n");
}

int GENL::open(const std::string& address, uint16_t port, int timeout)
{
	return m_session->Connect(address.c_str(), port, timeout);
}

bool GENL::isOpen(void)
{
	return m_session->isOpened();
}

void GENL::close(void)
{
	m_session->Close();
}

void GENL::DBG_HEX(const char* caption, const char* buffer, size_t size)
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
		}
		else {
			if (i < size - 1) {
				str += " ";
			}
		}
	}

	// cLogHandle::printer(true,"%s:\n%s\n", caption, str.c_str());
}

const char* GENL::SendGENL(const std::string& filename, DirectionType direction)
{
	clear();

	if (filename.empty()) {
		m_GENL_RSP.T_ERRC = 301;
		const char* sTemp = "request csv file is \"\"";
		memcpy((void*)m_GENL_RSP.T_ERRS, sTemp, strlen(sTemp));
		cLogHandle::printer(true, "%s\n", sTemp);
		return sTemp;
	}

	if (direction == REQUEST) {
		remove(testCSVPath.c_str());
	}

	m_GENL_REQ.FILE_SIZE = readFile(filename.c_str());
	m_GENL_REQ.DIRECTION = direction;

	int ret = SendFrame(kMTCP_CTRL_GENL, &m_GENL_REQ, sizeof(m_GENL_REQ), pivotRawdata, m_GENL_REQ.FILE_SIZE);
	if (MP_SUCCESSFUL != ret) {
		m_GENL_RSP.T_ERRC = 302;
		char sTemp[] = "SendGENL:SendFrame Fail";
		cLogHandle::printer(true, "%s\n", sTemp);
		memcpy((void*)m_GENL_RSP.T_ERRS, sTemp, strlen(sTemp));
		return sTemp;
	}

	int recv_cnt = RecvResponseEx(&m_GENL_RSP, sizeof(m_GENL_RSP), csv_File);
	if (recv_cnt < 0) {
		return (const char*)m_GENL_RSP.T_ERRS;
	}

	cLogHandle::printer(true, "GENL::SendGENL->>Receive csv data size:%d\n", csv_File.length());
	if (csv_File.length() > 0) {
		saveFile(testCSVPath.c_str(), csv_File.c_str(), csv_File.length());
		FILE* fp = fopen(testCSVPath.c_str(), "rb");
		if (fp) {
			fseek(fp, 0, SEEK_END);
			int file_size = ftell(fp);
			if (file_size == 0) {
				fclose(fp);
				saveFile(testCSVPath.c_str(), csv_File.c_str(), csv_File.length());
				cLogHandle::printer(true, "GENL::SendGENL->>save test script file again!\n");
			}
			else
				fclose(fp);
		}

		cLogHandle::printer(true, "GENL::SendGENL->>save csv data to path :%s\n", testCSVPath.c_str());
	}

	cLogHandle::printer(true, "GENL::SendGENL->>Receive T_ERRS:%s\n", (const char*)m_GENL_RSP.T_ERRS);
	std::string errS = std::string((const char*)m_GENL_RSP.T_ERRS);
	if (!errS.empty() && errS.find("ok") == std::string::npos) {
		return (const char*)m_GENL_RSP.T_ERRS;
	}

	return "";
}

int GENL::rspType(void)
{
	return m_GENL_RSP.FILE_TYPE;
}

void GENL::setTestCSVPath(const std::string& csvPath)
{
	testCSVPath = csvPath;
	cLogHandle::printer(true, "GENL::setTestCSVPath->>set test csv save path:%s\n", testCSVPath.c_str());
}

void GENL::setLogPath(const std::string& logPath) const
{
	cLogHandle::InitLog(logPath.c_str());
}

const char* GENL::version() const
{
	return "MTCP Dll V1.0 By li";
}

const std::string& GENL::csvFile(void) const
{
	return csv_File;
}

int GENL::csvSize(void) const
{
	return m_GENL_RSP.FILE_SIZE;
}

int GENL::SendFrame(uint32_t ctrl, void* payload, int payload_size, void* data, int datasize)
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

int GENL::RecvResponseEx(void* rspBuffer, int rspLen, std::string& csvString)
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
	}
	else {
		csvCString = new char[payCSVLen + 1];
		csvCString[payCSVLen] = 0;
		rcv_cnt = m_session->Recieve(csvCString, payCSVLen);
		cLogHandle::printer(true, "RecvResponseEx->>recv RSP payload CSV data(expected:%d, received:%d )\n", payCSVLen,
			rcv_cnt);
		recvDataLen = recvDataLen + rcv_cnt;
		if (rcv_cnt > 0) {
			// csvString = std::string(csvCString, rcv_cnt);
			csvString.assign(csvCString, rcv_cnt);
		}
		else {
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

int GENL::mp_packet_header(INOUT void* buffer, IN uint32_t MTCP, IN uint32_t CTRL, IN uint32_t PLEN,
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
	}
	else {
		ptMTCP_header->PLEN = (PLEN);
	}

	// ptMTCP_header->PLEN = 0x2000002;
	ptMTCP_header->SEQN = SEQN;

	ptMTCP_header->H_CS = _MTCP_calculateCheckSum((uint8_t*)buffer, 15);

	return MP_SUCCESSFUL;
}

int GENL::readFile(const char* filepath)
{
	FILE* fp = fopen(filepath, "rb");
	if (NULL == fp) {
		cLogHandle::printer(true, "GENL::readFile->>read file fail!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);

	pivotRawdata = new char[file_size + 1];
	memset((void*)pivotRawdata, 0, file_size + 1);

	fseek(fp, 0, SEEK_SET);
	fread(pivotRawdata, file_size, sizeof(char), fp);
	fclose(fp);
	cLogHandle::printer(true, "GENL::readFile->>data size by ftell:%d \n", file_size);
	pivotRawdata[file_size] = 0;

	return file_size;
}

int GENL::saveFile(const char* filepath, const char* file, int length)
{
	FILE* fp = fopen(filepath, "wb");
	if (NULL == fp) {
		cLogHandle::printer(true, "GENL::saveFile->>create file fail!\n");
		return -1;
	}
	// fseek(fp, 0, SEEK_END);
	fwrite(file, length, 1, fp);
	fflush(fp);
	fclose(fp);
	return 0;
}

void GENL::clear(void)
{
	cLogHandle::printer(true, "GENL::clear->>clear data buffer begin\n");
	memset(&m_GENL_REQ, 0, sizeof(m_GENL_REQ));
	memset(&m_GENL_RSP, 0, sizeof(m_GENL_RSP));
	if (pivotRawdata != nullptr) {
		delete[] pivotRawdata;
		pivotRawdata = nullptr;
	}
	csv_File.clear();
	cLogHandle::printer(true, "GENL::clear->>clear data buffer finish\n");
}

void GENL::destroy()
{
	delete this;
}


Extern MTCPAPI_INSTANCE(DynamicMtcp*) CreateMtcpHandle(const std::string& csvPath)
{
	return new GENL(csvPath);
}



