//
//  tools.h
//
//  Created by Joon Kwon on 8/27/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_UTILS_H_
#define _ML_UTILS_H_

#ifdef __ML_USE_AS_EMB__
#include <ml_typedef.h>
#include <ml_error.h>
#include <ml_file.h>
#include <ml_log.h>
#include <ml_string.h>
#else
#include <ml_typedef.h>
#include <ml_error.h>
#include <ml_file.h>
#include <ml_log.h>
#include <ml_string.h>
#endif

#if defined(_MSC_VER) && !defined(__func__)
#define __func__ __FUNCTION__
#endif

#if defined(__cplusplus)
extern "C" {
#endif
	
	/*	 system information	------------------------------------------------------------------------
	 */
#if defined(__APPLE__)
#	pragma mark -
#	pragma mark system information
	// system mac address
	const char* mlUtil_getMacAddress(void);
	// system serial number
	const char* mlUtil_getMacSerialNumber(void);

	/*	 system endianess	------------------------------------------------------------------------
	 */
#	pragma mark -
#	pragma mark system endianess
	static inline int mlUtil_GetSystemEndianess(){
		uint8_t tmp[2] = {0x01, 0x02};
		uint16_t* pTmp = (uint16_t*)tmp;
		return (*pTmp==0x0102)?BIG_ENDIAN:LITTLE_ENDIAN;
	}
#endif//__APPLE__
#	define SWAP_UINT64(x) (\
						(((x)&0xFF00000000000000)>>56)\
						|(((x)&0x00FF000000000000)>>40)\
						|(((x)&0x0000FF0000000000)>>24)\
						|(((x)&0x000000FF00000000)>>8)\
						|(((x)&0x00000000FF000000)<<8)\
						|(((x)&0x0000000000FF0000)<<24)\
						|(((x)&0x000000000000FF00)<<40)\
						|(((x)&0x00000000000000FF)<<56)\
						)
#	define SWAP_UINT32(x) (\
						(((x)&0xFF000000)>>24)\
						|(((x)&0x00FF0000)>>8)\
						|(((x)&0x0000FF00)<<8)\
						|(((x)&0x000000FF)<<24))
#	define SWAP_UINT16(x) (\
						(((x)&0xFF00)>>8)\
						|(((x)&0x00FF)<<8))

	static inline void mlUtil_getName(uint32_t name, char* strBuff){
		// convert uint32_t magic to string (swap & string conv)
		// use at your own risk
		assert(strBuff!=NULL);
		*(uint32_t*)strBuff = SWAP_UINT32(name);
		strBuff[4]=0;
	}
	
	static inline void mlUtil_getMagic(const char* strBuff, uint32_t* name){
		// convert string to magic (swap & bit or)
		assert(strBuff);
		assert(name);
		*name = strBuff[0] << 24 | strBuff[1] << 16 | strBuff[2] << 8 | strBuff[3];
	}
	
	/*	 bundle info	----------------------------------------------------------------------------
	 */
