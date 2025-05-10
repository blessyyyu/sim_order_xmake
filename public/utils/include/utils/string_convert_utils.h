/**
 * @file string_convert_utils.h
 * @brief 字符串转换的模板函数（模板函数只能放在头文件中）
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

#include <codecvt>
#include <locale>
#include <sstream>
#include <string>

#include "utils/utils_export.h"

namespace library
{
    namespace utils
    {
        class chs_codecvt : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
        {
        public:
            chs_codecvt()
                : codecvt_byname("chs")
            {
            } // zh_CN.GBK or .936
        };

        class StringConvertUtils
        {
        public:
            /**
             * @brief 将指定类型转换为字符串
             * @tparam T 指定类型
             * @param value 指定类型值
             * @return const std::string 字符串表示
             */
            template <typename T>
            static const std::string ToString(T value)
            {
                std::ostringstream ostrm;
                ostrm << std::fixed << value;
                return ostrm.str();
            }

            /**
             * @brief 专门针对double类型单独处理，设置保留小数位
             * @param value double数值
             * @param precision 保留的小数位置
             * @return const std::string 字符串表示
             */
            static const std::string ToString(double value, int precision)
            {
                char formatBuf[32];
                char valueBuf[64];
                sprintf(formatBuf, "%%.%df", precision);
                sprintf(valueBuf, formatBuf, value);

                return valueBuf;
            }

            /**
             * @brief 将string转换成wstring
             * @param value string
             * @return const std::wstring wstring
             */
            static const std::wstring StringToWString(const std::string &value)
            {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                return converter.from_bytes(value);
            }

            /**
             * @brief 将wstring转换成string
             * @param value wstring
             * @return const std::string string
             */
            static const std::string WStringToString(const std::wstring &value)
            {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                return converter.to_bytes(value);
            }

            /**
             * @brief 将gbk转换成utf8
             * @param value gbk编码字符串
             * @return const std::string utf8编码字符串
             */
            static const std::string GbkToUtf8(const std::string &value)
            {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
                std::wstring_convert<chs_codecvt> gbk_cvt;
                return utf8_cvt.to_bytes(gbk_cvt.from_bytes(value));
            }

            /**
             * @brief 将utf8转换成gbk
             * @param value utf8编码字符串
             * @return const std::string gbk编码字符串
             */
            static const std::string Utf8ToGbk(const std::string &value)
            {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
                std::wstring_convert<chs_codecvt> gbk_cvt;
                return gbk_cvt.to_bytes(utf8_cvt.from_bytes(value));
            }

            /**
             * @brief 将字符串转换为指定类型
             * @tparam T 指定类型
             * @param str 字符串表示
             * @return T 指定类型值
             */
            template <typename T>
            static T StringTo(const std::string str)
            {
                std::stringstream strm;
                strm.str(str);

                T value;
                strm >> value;
                return value;
            }
        };
    } // namespace utils
} // namespace library
