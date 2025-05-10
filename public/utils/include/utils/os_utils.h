/**
 * @file shm_utils.h
 * @brief 共享内存工具
 * @author
 * @date 2022-08-07
 *
 * @copyright Copyright (c) 2022
 *
 * @par 修改日志:
 * <table>
 * <tr> <th>日期</th>       <th>作者</th> <th>修改说明</th> </tr>
 * <tr> <td>2022-08-07</td> <td></td> <td>初始创建</td> </tr>
 * </table>
 */
#pragma once

#include <cstdint>
#include <string>

#include "utils/utils_export.h"

namespace library
{
    namespace utils
    {
        class UTILS_EXPORT os
        {
        public:
            /**
             * @brief 加载内存映射
             * @param path 文件路径
             * @param size 文件大小
             * @param lazy 当writable=false并且lazy=true时，内存映射文件使用延迟加载（非预加载），主要是用于页查询
             * @param readOnly 是否以只读映射
             * @return uintptr_t 内存映射地址
             */
            static uintptr_t LoadMmapBuffer(const std::string &path, size_t size, bool lazy, bool readOnly = false);

            /**
             * @brief 释放共享内存
             * @param address 内存映射地址
             * @param size 内存大小
             * @param lazy 当writable=false并且lazy=true时，内存映射文件使用延迟加载（非预加载），主要是用于页查询
             * @return true 成功
             * @return false 失败
             */
            static bool ReleaseMmapBuffer(uintptr_t address, size_t size, bool lazy);

            /**
             * @brief 顺序化指令
             */
            static inline void Serialize()
            {
                uint32_t eax, ebx, ecx, edx;
                __asm__ volatile("cpuid"
                                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                                 : "a"(1U));
            }
        };
    } // namespace utils
} // namespace library
