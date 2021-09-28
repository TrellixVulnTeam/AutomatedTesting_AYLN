//
//  ml_typedef.h
//
//  Created by Joon Kwon on 8/27/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_TYPEDEF_H_
#define _ML_TYPEDEF_H_

#if defined(__APPLE__)
#	include <CoreFoundation/CoreFoundation.h>
#	include <CoreGraphics/CoreGraphics.h>
#	include <IOKit/IOKitLib.h>
#if defined(__OBJC__)
#	include <Foundation/Foundation.h>
#endif
#endif

#if defined(_MSC_VER)
#define ML_API __declspec(dllexport) 
#define ML_API_DEP(msg) __declspec(dllexport,deprecated(msg))
#define DEPRECATED(msg)
#define DEPRECATED_F(func, msg) __declspec(dllexport,deprecated(msg)) func
#define DEPRECATED_C(func, msg) __declspec(deprecated(msg)) func
#else
#define ML_API
#define ML_API_DEP(msg)
#define DEPRECATED(msg)	__attribute__((deprecated(msg)))
#define DEPRECATED_F(func, msg) func __attribute__((deprecated(msg)))
#define DEPRECATED_C(func, msg) func __attribute__((deprecated(msg)))
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
//#include <getopt.h>
#include <assert.h>
#include <typeinfo>

#if defined(__APPLE__)
#include <pthread.h>
#include <unistd.h>
#include <AssertMacros.h>
#include <dirent.h>
#endif

#define ML_STATIC_ASSERT(constant_expression) \
	static_assert(constant_expression, #constant_expression)

// suppress compiler warnings
#pragma GCC diagnostic ignored "-Wwrite-strings" //suppress "warning: deprecated conversion from string constant to ‘char*’"
#pragma GCC diagnostic ignored "-Wstring-plus-int" //suppress "warning: adding 'int' to a string does not append to the string

/*! @abstract Get the MesaLibrary Framework Version */
ML_API const char* const MesaLibrary_GetVersion(void);

/*	type definitions
 */
#define	_NA_ML_					((float)-1.754941e-39)	// Use this when NO_DATA needed. To differentiate with system NAN
#define _IS_NA_(val)			((float)val==_NA_ML_)	// Use this method to check if a number is NO_DATA
#define _IS_VALID_NUMBER_(val)	(!isinf(val) && !isnan(val) && !_IS_NA_(val))
#define _TMP_DIR_				"/private/tmp/"
/*	Back support, previously _NA_ML_ was defined as "NA", but system math header has NA definition as well
 */
#ifndef NA
#define NA	_NA_ML_
#endif

typedef enum {
	k_mlDataType_uint8,
	k_mlDataType_sint8,
	k_mlDataType_uint16,
	k_mlDataType_sint16,
	k_mlDataType_uint32,
	k_mlDataType_sint32,
	k_mlDataType_uint64,
	k_mlDataType_sint64,
	k_mlDataType_float,
	k_mlDataType_double,
	k_mlDataType_string,
	//
	k_mlDataType_MAX,
	k_mlDataType_unknown = k_mlDataType_MAX,
} t_mlDataType;

static const size_t k_mlDataSize[] = {
	sizeof(uint8_t),	//k_mlDataType_uint8,
	sizeof(int8_t),		//k_mlDataType_sint8,
	sizeof(uint16_t),	//k_mlDataType_uint16,
	sizeof(int16_t),	//k_mlDataType_sint16,
	sizeof(uint32_t),	//k_mlDataType_uint32,
	sizeof(int32_t),	//k_mlDataType_sint32,
	sizeof(uint64_t),	//k_mlDataType_uint64,
	sizeof(int64_t),	//k_mlDataType_sint64,
	sizeof(float),		//k_mlDataType_float,
	sizeof(double),		//k_mlDataType_double,
	0,	//k_mlDataType_string,
};

#define k_mlDataItemStrLengMax 256
typedef struct t_mlDataItem{
	const char*			title;
	t_mlDataType		dataType;
	double				lowerLimit;
	double				upperLimit;
	double				value;
	char				strValue[k_mlDataItemStrLengMax];
} t_mlDataItem;

// type definitions & constants
#pragma mark -
#pragma macro type definitions
typedef uint16_t t_mlError;
static const char* const __className__ = 0;	// dummy class string for non-class namespace

// macros
#pragma mark -
#pragma macros
#if !defined(msleep)
#	define msleep(ms) usleep(1000*(ms))
#endif

#if !defined(FREE_MEM)
#	define FREE_MEM(ptr) {if(ptr) free(ptr); ptr=NULL;}
#endif

#if !defined(CLOSE_FILE)
#	define CLOSE_FILE(fp) {if(fp){fflush(fp); fclose(fp);} fp=NULL;}
#endif

#if defined(__cplusplus)
#if !defined(DELETE_INSTANCE)
#	define DELETE_INSTANCE(obj) {if(obj!=NULL) delete obj; obj=NULL;}
#endif
#if !defined(DELETE_INSTANCES)
#	define DELETE_INSTANCES(obj_array,size) {\
	if (obj_array!=NULL){\
		for(int i=0; i<size; i++) if (obj_array[i]!=NULL) delete obj_array[i];\
		delete[] obj_array; obj_array = NULL;}}
