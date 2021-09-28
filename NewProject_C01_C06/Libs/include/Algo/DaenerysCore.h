#pragma once

#if defined(_MSC_VER)
#define DCore_API __declspec(dllexport)   
#else
#define DCore_API
#endif

#include <stdlib.h>
#include <stdint.h>

#include "ml_error.h"
#include "ml_typedef.h"

#ifndef sprintf_s
#define sprintf_s snprintf
#endif

#ifndef printf_s
#define printf_s printf
#endif

#ifdef __APPLE__
#define _DC_PATH_DELI_	"/"
#else
#define _DC_PATH_DELI_	"\\"
#endif

#define MAX_FOM_NAME_LEN 64

/*
*	DaenerysCore_GetVersion
*	Get Library Version String
*/
DCore_API const char* const DaenerysCore_GetVersion(void);

/*
*	dcInitialize - Initialize DaenerysCore Library
*	Only run once at the beginning of the caller
*/
DCore_API t_mlError	dcInitialize(void* ctx);
DCore_API t_mlError dcInitializeSystem(void* ctx);
DCore_API t_mlError dcInitializeModule(void* ctx);
/*
*	dcRelease - Finish DaenerysCore Library
*	Only run once at the end of the caller
*/
DCore_API void		dcRelease();
DCore_API void		dcReleaseModule();
DCore_API void		dcReleaseSystem();

typedef int (*dcMainFcnType)(int, const char **);

DCore_API int dcRunMain(dcMainFcnType run_main, int argc, const char **argv);

