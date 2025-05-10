/**
 * @file string_utils.h
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
#pragma once

#include <string>
#include <vector>

#include "utils/utils_export.h"

namespace library
{
    namespace utils
    {
        class UTILS_EXPORT StringUtils
        {
        public:
            /**
             * @brief 去除字符串前后空格（直接修改源串）
             * @param s 待处理字符串
             * @return std::string& 处理后字符串
             */
            static std::string &Trim(std::string &s);

            /**
             * @brief 去除字符串前后空格（直接修改源串）
             * @param s 待处理C格式字符串
             * @return char* 处理后C格式字符串
             */
            static char *Trim(char *s);

            /**
             * @brief 替换指定子串一次
             * @param str 源串
             * @param from 要替换的子串
             * @param to 替换成的子串
             * @return true 替换成功
             * @return false 没有要替换的子串
             */
            static bool ReplaceOne(std::string &str, const std::string &from, const std::string &to);

            /**
             * @brief 替换所有指定子串
             * @param str 源串
             * @param from 要替换的子串
             * @param to 替换成的子串
             */
            static void ReplaceAll(std::string &str, const std::string &from, const std::string &to);

            /**
             * @brief 将字符串转换成大写
             * @param s 待处理字符串
             * @return std::string& 处理后字符串
             */
            static const std::string ToUpper(const std::string s);

            /**
             * @brief 将字符串转换成小写
             * @param s 待处理字符串
             * @return std::string& 处理后字符串
             */
            static const std::string ToLower(const std::string s);

            /**
             * @brief 判断字符串是不是以指定子串开始
             * @param source 源字符串
             * @param subStr 子串
             * @param ignoreCase 是否忽略大小写，默认不忽略
             * @return true 是以子串开头
             * @return false 不是以子串开头
             */
            static bool StartsWith(const std::string source, const std::string subStr, bool ignoreCase = false);

            /**
             * @brief 判断字符串是不是以指定子串结束
             * @param source 源字符串
             * @param subStr 子串
             * @param ignoreCase 是否忽略大小写，默认不忽略
             * @return true 是以子串开头
             * @return false 不是以子串开头
             */
            static bool EndsWith(const std::string source, const std::string subStr, bool ignoreCase = false);

            /**
             * @brief 将字符串根据指定分隔串进行分割
             * @param src 待处理字符串
             * @param sprStr 分隔串
             * @return std::vector<std::string> 分割后的字符串数组
             */
            static std::vector<std::string> SplitEx(const std::string &src, const std::string sprStr);

            /**
             * @brief 将整数显示的日期转换成带分隔符的字符串（yyyy/mm/dd），忽略错误的日期
             * @param ymd 日期的整数表示
             * @param sprCh 分隔符
             * @return std::string 日期字符串
             */
            static std::string DateToString(int ymd, char sprCh = '/');

            /**
             * @brief 将整数显示的时分秒转换成带:的时间字符串（hh:mm:ss[.ms]），忽略错误的时间
             * @param hms 时间的整数表示
             * @param ms 是否包含毫秒
             * @return const std::string 时间字符串
             */
            static const std::string TimeToString(int hms, bool ms = false);
        };
    } // namespace utils
} // namespace library
