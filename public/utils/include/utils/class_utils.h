/**
 * @file class_utils.h
 * @brief 类工具
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

#define MEMBER_NORMAL_RW(varType, varName, funcName) \
protected:                                           \
    varType varName;                                 \
                                                     \
public:                                              \
    void Set##funcName(varType var)                  \
    {                                                \
        varName = var;                               \
    }                                                \
                                                     \
public:                                              \
    varType Get##funcName(void)                      \
    {                                                \
        return varName;                              \
    }

#define MEMBER_NORMAL_WO(varType, varName, funcName) \
protected:                                           \
    varType varName;                                 \
                                                     \
public:                                              \
    void Set##funcName(varType var)                  \
    {                                                \
        varName = var;                               \
    }

#define MEMBER_NORMAL_RO(varType, varName, funcName) \
protected:                                           \
    varType varName;                                 \
                                                     \
public:                                              \
    varType Get##funcName(void)                      \
    {                                                \
        return varName;                              \
    }

#define MEMBER_REF_RW(varType, varName, funcName) \
protected:                                        \
    varType varName;                              \
                                                  \
public:                                           \
    void Set##funcName(const varType &var)        \
    {                                             \
        varName = var;                            \
    }                                             \
                                                  \
public:                                           \
    const varType &Get##funcName(void)            \
    {                                             \
        return varName;                           \
    }

#define MEMBER_REF_WO(varType, varName, funcName) \
protected:                                        \
    varType varName;                              \
                                                  \
public:                                           \
    void Set##funcName(const varType &var)        \
    {                                             \
        varName = var;                            \
    }

#define MEMBER_REF_RO(varType, varName, funcName) \
protected:                                        \
    varType varName;                              \
                                                  \
public:                                           \
    const varType &Get##funcName(void)            \
    {                                             \
        return varName;                           \
    }