#	pragma mark -
#	pragma mark Bundle Info
#if defined(__APPLE__)
	// When bundleIdentifier or identifier NULL, MainBundle will be used
	const char* mlUtil_GetBundleName(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetBundleIdentifider(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetBundleShortVersion(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetBundleBuildVersion(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetBundlePath(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetBundleResPath(const char* bundleIdentifier = NULL);
	const char* mlUtil_GetUserHomePath(void);
	const char*	mlUtil_GetAppPath(void);
	const char* mlUtil_GetAppResPath(void);
	// Following 3 APIs are not Thread safe, may cause data racing issue
	// Use C++ API instead in multi-threading condition
	const char* mlUtil_GetUserAppCachePath(bool withBundleName = true, bool withBundleVer = true, bool withBundleIdentifier = true);
	const char* mlUtil_GetAppSupportPath(bool withBundleName = false, bool withBundleVer = false, bool withBundleIdentifier = true);
	const char* mlUtil_GetAppLogPath(bool withBundleName = false, bool withBundleVer = false, bool withBundleIdentifier = true);
	//
	const char* mlUtil_GetUUID(void);
	//
	const char* mlUtil_GetBundleNameFromUrl(const char* bundleUrl);
	const char* mlUtil_GetBundleIdentifiderFromUrl(const char* bundleUrl);
	const char* mlUtil_GetBundleShortVersionFromUrl(const char* bundleUrl);
	const char* mlUtil_GetBundleBuildVersionFromUrl(const char* bundleUrl);
	//
	bool mlUtil_IsBundleRunning(const char* bundleIdentifier);
#endif//__APPLE__

	/*	 systme info	----------------------------------------------------------------------------
	 */
#	pragma mark -
#	pragma mark system info
	#define _ML_CFABSOLUTE_TIME_BASE_ 978307200.0f // (GMT): Monday, January 1, 2001 12:00:00 AM
	ML_API	double	mlUtil_getCurrentTime();
	// Get reference timestamp in ms in local timezone with absolute reference date of 1 Jan 2001 00:00:00 GMT
	ML_API	time_t	mlUtil_getUnixtime(double currentTime);
	// Get unix timestamp (since 00:00, Jan 1 1970 UTC) from reference timestamp, i.e. from mlUtil_getCurrentTime
 	ML_API	void mlUtil_formatTimestampStr(double timestamp, char* strBuff, int buffLeng, int type);
	ML_API	double	mlUtil_getTimestampStr(char* strBuff, int buffLeng, int type);
	// type=0	YYYY-MM-dd HH:mm:ss
	// type=1	YYYYMMddHHmmss
	// type=2   %Y%m%d%H%M%S
	// type3	yyyy-MM-dd HH:mm:ss.SSS
	// type4 	yyyyMMddHHmmssSSS
	// type5	HH:mm:ss
	double	mlUtil_getFromTimestampStr(const char* str, int strLen, int strType,
									   uint16_t* year, uint8_t* month, uint8_t* dayOfMonth,
									   uint8_t* hour, uint8_t* minute, uint8_t* second);
	// type=0	YYYY-MM-dd HH:mm:ss
	// type=1	YYYYMMddHHmmss
	// type=2   %Y%m%d%H%M%S
	// type3	yyyy-MM-dd HH:mm:ss.SSS
	// type4 	yyyyMMddHHmmssSSS
	// type5	HH:mm:ss
	ML_API	t_mlError mlUtil_getCalendar(uint16_t* year, uint8_t* month, uint8_t* dayOfMonth);
	ML_API	t_mlError mlUtil_getTime(uint8_t* hour, uint8_t* minute, uint8_t* second);

#if defined(__APPLE__)
	void					mlSys_getDiskSpace(double* totalSpaceGB, double* freeSpaceGB);
	double					mlSys_processMemUsage();
#endif//__APPLE__

	/*	 aligned/multi-dimension memory allocation	------------------------------------------------
	 */
#pragma mark -
#pragma mark aligned/multi-dimension memory allocation
	ML_API	void*	mlUtil_alignedMemory_alloc(t_mlDataType type, uint16_t dimension, uint16_t* dSize, int *rtnMemSize);
	ML_API	void	mlUtil_alignedMemory_free(void* ptr);
	
	/*	process	------------------------------------------------------------------------------------
	 */
#pragma mark -
#pragma mark process
	t_mlError	mlUtil_runTask(CML_logManager* lm, std::string cmd, std::vector<std::string> args, std::string& errMsg);
#if defined(__APPLE__)
	pid_t		mlUtil_launchProcess(const char* command, const char* fOut);
	//			description:
	//				launch process and returns the process id
	//			parameter:
	//				command		- null terminated command string to launch a process
	//				fOut		- [optional, can be NULL] filename to be set to the stdout of the process
	//								if it is set to NULL, current stdout will be set
	//			return
	//				pid_t		- the process ID of the process
	t_mlError	mlUtil_performProcess(const char* command, const char* fOut, int* pExitCode);
	//			description:
	//				launch process, wait for the completion of the process and return the exit code
	//			parameter:
	//				command		- null terminated command string to launch a process
	//				fOut		- [optional, can be NULL] filename to be set to the stdout of the process
	//								if it is set to NULL, current stdout will be set
	//				fExitCode	- [optional, can be NULL] the process exit/return code
	//			return
	//				t_mlError	- mesa library error
	t_mlError	mlUtil_killProcess(pid_t pid);
	//			parameter:
	//				pid			- the process id to be killed
	//			return
	//				t_mlError	- mesa library error
#endif//__APPLE__

	/*	 miscellaneous	----------------------------------------------------------------------------
	 */
	
	// count one
	static inline int mlUtil_countOne(uint64_t val){int cnt =0;while(val){cnt++;val=val& ~(val^(val-1));}return cnt;}
	// random number generation
#	define mlUtil_generateRandomNumber(bound) (int)(((double)rand())/RAND_MAX*(bound))
#if defined(__cplusplus)
#if defined(__APPLE__)
	// binary data printing
	void mlUtil_printBinary(CML_logManager* lm, t_mlVbsLvl vbsLvl, void* data, uint32_t leng);
	// binary data saving
	t_mlError mlUtil_saveBitStream(CML_logManager* lm, const char* fileName, const uint8_t* strmBuff, int leng, t_mlVbsLvl vbsLvl);
	// binary data saving in background
	t_mlError mlUtil_archiveBitStream(CML_logManager* lm, const char* fileName, const uint8_t* strmBuff, int leng, t_mlVbsLvl vbsLvl);
	// binary data loading
	t_mlError mlUtil_getBitStream(CML_logManager* lm, const char* fileName, uint8_t** ppData, int* pLeng, t_mlVbsLvl vbsLvl);
	t_mlError mlUtil_loadBitStream(CML_logManager* lm, const char* fileName, uint8_t* strmBuff, int* pLeng, t_mlVbsLvl vbsLvl);
	// binary data to stream
	t_mlError mlUtil_toHexString(CML_logManager* lm, std::string& outStr, const uint8_t* strmBuff, int leng, t_mlVbsLvl vbsLvl);
	t_mlError mlUtil_loadHexString(CML_logManager* lm, std::string inStr, void **data, size_t data_size, t_mlVbsLvl vbsLvl);
#endif//__APPLE__
#endif

	/*	debug tools	--------------------------------------------------------------------------------
	 */
#	pragma mark -
#	pragma mark debug tools
	ML_API	void mlDebug_PrintMarker(const char* fileName, const char* className, const char* functionName, const int lineNumber);
#	if defined(DEBUG)
#		define __MARK__		mlDebug_PrintMarker(__FILE__,__className__,__func__,__LINE__);
#	else
#		define __MARK__		{};
#	endif
	ML_API	void mlDebug_printError(void* lm, const char* file, const char* className, const char* func, int line, t_mlError err, const char* condition);
#	define PRINT_ERROR(lm,err) \
		mlDebug_printError(lm,__FILE__,__className__,__func__,__LINE__,err,"");
#	define CHECK_CONDITION(lm,cond,errCode) {\
		if (!(cond)){\
			err=errCode;\
			mlDebug_printError(lm,__FILE__,__className__,__func__,__LINE__,err,#cond);\
			break;\
		}}
#	define ASSERT_CONDITION(cond) {\
		if (!(cond)){\
			mlDebug_printError(NULL,__FILE__,__className__,__func__,__LINE__,0xF16C,#cond);\
			assert(0);\
		}}
#	define CHECK_ERROR(lm,err) {\
		if (err!=kMLErr_OK){\
			mlDebug_printError(lm,__FILE__,__className__,__func__,__LINE__,err,"");\
			break;\
		}}
#if defined(__cplusplus)
}

#	pragma mark -
#	pragma mark C++ Interface
#include <string>
#include <vector>

	/*	 checksum, crc, hash	--------------------------------------------------------------------
	 */
#	pragma mark -
#	pragma mark checksum, crc, hash
	ML_API	uint64_t 	mlUtil_hashStr(const char *str, size_t size, int initChar = 0);
	ML_API	uint32_t	mlUtil_hashFnv32(const uint8_t* data, int length);
	ML_API	uint16_t	mlUtil_crc16(const uint8_t* data, size_t length);
	ML_API	uint16_t	mlUtil_crc16_rev(const uint8_t* data, size_t length);
	ML_API	uint32_t	mlUtil_checksum(char* ptr, size_t length);
#if defined(__APPLE__)
	std::string mlUtil_checksumMD5(const char* content, size_t len);
	std::string mlUtil_checksumSHA1(const char* content, size_t len);
	std::string mlUtil_checksumSHA256(const char* content, size_t len);
	std::string mlUtil_checksumSHA512(const char* content, size_t len);
	//
	t_mlError mlUtil_hmacHash(const char* algo,
							  const char* data,int dataLen,
							  const char* key, int keyLen,
							  char** ppSigbuf, int* pSigBufSize);
	//			description:
	//				Create HMAC Signature in Base64 Encoding
	//			parameter:
	//				algo		- Hash algorithms. Can be "MD5", "SHA1", "SHA256", "SHA384", "512", "SHA224"
	//				data		- data string
	//				dataLen		- data string length
	//				key			- key string
	//				keyLen		- key string length
	//				ppSigBuf	- pointer address to output signature
	//				pSigBufSize	- pointer to the output signature length
	//			return
	//				t_mlError	- MesaLibrary error code
#endif//__APPLE__

#if defined(__APPLE__)
	std::string mlUtil_GetUserAppCachePathStr(bool withBundleName = true, bool withBundleVer = true, bool withBundleIdentifier = true);
	std::string mlUtil_GetAppSupportPathStr(bool withBundleName = false, bool withBundleVer = false, bool withBundleIdentifier = true);
	std::string mlUtil_GetAppLogPathStr(bool withBundleName = false, bool withBundleVer = false, bool withBundleIdentifier = true);
#endif//__APPLE__

#if defined(__APPLE__)
std::string mlUtil_executePopen(const char* command);

t_mlError mlSys_enableSystemSleep(bool enableSleep);
#endif//__APPLE__
ML_API	std::string mlUtil_formatTimestampStr(double timestamp, int type = 0);
ML_API	std::string	mlUtil_getTimestampStr(int type = 0);
// type=0	YYYY-MM-dd HH:mm:ss
// type=1	YYYYMMddHHmmss

ML_API	uint16_t mlUtil_crc16(const std::vector<uint8_t> data);
ML_API	uint16_t mlUtil_crc16_rev(const std::vector<uint8_t> data);

ML_API	bool mlUtil_isEncrypted(std::string fn);
ML_API	t_mlError mlUtil_encrypt_file(std::string fn, std::string fn_out="");
ML_API	std::vector<uint8_t> mlUtil_decrypt_file(std::string fn);

ML_API 	std::string mlUtil_uuidString();
/***************************************************************************************************
 *	CML_fifo
 *	Description:	general purpose fifo 
 */
#	pragma mark -
#	pragma mark CML_fifo
#include <mutex>
#if defined(__APPLE__)
class CML_fifo:public CML_baseObject {
protected:
	std::mutex			_mtx;
	uint32_t			_elementSize;
	void*				_buff;
	void*				_begin;
	void*				_end;
	void*				_head;
	void*				_tail;
public:
	CML_fifo	(uint32_t elementCount = 256*1024, uint32_t elementSize = 1);
	//		description:
	//			constructor
	//		parameter:
	//			elementCount:	[in]	fifo buffer size (default 256K)
	//			elementSize:	[in]	indicates the data block size to write and read (default 1byte)
	//		[NOTE]
	//			when blkSize greater than 1, the write and read function will check if the data
	//			size for writing and reading is multiple of blkSize. 
	//			IF NOT, ASSERTION FAILURE occur.
	~CML_fifo	();
	t_mlError	setLogManager(CML_logManager* lm);
public:
	void		clear();
	//		description:
	//			clear the buffer
	t_mlError	write(const void* buff, uint32_t* pWriteSz);
	//		description:
	//			write data into the buffer
	//		parameter:
	//			buff:			[in]	data
	//			pWriteSz:		[in]	data size
	//							[out]	data written size 
	//		[NOTE 1]
	//			when blkSize greater than 1, the function will check if the data
	//			size (*pWriteSz) is multiple of blkSize
	//			IF NOT, ASSERTION FAILURE occur.
	//		[NOTE 2]
	//			when the free space in FIFO is less than *pWriteSz, the write action will happen
	//			partially.
	uint32_t	dataSize();		
	//		description:
	//			returns the data size stored in the FIFO (in bytes)
	t_mlError	read(void* buff, uint32_t* pReadSz);
	//		description:
	//			read data fom the buffer
	//		parameter:
	//			buff:			[in]	data
	//			pReadSz:		[in]	read buffer size
	//							[out]	data size that is read
	//		[NOTE]
	//			when blkSize greater than 1, the function will check if the data
	//			size (*pReadSz) is multiple of blkSize
	//			IF NOT, ASSERTION FAILURE occur.
};
#endif//__APPLE__
#endif//defined(__cplusplus)
#endif//ifndef _ML_UTILS_H_
