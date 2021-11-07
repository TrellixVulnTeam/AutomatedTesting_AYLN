
#ifdef _DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

#include <memory>
#include <exception>
#include <string>


#define ZMQREPAPI_FUNC(type) type
#define ZMQREPAPI_INSTANCE(cls) cls

typedef std::string(*ReplyCallBack)(void* buf, long len, void* context);

class DynamicZmqRep
{
public:
	virtual ZMQREPAPI_FUNC(int) Bind(const char* szSvrIp, int port) = 0;
	virtual ZMQREPAPI_FUNC(int) Bind(const char* address) = 0;
	virtual ZMQREPAPI_FUNC(int) Close() = 0;
	virtual ZMQREPAPI_FUNC(int) SetCallBack(ReplyCallBack cb, void* ctx) = 0;
	virtual ZMQREPAPI_FUNC(void) Start() = 0;
	virtual ZMQREPAPI_FUNC(std::string) GetIp() = 0;
	virtual ZMQREPAPI_FUNC(int) GetPort() = 0;

	virtual ZMQREPAPI_FUNC(int) Send(const void* data, int len) = 0;
	virtual ZMQREPAPI_FUNC(int) SendString(const char* buffer) = 0;
	virtual ZMQREPAPI_FUNC(std::string) GetError() = 0;
	virtual ZMQREPAPI_FUNC(bool) IsStart() = 0;

	virtual ZMQREPAPI_FUNC(void) destroy() = 0;
};


#define Extern extern "C" DLL_API

/// 生成实例对象
Extern ZMQREPAPI_INSTANCE(DynamicZmqRep*) CreateZmqRepHandle();

