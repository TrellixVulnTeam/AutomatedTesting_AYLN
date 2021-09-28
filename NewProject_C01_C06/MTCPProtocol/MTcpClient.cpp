#include "MTcpClient.h"
#include "logHandle.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>

#pragma comment(lib, "wsock32.lib")

#ifdef WIN32
#define sprintf sprintf_s
#endif

MTcpClient::MTcpClient()
{
    _ip = "127.0.0.1";
    _port = 61806;
    _timeOut = 10000;
}

MTcpClient::~MTcpClient()
{
    Close();
}

void MTcpClient::SetRecvDataCallBack(RecvDataCallBack callback, void* context)
{
    _recvDataCallback = callback;
    _recvDataContext = context;
}

void MTcpClient::SetCloseClientCallBack(CloseClientCallBack callback, void* context)
{
    _closeClientCallback = callback;
    _closeClientContext = context;
}

int MTcpClient::Connect(const char* szSvrIp, int port, int timeout)
{
    cLogHandle::printer(true, "Begin to Connect Ip[%s] port[%d]\n", szSvrIp, port);
    isOpen = false;
    _timeOut = timeout;
    int Ret = 0;
    int AddrLen = 0;

    WSADATA Ws;
    if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0) {
        printf("Init Socket Failed:%d\n", GetLastError());
        return -1;
    }

    _socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socketHandle == INVALID_SOCKET) {
        printf("Create Socket Failed:%d\n", GetLastError());
        return -1;
    }

    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(szSvrIp);
    ServerAddr.sin_port = htons(port);
    memset(ServerAddr.sin_zero, 0x00, 8);

    u_long mode = 1;
    if (ioctlsocket(_socketHandle, FIONBIO, &mode) == SOCKET_ERROR) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("set socket nonblock failed! Ip[%s] port[%d]", szSvrIp, port);
        return -1;
    }

    int ret = connect(_socketHandle, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (ret == SOCKET_ERROR && WSAEWOULDBLOCK != GetLastError()) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("Connect server failed, error=[%d]! Ip[%s] port[%d]", GetLastError(), szSvrIp, port);
        cLogHandle::printer(true, "Connect server failed, error=[%d]! Ip[%s] port[%d]\n", GetLastError(), szSvrIp,
                            port);
        return -1;
    }

    fd_set fdWrite;
    FD_ZERO(&fdWrite);
    FD_SET(_socketHandle, &fdWrite);
    timeval tv = { 1.2 };  // 1.5秒超时
    if (select(0, NULL, &fdWrite, NULL, &tv) <= 0 || !FD_ISSET(_socketHandle, &fdWrite)) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("Connect to server failed! Ip[%s] port[%d]", szSvrIp, port);
        cLogHandle::printer(true, "Connect to server failed!\n");
        return -1;
    }

    mode = 0;
    if (ioctlsocket(_socketHandle, FIONBIO, &mode) == SOCKET_ERROR) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("set socket block failed! Ip[%s] port[%d]", szSvrIp, port);
        return -1;
    }

    // bool bKeepAlive = true;
    // ret = ::setsockopt(_socketHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
    // if (ret == SOCKET_ERROR)
    // {
    // 	printf("set keepalive fail:%d", GetLastError());
    // }

    struct linger
        so_linger;  //在调用closesocket的时候会立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，也就是强制的退出。
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    ret = setsockopt(_socketHandle, SOL_SOCKET, SO_LINGER, (char*)&so_linger, sizeof(so_linger));

    if (SOCKET_ERROR == setsockopt(_socketHandle, SOL_SOCKET, SO_SNDTIMEO, (char*)&_timeOut, sizeof(int))) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("Set Send OverTime. Error:%d\n", GetLastError());
        return -1;
    }

    if (SOCKET_ERROR == setsockopt(_socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char*)&_timeOut, sizeof(int))) {
        closesocket(_socketHandle);
        _socketHandle = 0;
        printf("Set Recv OverTime. Error:%d\n", GetLastError());
        return -1;
    }

    _ip = szSvrIp;
    _port = port;
    isOpen = true;
    printf("Connect Server Success. IP[%s] Port[%d]\n", szSvrIp, port);
    cLogHandle::printer(true, "Connect Server Success. IP[%s] Port[%d]\n", szSvrIp, port);
    return 0;
}

