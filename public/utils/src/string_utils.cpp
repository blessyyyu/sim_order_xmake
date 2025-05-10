/**
 * @file string_utils.cpp
 * @brief 跟字符串相关的工具函数集合
 * @author
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-06-03</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#include "utils/string_utils.h"

#include <algorithm>
#include <cstring>

namespace library
{
    namespace utils
    {
        std::string &StringUtils::Trim(std::string &s)
        {
            if (s.empty())
            {
                return s;
            }

            s.erase(0, s.find_first_not_of("\t\n "));
            s.erase(s.find_last_not_of("\t\n ") + 1);
            return s;
        }

        char *StringUtils::Trim(char *s)
        {
            int len = strlen(s);
            // 去掉末尾空白
            while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' || s[len - 1] == '\n'))
            {
                s[len - 1] = '\0';
                len--;
            }

            // 计算串首空白字符数量
            int count = 0;
            while (len > count && (s[count] == ' ' || s[count] == '\t' || s[count] == '\n'))
            {
                count++;
            }

            // 向前移动，覆盖空白，长度包含末尾的NUL字符
            if (count > 0)
            {
                memcpy(s, s + count, len - count + 1);
            }

            return s;
        }

        bool StringUtils::ReplaceOne(std::string &str, const std::string &from, const std::string &to)
        {
            size_t startPos = str.find(from);
            if (startPos == std::string::npos)
                return false;
            str.replace(startPos, from.length(), to);
            return true;
        }

        void StringUtils::ReplaceAll(std::string &str, const std::string &from, const std::string &to)
        {
            if (from.empty())
            {
                return;
            }

            size_t startPos = 0;
            while ((startPos = str.find(from, startPos)) != std::string::npos)
            {
                str.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        }

        const std::string StringUtils::ToUpper(const std::string s)
        {
            std::string rst = s;
            std::transform(s.begin(), s.end(), rst.begin(), ::toupper);
            return s;
        }

        const std::string StringUtils::ToLower(const std::string s)
        {
            std::string rst = s;
            std::transform(s.begin(), s.end(), rst.begin(), ::tolower);
            return s;
        }

        bool StringUtils::StartsWith(const std::string source, const std::string subStr, bool ignoreCase)
        {
            std::string src = source;
            std::string sub = subStr;
            size_t srcSize = src.size();
            size_t subSize = sub.size();
            // 子串大于源串，直接返回即可
            if (srcSize < subSize)
            {
                return false;
            }

            // 若忽略大小写，先转换成小写后再比较
            if (ignoreCase)
            {
                std::transform(src.begin(), src.end(), src.begin(), ::tolower);
                std::transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
            }

            // 比较前subSize个字符
            return 0 == src.compare(0, subSize, sub, 0, subSize);
        }

        bool StringUtils::EndsWith(const std::string source, const std::string subStr, bool ignoreCase)
        {
            std::string src = source;
            std::string sub = subStr;
            size_t srcSize = src.size();
            size_t subSize = sub.size();
            // 子串大于源串，直接返回即可
            if (srcSize < subSize)
            {
                return false;
            }

            // 若忽略大小写，先转换成小写后再比较
            if (ignoreCase)
            {
                std::transform(src.begin(), src.end(), src.begin(), ::tolower);
                std::transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
            }

            // 比较后subSize个字符
            return 0 == src.compare(srcSize - subSize, subSize, sub, 0, subSize);
        }

        std::vector<std::string> StringUtils::SplitEx(const std::string &src, const std::string sprStr)
        {
            std::vector<std::string> strs;

            // 分割字符串的长度，这样就可以支持如“,,”多字符串的分隔符
            int sprStrLen = sprStr.size();
            int lastPosition = 0, index = -1;
            while (-1 != (index = src.find(sprStr, lastPosition)))
            {
                strs.push_back(src.substr(lastPosition, index - lastPosition));
                lastPosition = index + sprStrLen;
            }

            // 截取最后一个分隔符后的内容
            std::string lastString = src.substr(lastPosition);

            // 如果最后一个分隔符后还有内容就入队
            if (!lastString.empty())
                strs.push_back(lastString);

            return strs;
        }

        std::string StringUtils::DateToString(int ymd, char sprCh)
        {
            int nYear = ymd / 10000 % 10000; // 这里对10000取余，避免格式化时溢出
            int nMonth = ymd / 100 % 100;
            int nDay = ymd % 100;

            char cTimeString[11] = {0};
            sprintf(cTimeString, "%04d%c%02d%c%02d", nYear, sprCh, nMonth, sprCh, nDay);
            return cTimeString;
        }

        const std::string StringUtils::TimeToString(int hms, bool ms)
        {
            int nHour = hms / 10000 % 100; // 这里对100取余，避免格式化时溢出
            int nMin = hms / 100 % 100;
            int nSec = hms % 100;

            char cTimeString[9] = {0};
            sprintf(cTimeString, "%02d:%02d:%02d", nHour, nMin, nSec);
            return cTimeString;
        }
    } // namespace utils
} // namespace library
