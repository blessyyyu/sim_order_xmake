/**
 * @file endian_convert.h
 * @brief 大小端转换类
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

inline bool IsBigEndian()
{
    unsigned short test = 0x1122;
    if (*((unsigned char *)&test) == 0x11)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
T Host2Net(T value)
{
    if (IsBigEndian())
    {
        return value;
    }

    int size = sizeof(T);
    T tmp;
    char *buf = (char *)&tmp;
    for (int i = 0; i < size; i++)
    {
        buf[i] = (char)(value >> ((size - 1 - i) * 8));
    }

    return tmp;
}

template <typename T>
T Net2Host(T value)
{
    if (IsBigEndian())
    {
        return value;
    }

    int size = sizeof(T);
    T tmp;
    char *buf = (char *)&tmp;
    for (int i = 0; i < size; i++)
    {
        buf[i] = (char)(value >> ((size - 1 - i) * 8));
    }

    return tmp;
}
