#pragma once

/***************************************************************
 * Purpose:   日志封装宏
 **************************************************************/

#include "Commonlib_Global.h"
#include "Util.h"
#include <log4cplus/fileappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

COMMONLIB_EXPORT void UNINT_LOG();

COMMONLIB_EXPORT void INIT_LOG(const std::string& path, int type = 0);
COMMONLIB_EXPORT void INIT_LOG_NAME(const std::string& name, const std::string& path, int type = 0);

COMMONLIB_EXPORT void SET_LOGLEVEL(int level);
COMMONLIB_EXPORT void SET_LOGLEVEL_NAME(const std::string& name, int level);

COMMONLIB_EXPORT void LOG_DEBUG(const char* msg, ...);

COMMONLIB_EXPORT void LOG_INFO(const char* msg, ...);

COMMONLIB_EXPORT void LOG_WARN(const char* msg, ...);

COMMONLIB_EXPORT void LOG_ERROR(const char* msg, ...);

COMMONLIB_EXPORT void LOG_FATAL(const char* msg, ...);

COMMONLIB_EXPORT void LOG_DEBUG_NAME(const char* name, const char* msg, ...);

COMMONLIB_EXPORT void LOG_INFO_NAME(const char* name, const char* msg, ...);

COMMONLIB_EXPORT void LOG_WARN_NAME(const char* name, const char* msg, ...);

COMMONLIB_EXPORT void LOG_ERROR_NAME(const char* name, const char* msg, ...);

COMMONLIB_EXPORT void LOG_FATAL_NAME(const char* name, const char* msg, ...);

// log file and print
COMMONLIB_EXPORT void LOG_PRINT(const char* msg, ...);
