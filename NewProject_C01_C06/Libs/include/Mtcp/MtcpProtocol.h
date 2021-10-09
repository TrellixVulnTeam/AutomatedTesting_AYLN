
#ifdef _DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

#include <memory>
#include <exception>
#include <string>


#define MTCPAPI_FUNC(type) type
#define MTCPAPI_INSTANCE(cls) cls

class DLL_API MTCP_ProtocolException
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

class DynamicMtcp
{
public:
	enum DirectionType
	{
		REQUEST = 0,
		REPORT = 1
	};

	virtual MTCPAPI_FUNC(int) open(const std::string& address, uint16_t port, int timeout = 10000) = 0;
	virtual MTCPAPI_FUNC(bool) isOpen(void) = 0;
	virtual MTCPAPI_FUNC(void) close(void) = 0;
	virtual MTCPAPI_FUNC(void) DBG_HEX(const char* caption, const char* buffer, size_t size) = 0;

	virtual MTCPAPI_FUNC(const char*) SendGENL(const std::string& filename, DirectionType direction) = 0;
	virtual MTCPAPI_FUNC(int) rspType(void) = 0;
	virtual MTCPAPI_FUNC(void) setTestCSVPath(const std::string& csvPath) = 0;
	virtual MTCPAPI_FUNC(void) setLogPath(const std::string& logPath) const = 0;
	virtual MTCPAPI_FUNC(const char*) version() const = 0;

	virtual MTCPAPI_FUNC(const std::string&) csvFile(void) const = 0;
	virtual MTCPAPI_FUNC(int) csvSize(void) const = 0;
	virtual MTCPAPI_FUNC(void) destroy() = 0;
};

#define Extern extern "C" DLL_API

/// 生成实例对象
Extern MTCPAPI_INSTANCE(DynamicMtcp*) CreateMtcpHandle(const std::string& csvPath);




