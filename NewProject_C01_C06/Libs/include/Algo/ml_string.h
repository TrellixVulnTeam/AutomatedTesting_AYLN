//
//  ml_string.h
//
//  Created by Joon Kwon on 8/27/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_STRING_H_
#define _ML_STRING_H_

#ifdef __ML_USE_AS_EMB__
#include <ml_typedef.h>
#else
#include <ml_typedef.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef _MSC_VER
#define strcasecmp _stricmp
#include <Shlwapi.h>
#define strcasestr StrStrIA
#endif
	
#pragma mark -
#pragma mark string tools
	ML_API	char*		mlStr_getSizeString(uint32_t size);
	ML_API	uint32_t	mlStr_checkTextCount(const char* ptr, const char* txt);
	ML_API	char*		mlStr_skipWord(const char* ptr);
	ML_API	char*		mlStr_skipSpace(const char* ptr);
	ML_API	char*		mlStr_goAfter(const char* ptr, const char* txt);
	ML_API	char*		mlStr_goNextWord(const char* ptr);
	ML_API	char*		mlStr_goNextLine(const char* ptr);
	DEPRECATED_F(t_mlError mlStr_getNextString(char* text, char* delimiter, char* textBuff), "Unsafe! use mlStr_getNextStringWithLimit");
	ML_API	t_mlError	mlStr_getNextStringWithLimit(char* text, char* delimiter, char* textBuff, int textBuffLen);
	ML_API	t_mlError	mlStr_getItemString(char* strLineTitle,char* strLineData,char* strDelimiter,char* strItemName,char* strBuff);
	ML_API	char*		mlStr_goLastText(const char* ptr, const char* txt);

	ML_API	void		mlStr_upperCaseString(char* ptr);
	ML_API	void		mlStr_lowerCaseString(char* ptr);

	ML_API	char*		mlStr_scanInt_1D(char* ptr, int* buff, int width);
	ML_API	char*		mlStr_scanInt_2D(char* ptr, int* buff, int width, int height, bool flip);
	ML_API	char*		mlStr_scanFloat_1D(char* ptr, float* buff, int width);
	ML_API	char*		mlStr_scanFloat_2D(char* ptr, float* buff, int width, int height, bool flip);
	//
#if defined(__cplusplus)
}
#include <string>
#include <vector>
ML_API	void mlStr_upperCaseString(std::string& str);
ML_API	void mlStr_lowerCaseString(std::string& str);
ML_API	void mlStr_replaceAll(std::string& str,const std::string subStrOld,const std::string& subStrNew);
ML_API	const char* mlStr_strstr(const std::string&, const std::string&);
ML_API	const char* mlStr_strcasestr(const std::string&, const std::string&);
ML_API	char* mlStr_goAfter(const std::string&, const std::string&);
ML_API	std::vector<std::string> mlStr_split(const std::string& inputStr, const std::string& delimitor);
ML_API	void mlStr_split(const char* inputStr, const char* delimitor, std::vector<std::string>& strGroup, bool preAlloc = false);
#endif

#endif//ifndef _ML_STRING_H_
