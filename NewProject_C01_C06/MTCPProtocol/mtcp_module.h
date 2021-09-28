#ifndef __MTCP_MODULE__H__
#define __MTCP_MODULE__H__

#include <memory>
#include <exception>
#include <string>
#include "FrameBase.h"

class MTCP_ProtocolException
{
public:
    ~MTCP_ProtocolException() {}

    static MTCP_ProtocolException* GetInstance(int errCode = NULL, const std::string& errStr = "")
    {
        static MTCP_ProtocolException* singleton = new MTCP_ProtocolException(errCode, errStr);
        return singleton;
    }

    int getErrCode(void) const
    {
        return errCode;
    }

    void setErrCode(int _errCode)
    {
        errCode = _errCode;
    }

    std::string getErrStr() const
    {
        return errStr;
    }

    void setErrStr(const std::string& _errStr)
    {
        errStr = _errStr;
    }

private:
    MTCP_ProtocolException(int _errCode, const std::string& _errStr)
    {
        if (_errCode)
            errCode = _errCode;

        if ("" != _errStr)
            errStr = _errStr;
    }

private:
    int errCode;
    std::string errStr;
};

namespace MTcp
{
class GENL : public CFrameBase
{
public:
    enum DirectionType
    {
        REQUEST = 0,
        REPORT = 1
    };

    static GENL* getInstance(const std::string& csvPath)
    {
        static GENL instance(csvPath);
        return &instance;
    }
    virtual ~GENL(void);

    const char* SendGENL(const std::string& filename, DirectionType direction);
    int rspType(void);
    void setTestCSVPath(const std::string& csvPath);
    void setLogPath(const std::string& logPath) const;
    const char* version() const;

    const std::string& csvFile(void) const;
    int csvSize(void) const;

private:
    GENL(const std::string& csvPath);

    int readFile(const char* filepath);
    int saveFile(const char* filepath, const char* file, int length);
    void clear(void);

    tMTCP_payload_GENL_REQ m_GENL_REQ;
    tMTCP_payload_GENL_RSP m_GENL_RSP;
    char* pivotRawdata;
    std::string csv_File;
    std::string testCSVPath;
};
}  // namespace MTcp

#endif  //__MTCP_MODULE__H__
