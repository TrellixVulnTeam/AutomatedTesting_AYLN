#pragma once

/***************************************************************
 * Purpose:   tcp/ip client 封装类
 * Author:    wuzx ()
 * Created:   2016-10-28
 **************************************************************/

#include <string>
#include <thread>
#include <mutex>
#include <winsock2.h>

typedef void (*RecvDataCallBack)(const std::string& data, void* context);
typedef void (*CloseClientCallBack)(void* context);

class MTcpClient
{
public:
    MTcpClient();
    ~MTcpClient();

    int Connect(const char* szSvrIp, int port, int timeout);
    void Close();
    void Start();

    void SetTimeout(int timeout)
    {
        _timeOut = timeout;
    }
    int SendString(const std::string& str);
    int Send(const char* data, size_t len);
    int Recieve(char* buffer, int size);
    bool isOpened();
    void SetRecvDataCallBack(RecvDataCallBack callback, void* context);
    void SetCloseClientCallBack(CloseClientCallBack callback, void* context);

    std::string GetError();

private:
    void Stop();
    static void OnProcServerData(void* context);
    int RecvData();
    bool Select();
    void SetError(const std::string& msg);

private:
    std::thread* _workThread = NULL;
    unsigned int _socketHandle = 0;

    bool _isExit = false;
    bool isOpen = false;
    std::string _ip;
    int _port = -9999;
    int _timeOut = 3000;

    std::string _error;

    RecvDataCallBack _recvDataCallback = NULL;
    void* _recvDataContext = NULL;

    CloseClientCallBack _closeClientCallback = NULL;
    void* _closeClientContext = NULL;

    std::mutex _mutex;
};