bool MTcpClient::isOpened()
{
    cLogHandle::printer(true, "isOpened:%d\n", isOpen);
    return isOpen;
}

void MTcpClient::Close()
{
    if (!isOpen) {
        return;
    }
    isOpen = false;
    if (!_isExit && _workThread != NULL && _workThread->joinable()) {
        _isExit = true;
        _workThread->join();
        _workThread = NULL;
    }

    if (_socketHandle != 0) {
        closesocket(_socketHandle);
        _socketHandle = 0;
    }
    // WSACleanup();
    cLogHandle::printer(true, "Close CTcpClient succesful\n");
}

int MTcpClient::SendString(const std::string& str)
{
    return Send(str.c_str(), str.size());
}

int MTcpClient::Send(const char* data, size_t len)
{
    int haveSend = 0;
    while (haveSend < len) {
        size_t snd = send(_socketHandle, data + haveSend, len - haveSend, 0);
        if (snd < 0) {
            int lastErr = WSAGetLastError();
            if (lastErr == WSAEWOULDBLOCK || lastErr == WSAEINTR) {
                snd = 0;
                continue;
            }

            char szError[1024];
            sprintf(szError, "Send data failed. Error is %d", lastErr);
            printf("%s\n", szError);
            SetError(szError);

            return -1;
        }
        haveSend += snd;
    }

    return haveSend;
}

int MTcpClient::Recieve(char* buffer, int size)
{
    size_t rcv_cnt = 0;
    size_t rcv = 0;

    while (rcv_cnt < size) {
        int rcv = recv(_socketHandle, buffer + rcv_cnt, size - rcv_cnt, 0);
        if (rcv < 0) {
            int lastErr = WSAGetLastError();
            char szError[1024];
            sprintf(szError, "Recv data failed. Error is %d", lastErr);
            printf("%s\n", szError);
            SetError(szError);

            Stop();
            return -1;
        } else if (rcv == 0) {
            std::string info = "The Server have been closed.\n";
            printf("%s", info.c_str());
            SetError(info);
            Stop();
            return 0;
        }
        rcv_cnt += rcv;
    }
    return rcv_cnt;
}

int MTcpClient::RecvData()
{
    char szBuf[8192] = { 0 };
    int readNum = recv(_socketHandle, szBuf, sizeof(szBuf), 0);
    if (readNum < 0) {
        int lastErr = WSAGetLastError();
        char szError[1024];
        sprintf(szError, "Recv data failed. Error is %d", lastErr);
        printf("%s\n", _error);
        SetError(szError);

        Stop();
        return -1;
    } else if (readNum == 0) {
        std::string info = "The Server have been closed.\n";
        printf("%s", info.c_str());
        SetError(info);
        Stop();
        return 0;
    }

    std::string data;
    data.assign(szBuf, readNum);
    if (_recvDataCallback != NULL) {
        _recvDataCallback(data, _recvDataContext);
    } else {
        printf("Recv data:%s\n", data.c_str());
    }
    return 0;
}

bool MTcpClient::Select()
{
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(_socketHandle, &rfd);

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    int ret = select(_socketHandle + 1, &rfd, NULL, NULL, &timeout);
    if (ret == 0 || ret == SOCKET_ERROR) {
        return false;
    }

    if (FD_ISSET(_socketHandle, &rfd)) {
        return true;
    }

    return false;
}

void MTcpClient::OnProcServerData(void* context)
{
    MTcpClient* client = static_cast<MTcpClient*>(context);
    while (!client->_isExit) {
        if (client->Select()) {
            client->RecvData();
        }
    }
}

void MTcpClient::Start()
{
    _isExit = false;
    _workThread = new std::thread(MTcpClient::OnProcServerData, this);
}

void MTcpClient::Stop()
{
    _isExit = true;

    if (_closeClientCallback != NULL) {
        _closeClientCallback(_closeClientContext);
    }
}

std::string MTcpClient::GetError()
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _error;
}

void MTcpClient::SetError(const std::string& msg)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _error = msg;
}
