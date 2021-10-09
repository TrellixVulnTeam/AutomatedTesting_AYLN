
#include "logHandle.h"
#include "time.h"
#include "windows.h"
#include "UtilDll.h"

std::ofstream cLogHandle::f;
std::string cLogHandle::g_date;
std::string cLogHandle::g_path = "D:/vault/JCiT/MtcpLog/MTCP_Log";

std::string GetDate()
{
    SYSTEMTIME date;
    GetLocalTime(&date);
    char buf[128];
    sprintf_s(buf, "_%u_%u_%u", date.wYear, date.wMonth, date.wDay);
    return std::string(buf);
}

int cLogHandle::InitLog(std::string path)
{
    std::string s = GetDate();
    g_path.clear();
    g_path = path;
    g_date.clear();
    g_date = s;
    path += s;
    path += std::string(".log");
	UtilDll::MakeNDir(path);

    if (!cLogHandle::f.is_open()) {
        cLogHandle::f.open(path, std::ios::out | std::ios::app);
    } else {
        cLogHandle::f.close();
        cLogHandle::f.open(path, std::ios::out | std::ios::app);
    }

    return 0;
}

int cLogHandle::LogcMsg(const char* str, bool timeStamp)
{
    cLogHandle::LogMsg(std::string(str), timeStamp);
    return 0;
}

int cLogHandle::LogMsg(std::string str, bool timeStamp)
{
    std::string tmp;
    tmp.clear();
    if (timeStamp == true) {
        char d[128];
        SYSTEMTIME time;
        GetLocalTime(&time);
        sprintf_s(d, "%u-%u-%u_%u:%u:%u.%u\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute,
                  time.wSecond, time.wMilliseconds);
        tmp += std::string(d);
    }
    tmp += str;
    // tmp += std::string("\r\n");
    std::string date = GetDate();
    if (date != g_date) {
        cLogHandle::CloseLog();
        cLogHandle::InitLog(g_path);
    }

    if (cLogHandle::f.is_open()) {
        cLogHandle::f.seekp(std::ios::end);
        // cLogHandle::f << tmp.c_str();
        cLogHandle::f.write(tmp.c_str(), strlen(tmp.c_str()));
        cLogHandle::f.flush();
    }

    return 0;
}

int cLogHandle::printer(bool timestamp, const char* fmt, ...)
{
    std::string str;
    str.clear();
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    char printf_buf[1024];
    vsprintf_s(printf_buf, fmt, arg_ptr);
    puts(printf_buf);

    char buf[1028];
    memset(buf, 0, 1028);
    vsprintf_s(buf, fmt, arg_ptr);
    str += std::string(buf);
    cLogHandle::LogMsg(str, timestamp);
    va_end(arg_ptr);
    return 0;
}

int cLogHandle::CloseLog()
{
    if (cLogHandle::f.is_open()) {
        cLogHandle::f.close();
    }

    return 0;
}
