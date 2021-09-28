//
//  ml_log_def.h
//  MLCore
//
//  Created by Yida Zhang on 12/10/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef ml_log_def_h
#define ml_log_def_h

// typedef
/**
 	@brief An enum for different logging levels
 */
typedef enum {
	kVLEmergency 	= 0,	//! Log level for Emergency Error. Top Availability
	kVLAlarm 		= 1,	//! Log level for Alarming Error.
	kVLCritical 	= 2,	//! Log level for Critical Error. Default Availability
	kVLError		= 3,	//! Log level for Error
	kVLWarning		= 4,	//! Log level for Warning
	kVLNotice		= 5,	//! Log level for Notice
	kVLInfo			= 6,	//! Log level for Infomation
	kVLDebug		= 7,	//! Log level for Debugging
	kVLMax			,		//! SHOULD NOT USE. Internal use only
	kVLUnknown 		= -1,
} t_mlVbsLvl;

#define kVL0 		kVLEmergency
#define kVL1		kVLAlarm
#define kVL2		kVLCritical
#define kVL3		kVLError
#define kVL4		kVLWarning
#define kVL5		kVLNotice
#define kVL6		kVLInfo
#define kVL7		kVLDebug

inline const char* mlLog_getLevelStr(t_mlVbsLvl vLvl){
	switch(vLvl) {
		//case kVLEmergency:return "LOG_EMERG";
		case kVLAlarm:return "LOG_ALARM";
		case kVLCritical:return "LOG_CRITICAL";
		case kVLError:return "LOG_ERROR";
		case kVLWarning:return "LOG_WARNING";
		case kVLNotice:return "LOG_NOTICE";
		case kVLInfo:return "LOG_INFO";
		case kVLDebug:return "LOG_DEBUG";
		default:return "LOG_UNKNOWN";
	};
}

// Deprecated to avoid index overflow and memory leakage, use mlLog_getLevelStr instead
//static const char* const t_mlLogLvelStr[] = {
//	"LOG_EMERG",
//	"LOG_ALARM",
//	"LOG_CRITICAL",
//	"LOG_ERROR",
//	"LOG_WARNING",
//	"LOG_NOTICE",
//	"LOG_INFO",
//	"LOG_DEBUG",
//};

/**
 *	@brief The interface for logging relay
 *
 *	@discussion In some applications, the parent class needs to know the printed text of a log manager,
 *				for example display the logging info on GUI, or writing the logging to a different file.
 *				The parent class then should inherit this
 *				interface and put the text handling code insid ethe onLogRelay method
 @code
 // In declaration
 class A : IML_logRelay {
 private:
 	CML_logManager* lm;
 	void onLogRelay(const char* strRelay);
 };
 
 // In constructor source code
 A::A() {
 	lm = new CML_logManager();
 	lm->setLogRelay(this);
 }
 
 A::onLogRelay(const char* strRelay){
 	// Do something special
 }
 @endcode
 */
struct IML_logRelay {
	/**
	 	@brief A virtual function for inherited class to implement

	 	@param strRelay the text relayed from a linked log manager
	 
	 	@warning the string pointer might corrupt when passing between different threads.
				the log manager and the relay method should be in the same thread.
	 */
	virtual void onLogRelay(const char* strRelay) = 0;
};

#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 101200
#import <OS/log.h>
#define mlLog_sysLog(vLvl, format, ...) \
{switch(vLvl) {\
	case kVLAlarm:		{os_log_fault(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLCritical:	{os_log_fault(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLError:		{os_log_error(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLWarning:	{os_log(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLNotice:		{os_log(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLInfo:		{os_log_info(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	case kVLDebug:		{os_log_debug(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
	default:			{os_log(OS_LOG_DEFAULT, format, ##__VA_ARGS__);} break;\
};}
#else
ML_API void mlLog_sysLog(t_mlVbsLvl vLvl, const char* format, ...);
#endif

#endif /* ml_log_def_h */
