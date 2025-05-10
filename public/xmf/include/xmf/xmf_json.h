/**
 * @file xmf_json.h
 * @brief JSON解析和生成工具
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

#include "xmf/xmf_object.h"

namespace library
{
    namespace xmf
    {
        class XMF_EXPORT XmfJson
        {
        public:
            /**
             * @brief 从文件中读取JSON数据
             * @param fileName 文件名
             * @return XmfValuePtr 返回值解析后的Xmf结构，失败返回空，错误信息通过GetErrorMsg获取
             */
            XmfValuePtr Read(const char *fileName);

            /**
             * @brief 从流中读取JSON数据
             * @param in 输入流
             * @return XmfValuePtr 返回值解析后的Xmf结构，失败返回空，错误信息通过GetErrorMsg获取
             */
            XmfValuePtr Read(std::istream &in);

            /**
             * @brief 将Xmf结构转换成JSON输出到文件
             * @param fileName 文件名
             * @param valuePtr Xmf结构数据
             * @param pretty 是否进行美化，即在输出中加入缩进及换行
             * @return true 成功
             * @return false 失败，错误信息通过GetErrorMsg获取
             */
            bool Write(const char *fileName, XmfValuePtr valuePtr, bool pretty = true);

            /**
             * @brief 将Xmf结构转换成JSON输出到流
             * @param out 输出流
             * @param valuePtr Xmf结构数据
             * @param pretty 是否进行美化，即在输出中加入缩进及换行
             * @return true 成功
             * @return false 失败，错误信息通过GetErrorMsg获取
             */
            bool Write(std::ostream &out, XmfValuePtr valuePtr, bool pretty = true);

            /**
             * @brief 是否操作出错
             * @return true 是，错误信息通过GetErrorMsg获取
             * @return false 否
             */
            bool IsError() { return _errMsg.size() != 0; }

            /**
             * @brief 返回出错信息
             * @return const std::string& 出错信息
             */
            const std::string &GetErrorMsg() const { return _errMsg; }

        private:
            std::string _errMsg; // 出错信息
        };
    } // namespace xmf
} // namespace library
