
#include "mtcp_module.h"
#include <QString>

namespace MTcp
{
GENL::GENL(const std::string& csvPath) : CFrameBase(), testCSVPath(csvPath)
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

void GENL::setTestCSVPath(const std::string& csvPath)
{
    testCSVPath = csvPath;
    cLogHandle::printer(true, "GENL::setTestCSVPath->>set test csv save path:%s\n", testCSVPath.c_str());
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
            } else
                fclose(fp);
        }

        cLogHandle::printer(true, "GENL::SendGENL->>save csv data to path :%s\n", testCSVPath.c_str());
    }

    cLogHandle::printer(true, "GENL::SendGENL->>Receive T_ERRS:%s\n", (const char*)m_GENL_RSP.T_ERRS);
    std::string errS = std::string((const char*)m_GENL_RSP.T_ERRS);
    if (!errS.empty() && !QString::fromStdString(errS).contains("ok", Qt::CaseInsensitive)) {
        return (const char*)m_GENL_RSP.T_ERRS;
    }

    return "";
}

int GENL::rspType(void)
{
    return m_GENL_RSP.FILE_TYPE;
}

const std::string& GENL::csvFile(void) const
{
    return csv_File;
}

int GENL::csvSize(void) const
{
    return m_GENL_RSP.FILE_SIZE;
}

void GENL::setLogPath(const std::string& logPath) const
{
    cLogHandle::InitLog(logPath.c_str());
}

const char* GENL::version() const
{
    return "MTCP Wapper V2.0";
}

}  // namespace MTcp
