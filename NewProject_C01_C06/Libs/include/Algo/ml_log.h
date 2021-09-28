/**
	@brief Basic Class for Logging
	@author Joon Kwon
	@author Yida Zhang
	@date 2014-08-27
	@copyright 2014 Apple. All rights reserved.
 */

#ifndef _ML_LOG_MANAGER_H_
#define _ML_LOG_MANAGER_H_
#include "ml_typedef.h"
#include <mutex>
#include <vector>

/** @addtogroup ml_log
 *  @{
 */

#ifdef __ML_USE_AS_EMB__
#include <ml_typedef.h>
#include <ml_log_def.h>
#else
#include <ml_typedef.h>
#include <ml_log_def.h>
#endif
	
/**
*	@brief The basic class for Log Manager
*
*	@discussion This class standardize the logging format with different predefined log level.
*				All MesaLibrary classes use this base class for logging purpose.
*/

#if defined(__cplusplus)
typedef class ML_API CML_logManager: public __CML_OBJECT_BASIC__ {
protected:
	t_mlVbsLvl						_vbsLvl;		//!< logging/displaying level
	t_mlVbsLvl						_vbsLvl_file;	//!< log file verbose level, should be level 1 or higher
	char*		 					_logFileName;	//!< log file name currently opened
	bool							_ownLogFp;
	bool							_logFpflush;
	std::shared_ptr<std::ofstream> 	_logFp;			//!< log file descriptor currently opened
	std::mutex			_mtx_str;					//!< mutex lock for stdout export
	std::mutex			_mtx_txt;					//!< mutex lock for log file export
	char*				_lineBuffer_str;			//!< line buffer for stdout export
	char*				_lineBuffer_txt;			//!< line buffer for log file export
	uint32_t			_logRelay_key;
	IML_logRelay* 		_logRelay;					//!< interface for the relay
	std::vector<std::weak_ptr<IML_logRelay> > _wplogRelay;	//!< flag to print timestamp for each line
	//
	bool				_printTimestamp;
	bool				_printTimestampMs;
public:
	CML_logManager();	//!< Constructor
	virtual ~CML_logManager();	//!< Deconstructor
	
	/**
	 	@brief DO NOT CALL setLogManager of a CML_logManager class!!!
	 	@warning CML_logManager does not accept external CML_logManager object, for log string relay check setLogRelay()
	 */
	t_mlError			setLogManager(CML_logManager* lm);
	/**
	 	@brief This method enables a timestamp print out in front of each line of logging
	 
	 	@discussion With this feature on, a timestamp format of [YYYY-MM-DD HH:MM:SS] will be added
	 	@code
	 		// For example, with feature on
	 		// 		[2018-05-02 22:58:52].bind socket: port=61806
	 		//	with feature off
	 		// 		.bind socket: port=61806
	 	@endcode

	 	@param enable	turn on or turn off this feature
		@param withMs	turn on timestamp print with ms
	 	@return	MesaLibrary t_mlError defined in ml_error.h
	 */
	t_mlError 			setTimestampPrint(bool enable, bool withMs = false);
	/**
	 	@brief This method set the verbose level for stdout export
	 
	 	@discussion Only the lines labelled logging level lower or equal to vLvl will be exported
	 	@param 	vLvl 		the maximum logging level to export in stdout
	 	@return	MesaLibrary t_mlError defined in ml_error.h
	 *	verbose level control
	 */
	t_mlError			setVerboseLevel(t_mlVbsLvl vLvl);
	/**
		@brief This method returns the current verbose level setting for stdout export
	 */
	t_mlVbsLvl			getVerboseLevel(void){return (!INSTANCE_VALID)?kVLUnknown:_vbsLvl;};
	/**
		@brief This method set the verbose level for file export
	 
	 	@discussion Only the lines labelled logging level lower or equal to vLvl will be exported
		@param 	vLvl the maximum logging level to export in the file
	 	@return	MesaLibrary t_mlError defined in ml_error.h
	 */
	t_mlError 			setVerboseLevel_file(t_mlVbsLvl vLvl);
	/**
	 	@brief This method returns the current verbose level setting for file export
	 */
	t_mlVbsLvl			getVerboseLevel_file(void){return (!INSTANCE_VALID)?kVLUnknown:_vbsLvl_file;};
	/**
		@brief This method turn ON and OFF the logging to file system
		@code
			//	Turn ON file logging
			lm->setLogFileName("newlogfile.log");
			//	Turn OFF file logging
			lm->setLogFileName(NULL);
		@endcode
	 
		@param filePathName	the log file name
        @param extFp external file descriptor
		@param doInstantFlush flush the file instantly after file write
		@return	MesaLibrary t_mlError defined in ml_error.h
	 */
    t_mlError            setLogFileName(const char* filePathName,
                                       std::shared_ptr<std::ofstream> extFp = NULL,
                                       bool doInstantFlush = true);
	/**
		@brief This method returns the current log file name
	 */
	std::string			getLogFileNameStr();
	DEPRECATED_C(const char* getLogFileName(), "Use getLogFileNameStr() instead to avoid potential memory leak");

	/**
        @brief This method print out the log WITHOUT line change

        @param vLvl        the logging level
         @param format    C style text content to be exported to log file and stdout
     */
	void printString(t_mlVbsLvl vLvl, const char* format, ...);
	/**
		@brief This method print out the log WITH line change

		@param vLvl	the logging level
		@param format	C style text content to be exported to log file and stdout
	 */
	void printText(t_mlVbsLvl vLvl, const char* format, ...);
	/**
		@brief This method print 1-D char array

		@param vLvl			logging level
		@param pData		data array pointer
	 	@param length		number of elements in data array
	 	@param flip			flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
	 	@param numFormat	C style text format for @b EACH number
	 	@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
				the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const char		*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint8_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const uint8_t	*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int8_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const int8_t		*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint16_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const uint16_t	*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int16_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const int16_t	*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint32_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const uint32_t	*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int32_t array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const int32_t	*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D float array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const float		*pData, int length, bool flip, char* numFormat);
	/**
		@brief This method print 1-D double array

		@param vLvl		logging level
		@param pData		data array pointer
		@param length		number of elements in data array
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_1d(t_mlVbsLvl vLvl, const double		*pData, int length, bool flip, char* numFormat);

	/**
		@brief This method print 1-D char array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const char		*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint8_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const uint8_t	*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int8_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const int8_t		*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint16_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const uint16_t	*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int16_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const int16_t	*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D uint32_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const uint32_t	*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D int32_t array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const int32_t	*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D float array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
		@param width	width of the output matrix
		@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const float		*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method print 1-D double array in a 2D Matrix format

		@param vLvl		logging level
		@param pData	data array pointer
		@param stride	MUST Input the same as width. Other stride NOT SUPPORTED
	 	@param width	width of the output matrix
	 	@param height	height of the output matrix
		@param flip		flag to print in back order. @b FALSE, from first to last, @b TRUE, from last to first
		@param numFormat	C style text format for @b EACH number
		@warning No invalid data pointer check or data length matching check. Caller is responsible to ensure
		the number array valid
	 */
	void print_2d(t_mlVbsLvl vLvl, const double		*pData, int stride, int width, int height, bool flip, char* numFormat);
	/**
		@brief This method setups the interface for logging relay

		@param logRelay	the relay method pointer in format of IML_logRelay
	 */
	DEPRECATED_C(void setLogRelay(IML_logRelay* logRelay), "Use setLogRelay with std::weak_ptr to avoid retain cycle");
	void setLogRelay(std::weak_ptr<IML_logRelay> logRelay);
} CML_logManager, *CML_logManagerPtr;

#endif//defined(__cplusplus)

/** @} */
#endif//ifndef _ML_LOG_MANAGER_H_