#endif
#endif//defined(__cplusplus)

#if defined(__cplusplus)
#include <string>
#include <vector>
static const std::vector<std::string> kMlDataTypeStr = {
	"uint8_t",
	"int8_t",
	"uint16_t",
	"int16_t",
	"uint32_t",
	"int32_t",
	"uint64_t",
	"int64_t",
	"float",
	"double",
	"string",
};

static inline const char* ml_getTypeStr(size_t typeHashCode){
	const char* rtn = "unknown";
		 if (typeHashCode==typeid(uint8_t		).hash_code())	rtn = "uint8_t"	;
	else if (typeHashCode==typeid(uint16_t		).hash_code())	rtn = "uint16_t";
	else if (typeHashCode==typeid(uint32_t		).hash_code())	rtn = "uint32_t";
	else if (typeHashCode==typeid(uint64_t		).hash_code())	rtn = "uint64_t";
	else if (typeHashCode==typeid(int8_t		).hash_code())	rtn = "int8_t"	;
	else if (typeHashCode==typeid(int16_t		).hash_code())	rtn = "int16_t"	;
	else if (typeHashCode==typeid(int32_t		).hash_code())	rtn = "int32_t"	;
	else if (typeHashCode==typeid(int64_t		).hash_code())	rtn = "int64_t"	;
	else if (typeHashCode==typeid(float			).hash_code())	rtn = "float"	;
	else if (typeHashCode==typeid(double		).hash_code())	rtn = "double"	;
	else if (typeHashCode==typeid(std::string	).hash_code())	rtn = "string"	;
	return rtn;
}
#define ML_GET_TYPE_STR(type) ml_getTypeStr(typeid(type).hash_code())

// macros
#pragma mark -
#pragma C++ specific macros
#define kMl_classNameLengthMax	128
#define ML_SET_CLASS_NAME {\
/**/const char* cl = typeid(this).name();\
/**/bool isNumSeen = false;\
/**/while(1){\
/**/	if (!isNumSeen) {if (*cl<'0'||'9'<*cl) cl++;	else isNumSeen=true;}\
/**/	else			{if (*cl<'0'||'9'<*cl) break;	else cl++;			}}\
/**/strncpy(__className__,cl,kMl_classNameLengthMax-1);}

/*
 *	CML_baseObject
 *	the base class of all Mesa Library classes
 */
class CML_logManager;	// forward declaration;log manager 
#define __valid_mark__ 'vald'
#define INSTANCE_VALID (this!=NULL && this->_mark_==__valid_mark__)
class ML_API __CML_OBJECT_BASIC__ {
protected:
	char __className__[kMl_classNameLengthMax];
	uint32_t _mark_;
public:
	__CML_OBJECT_BASIC__():_mark_(__valid_mark__){};
	virtual ~__CML_OBJECT_BASIC__(){if(!INSTANCE_VALID) return; _mark_=0;__className__[0]=0;};
	//
	const char* getClassName(){return __className__;};
	bool isClassOf(const char* className){return INSTANCE_VALID?strncmp(__className__, className, sizeof(__className__))==0:false;};
	bool isInstanceValid(){return INSTANCE_VALID;};
};
class ML_API CML_baseObject: public __CML_OBJECT_BASIC__ {
protected:
	CML_logManager* _lm;
public:
	CML_baseObject();
	virtual ~CML_baseObject(){};
	//
	t_mlError setLogManager(CML_logManager* lm);
	CML_logManager* getLogManager();
};
#define ML_DEFAULT_SET_LOG_MGR(cls) \
t_mlError cls::setLogManager(CML_logManager* lm){\
/**/if (lm==NULL){_lm = NULL;}\
/**/else{\
/**/	if(!lm->isInstanceValid()) return kMLErr_invalidPointer;\
/**/	if(!INSTANCE_VALID) return kMLErr_objectNotAvailable;\
/**/	if(strcmp(lm->getClassName(),"CML_logManager")) return kMLErr_invalidPointer;\
/**/	_lm = lm;\
/**/}\
/**/	return kMLErr_OK;\
/**/}

#endif//defined(__cplusplus)

#if defined(__OBJC__) && !TARGET_OS_IPHONE
@protocol mlObjc_protocol_viewControllerAppEvent
//
@required
- (void)viewActivated;
- (void)viewDeactivated;
- (void)applicationWillTerminate:(NSNotification *)notification;
//
@optional
- (void) OnMenu_command_up:(id)sender;
- (void) OnMenu_command_down:(id)sender;
- (void) OnMenu_command_bracket_left:(id)sender;
- (void) OnMenu_command_bracket_right:(id)sender;
- (void) OnMenu_command_shift_c:(id)sender;
@end
#endif//defined(__OBJC__) && !TARGET_OS_IPHONE

#endif//ifndef _ML_TYPEDEF_H_
