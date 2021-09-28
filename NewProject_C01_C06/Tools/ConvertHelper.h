#pragma once

/***************************************************************
 * Purpose:   类型转换
 **************************************************************/

#include <string>
#include "Commonlib_Global.h"

class COMMONLIB_EXPORT ConvertHelper
{
public:
    template <class T>  //支持int unsigned int float double bool long longlong string
    static T Convert(const std::string& name, T t = T())
    {
        return ConvertValue(name, t);
    }

    template <typename T>  //支持int unsigned int float double bool long longlong string
    static std::string ConvertToStr(const T& value)
    {
        return ConvertValue_(value);
    }

private:
    static int ConvertValue(const std::string& value, int defaultVal);
    static unsigned int ConvertValue(const std::string& value, unsigned int defaultVal);
    static float ConvertValue(const std::string& value, float defaultVal);
    static double ConvertValue(const std::string& value, double defaultVal);
    static bool ConvertValue(const std::string& value, bool defaultVal);
    static long ConvertValue(const std::string& value, long defaultVal);
    static long long ConvertValue(const std::string& value, long long defaultVal);
    static std::string ConvertValue(const std::string& value, std::string defaultVal);

    static std::string ConvertValue_(const std::string& value);
    static std::string ConvertValue_(const char* value);
    static std::string ConvertValue_(const int value);
    static std::string ConvertValue_(const unsigned int value);
    static std::string ConvertValue_(const float value);
    static std::string ConvertValue_(const double value);
    static std::string ConvertValue_(const bool value);
    static std::string ConvertValue_(const long value);
    static std::string ConvertValue_(const long long value);

private:
    static bool IsInt(const std::string& src);
    static bool IsHex(const char* src);
    static bool IsDouble(const char* src);
    static int IsBool(const char* src);  // src=true return 1; src=false return 0; else return -1;
private:
    ConvertHelper() {}
};
