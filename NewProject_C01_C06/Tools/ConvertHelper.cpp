#include "ConvertHelper.h"
#include <stdio.h>
#include <stdlib.h>

bool ConvertHelper::IsInt(const std::string& src)
{
    std::string tmpStr = src;
    tmpStr.erase(0, src.find_first_not_of(" "));
    tmpStr.erase(tmpStr.find_last_not_of(" ") + 1);

    char* loopSrc = (char*)tmpStr.c_str();
    while (*loopSrc != '\0') {
        if (isdigit(*loopSrc) == 0) {
            //判断是否是第一位，且为-号，负整数
            if (loopSrc == tmpStr.c_str() && *loopSrc == '-') {
                ++loopSrc;
                if (*loopSrc == '\0')  //判断下一位是否为\0
                    return false;      //输入字符串是"-"
                else
                    continue;
            }

            return false;
        }

        ++loopSrc;
    }

    return true;
}

bool ConvertHelper::IsHex(const char* src)
{
    char* loopSrc = (char*)src;
    std::string str(src);

    //先找出是否具有0x或者0X这样的16进制前缀(其实也可以不带的，所以要判断下)
    static const char* ch = "0x";
    static const char* ch1 = "0X";
    int pos = -1;
    pos = str.find(ch);
    if (pos >= 0) {
        if (pos != 0 && src[0] != '-')  //判断负数-0x
            return false;               //必须是0x开头的，不能是000x开头或者0x在中间
        else
            loopSrc = loopSrc + (src[0] != '-' ? 2 : 3);
    } else {
        pos = str.find(ch1);
        if (pos >= 0) {
            if (pos != 0 && src[0] != '-')  //判断负数-0x
                return false;               //必须是0x开头的，不能是000x开头或者0x在中间
            else
                loopSrc = loopSrc + (src[0] != '-' ? 2 : 3);
        } else {
            //不是0x或者0X开头的
        }
    }

    while (*loopSrc != '\0') {
        if (isxdigit(*loopSrc) == 0)  //判断是否16进制数
        {
            return false;
        }

        ++loopSrc;
    }

    return true;
}

bool ConvertHelper::IsDouble(const char* src)
{
    bool flag = false;
    char* loopSrc = (char*)src;
    if (strlen(src) >= 2) {
        if (*loopSrc == '0' && *(loopSrc + 1) != '.') {  //第一位不能是0,并且后面不是跟的小数点
            return false;
        }
    }

    while (*loopSrc != '\0') {
        if (isdigit(*loopSrc) == 0) {
            //判断是否是第一位，且为-号，负整数
            if (loopSrc == src && *loopSrc == '-') {
                ++loopSrc;
                if (*loopSrc == '\0')  //判断下一位是否为\0
                    return false;      //输入字符串是"-"
                else
                    continue;
            } else {  //判断是否小数点号
                if (*loopSrc == '.') {
                    if (flag)
                        return false;  //出现2次小数点，错误

                    flag = true;
                    ++loopSrc;
                    continue;
                }
            }

            return false;
        }

        ++loopSrc;
    }

    return true;
}

int ConvertHelper::IsBool(const char* src)
{
#ifdef _WINDOWS
    if (0 == _stricmp(src, "true"))
        return 1;
    else if (0 == _stricmp(src, "false"))
        return 0;
#else
    if (0 == strcasecmp(src, "true"))
        return 1;
    else if (0 == strcasecmp(src, "false"))
        return 0;
#endif
    return -1;
}

int ConvertHelper::ConvertValue(const std::string& value, int defaultVal)
{
    if (!IsInt(value.c_str())) {
        if (!IsHex(value.c_str()))
            return defaultVal;

        return strtoul(value.c_str(), NULL, 16);
    }

    return atoi(value.c_str());
}

unsigned int ConvertHelper::ConvertValue(const std::string& value, unsigned int defaultVal)
{
    if (!IsInt(value.c_str())) {
        if (!IsHex(value.c_str()))
            return defaultVal;

        return strtoul(value.c_str(), NULL, 16);
    }

    if (value[0] == '-')
        return defaultVal;
    return strtoul(value.c_str(), NULL, 10);
}

float ConvertHelper::ConvertValue(const std::string& value, float defaultVal)
{
    if (!IsDouble(value.c_str()))
        return defaultVal;

    return (float)(atof(value.c_str()));
}

double ConvertHelper::ConvertValue(const std::string& value, double defaultVal)
{
    if (!IsDouble(value.c_str()))
        return defaultVal;

    return atof(value.c_str());
}

bool ConvertHelper::ConvertValue(const std::string& value, bool defaultVal)
{
    int ret = IsBool(value.c_str());
    if (-1 == ret)
        return defaultVal;

    return (bool)(ret);
}

long ConvertHelper::ConvertValue(const std::string& value, long defaultVal)
{
    if (!IsInt(value.c_str())) {
        if (!IsHex(value.c_str()))
            return defaultVal;

        return strtoul(value.c_str(), NULL, 16);
    }

    return atol(value.c_str());
}

long long ConvertHelper::ConvertValue(const std::string& value, long long defaultVal)
{
    if (!IsInt(value.c_str())) {
        if (!IsHex(value.c_str()))
            return defaultVal;

        return strtoul(value.c_str(), NULL, 16);
    }

    return atoll(value.c_str());
}

std::string ConvertHelper::ConvertValue(const std::string& value, std::string defaultVal)
{
    return value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
std::string ConvertHelper::ConvertValue_(const std::string& value)
{
    return value;
}

std::string ConvertHelper::ConvertValue_(const char* value)
{
    return std::string(value);
}

std::string ConvertHelper::ConvertValue_(const int value)
{
    char szBuf[32] = { 0 };
    sprintf(szBuf, "%d", value);

    return std::string(szBuf);
}

std::string ConvertHelper::ConvertValue_(const unsigned int value)
{
    return ConvertValue_((int)value);
}

std::string ConvertHelper::ConvertValue_(const float value)
{
    char szBuf[64] = { 0 };
    sprintf(szBuf, "%f", value);
    return std::string(szBuf);
}

std::string ConvertHelper::ConvertValue_(const double value)
{
    char szBuf[64] = { 0 };
    sprintf(szBuf, "%lf", value);
    return std::string(szBuf);
}

std::string ConvertHelper::ConvertValue_(const bool value)
{
    return value ? "TRUE" : "FALSE";
}

std::string ConvertHelper::ConvertValue_(const long value)
{
    char szBuf[32] = { 0 };
    sprintf(szBuf, "%ld", value);
    return std::string(szBuf);
}

std::string ConvertHelper::ConvertValue_(const long long value)
{
    char szBuf[64] = { 0 };
    sprintf(szBuf, "%lld", value);
    return std::string(szBuf);
}
