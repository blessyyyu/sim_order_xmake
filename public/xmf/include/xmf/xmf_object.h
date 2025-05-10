/**
 * @file xmf_object.h
 * @brief 跨平台消息内存格式，用于通用参数传递
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

#include <cstdio>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "utils/exception_utils.h"
#include "utils/string_convert_utils.h"
#include "utils/string_utils.h"
#include "xmf/xmf_export.h"

namespace library
{
    namespace xmf
    {
        enum XmfType
        {
            XMF_NULL = 0,
            XMF_BOOL,
            XMF_INT,
            XMF_UINT,
            XMF_INT64,
            XMF_UINT64,
            XMF_DOUBLE,
            XMF_STRING,
            XMF_OBJECT,
            XMF_ARRAY,
        };

        class XmfValue;
        class XmfNull;
        class XmfBool;
        class XmfInt;
        class XmfUInt;
        class XmfInt64;
        class XmfUInt64;
        class XmfDouble;
        class XmfString;
        class XmfArray;
        class XmfObject;
        class XmfIterator;

        typedef std::shared_ptr<XmfValue> XmfValuePtr;
        typedef std::shared_ptr<XmfNull> XmfNullPtr;
        typedef std::shared_ptr<XmfBool> XmfBoolPtr;
        typedef std::shared_ptr<XmfInt> XmfIntPtr;
        typedef std::shared_ptr<XmfUInt> XmfUIntPtr;
        typedef std::shared_ptr<XmfInt64> XmfInt64Ptr;
        typedef std::shared_ptr<XmfUInt64> XmfUInt64Ptr;
        typedef std::shared_ptr<XmfDouble> XmfDoublePtr;
        typedef std::shared_ptr<XmfString> XmfStringPtr;
        typedef std::vector<XmfValuePtr> XmfValueArray;
        typedef std::map<std::string, XmfValuePtr> XmfValueMap;
        typedef std::shared_ptr<XmfArray> XmfArrayPtr;
        typedef std::shared_ptr<XmfObject> XmfObjectPtr;
        typedef std::shared_ptr<XmfIterator> XmfIteratorPtr;

        class XMF_EXPORT XmfIterator
        {
        public:
            virtual void MoveFirst() = 0;
            virtual void MoveNext() = 0;
            virtual bool IsEof() = 0;
            virtual const std::string GetKey() = 0;
            virtual XmfValuePtr GetValue() = 0;
        };

        class XMF_EXPORT XmfValue
        {
        public:
            XmfValue() {}
            virtual ~XmfValue(void) {}

            virtual XmfType DataType() = 0;
            virtual size_t GetChildCount() { return 0; }
            virtual bool ToBool() { throw library::utils::Exception("can't convert to bool"); }
            virtual int ToInt() { throw library::utils::Exception("can't convert to int"); }
            virtual uint ToUInt() { throw library::utils::Exception("can't convert to unsigned int"); }
            virtual int64_t ToInt64() { throw library::utils::Exception("can't convert to int64"); }
            virtual uint64_t ToUInt64() { throw library::utils::Exception("can't convert to unsigned int64"); }
            virtual double ToDouble() { throw library::utils::Exception("can't convert to double"); }
            virtual std::string ToString() { throw library::utils::Exception("can't convert to string"); }
            virtual XmfValuePtr Item(size_t index) { throw library::utils::Exception("not support operation Item[i]"); }
            virtual XmfValuePtr Item(const char *key) { throw library::utils::Exception("not support operation Item['key']"); }
            virtual XmfValuePtr Item(const std::string &key) { return Item(key.c_str()); }
            virtual XmfIteratorPtr GetChildIterator();
        };

        class XMF_EXPORT XmfNull : public XmfValue
        {
        public:
            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_NULL; }

            static XmfValuePtr Null;
        };

        class XMF_EXPORT XmfBool : public XmfValue
        {
        public:
            XmfBool(bool value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(bool value) { _value = value; }
            bool GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_BOOL; }

            // 类型转换
            virtual bool ToBool() { return _value; }
            virtual int ToInt() { return _value ? 1 : 0; }
            virtual uint ToUInt() { return _value ? 1 : 0; }
            virtual int64_t ToInt64() { return _value ? 1 : 0; }
            virtual uint64_t ToUInt64() { return _value ? 1 : 0; }
            virtual double ToDouble() { return _value ? 1 : 0; }
            virtual std::string ToString() { return _value ? "true" : "false"; }

        private:
            bool _value;
        };

        class XMF_EXPORT XmfInt : public XmfValue
        {
        public:
            XmfInt(int value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(int value) { _value = value; }
            int GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_INT; }

            // 类型转换
            virtual bool ToBool() { return _value != 0; }
            virtual int ToInt() { return _value; }
            virtual uint ToUInt() { return _value; }
            virtual int64_t ToInt64() { return _value; }
            virtual uint64_t ToUInt64() { return _value; }
            virtual double ToDouble() { return _value; }
            virtual std::string ToString() { return library::utils::StringConvertUtils::ToString(_value); }

        private:
            int _value;
        };

        class XMF_EXPORT XmfUInt : public XmfValue
        {
        public:
            XmfUInt(uint value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(uint value) { _value = value; }
            uint GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_UINT; }

            // 类型转换
            virtual bool ToBool() { return _value != 0; }
            virtual int ToInt() { return _value; }
            virtual uint ToUInt() { return _value; }
            virtual int64_t ToInt64() { return _value; }
            virtual uint64_t ToUInt64() { return _value; }
            virtual double ToDouble() { return _value; }
            virtual std::string ToString() { return library::utils::StringConvertUtils::ToString(_value); }

        private:
            uint _value;
        };

        class XMF_EXPORT XmfInt64 : public XmfValue
        {
        public:
            XmfInt64(int64_t value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(int64_t value) { _value = value; }
            int64_t GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_INT64; }

            // 类型转换
            virtual bool ToBool() { return _value != 0; }
            virtual int ToInt() { return _value; }
            virtual uint ToUInt() { return _value; }
            virtual int64_t ToInt64() { return _value; }
            virtual uint64_t ToUInt64() { return _value; }
            virtual double ToDouble() { return _value; }
            virtual std::string ToString() { return library::utils::StringConvertUtils::ToString(_value); }

        private:
            int64_t _value;
        };

        class XMF_EXPORT XmfUInt64 : public XmfValue
        {
        public:
            XmfUInt64(uint64_t value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(uint64_t value) { _value = value; }
            uint64_t GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_UINT64; }

            // 类型转换
            virtual bool ToBool() { return _value != 0; }
            virtual int ToInt() { return _value; }
            virtual uint ToUInt() { return _value; }
            virtual int64_t ToInt64() { return _value; }
            virtual uint64_t ToUInt64() { return _value; }
            virtual double ToDouble() { return _value; }
            virtual std::string ToString() { return library::utils::StringConvertUtils::ToString(_value); }

        private:
            uint64_t _value;
        };

        class XMF_EXPORT XmfDouble : public XmfValue
        {
        public:
            XmfDouble(double value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(double value) { _value = value; }
            double GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_DOUBLE; }

            // 类型转换
            virtual bool ToBool() { return _value != 0; }
            virtual int ToInt() { return _value; }
            virtual uint ToUInt() { return _value; }
            virtual int64_t ToInt64() { return _value; }
            virtual uint64_t ToUInt64() { return _value; }
            virtual double ToDouble() { return _value; }
            virtual std::string ToString() { return library::utils::StringConvertUtils::ToString(_value); }

        private:
            double _value;
        };

        class XMF_EXPORT XmfString : public XmfValue
        {
        public:
            XmfString(const std::string &value)
                : _value(value)
            {
            }
            XmfString(const char *value)
                : _value(value)
            {
            }

            // 设置获取值
            void SetValue(const char *value) { _value = value; }
            void SetValue(const std::string value) { _value = value; }
            const std::string &GetValue() const { return _value; }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_STRING; }

            // 类型转换
            virtual bool ToBool() { return library::utils::StringUtils::ToLower(_value) == "true" ? true : false; }
            virtual int ToInt() { return library::utils::StringConvertUtils::StringTo<int>(_value); }
            virtual uint ToUInt() { return library::utils::StringConvertUtils::StringTo<uint>(_value); }
            virtual int64_t ToInt64() { return library::utils::StringConvertUtils::StringTo<int64_t>(_value); }
            virtual uint64_t ToUInt64() { return library::utils::StringConvertUtils::StringTo<uint64_t>(_value); }
            virtual double ToDouble() { return library::utils::StringConvertUtils::StringTo<double>(_value); }
            virtual std::string ToString() { return _value; }

        private:
            std::string _value;
        };

        class XMF_EXPORT XmfArray : public XmfValue
        {
        public:
            XmfArray() { _valueArray = std::make_shared<XmfValueArray>(); }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_ARRAY; }

            // 子节点数量
            virtual size_t GetChildCount() { return _valueArray->size(); }

            // 添加数据
            void AddData(bool value) { _valueArray->push_back(std::make_shared<XmfBool>(value)); }
            void AddData(int value) { _valueArray->push_back(std::make_shared<XmfInt>(value)); }
            void AddData(uint value) { _valueArray->push_back(std::make_shared<XmfUInt>(value)); }
            void AddData(int64_t value) { _valueArray->push_back(std::make_shared<XmfInt64>(value)); }
            void AddData(uint64_t value) { _valueArray->push_back(std::make_shared<XmfUInt64>(value)); }
            void AddData(double value) { _valueArray->push_back(std::make_shared<XmfDouble>(value)); }
            void AddData(const std::string &value) { _valueArray->push_back(std::make_shared<XmfString>(value)); }
            void AddData(const char *value) { _valueArray->push_back(std::make_shared<XmfString>(value)); }
            void AddData(XmfValuePtr valuePtr) { _valueArray->push_back(valuePtr == nullptr ? XmfNull::Null : valuePtr); }

            // 修改数据
            void Set(size_t index, bool value) { Set(index, std::make_shared<XmfBool>(value)); }
            void Set(size_t index, int value) { Set(index, std::make_shared<XmfInt>(value)); }
            void Set(size_t index, uint value) { Set(index, std::make_shared<XmfUInt>(value)); }
            void Set(size_t index, int64_t value) { Set(index, std::make_shared<XmfInt64>(value)); }
            void Set(size_t index, uint64_t value) { Set(index, std::make_shared<XmfUInt64>(value)); }
            void Set(size_t index, double value) { Set(index, std::make_shared<XmfDouble>(value)); }
            void Set(size_t index, const std::string &value) { Set(index, std::make_shared<XmfString>(value)); }
            void Set(size_t index, const char *value) { Set(index, std::make_shared<XmfString>(value)); }
            void Set(size_t index, XmfValuePtr valuePtr)
            {
                if (index < _valueArray->size())
                {
                    (*_valueArray)[index] = valuePtr == nullptr ? XmfNull::Null : valuePtr;
                }
                else
                {
                    std::stringstream stm;
                    stm << "XmfArray Set overflow. index:'" << index << ", size:" << _valueArray->size();
                    throw library::utils::Exception(stm.str().c_str());
                }
            }

            // 删除数据
            void Delete(size_t index)
            {
                if (index < _valueArray->size())
                {
                    _valueArray->erase(_valueArray->begin() + index);
                }
                else
                {
                    std::stringstream stm;
                    stm << "XmfArray Delete overflow. index:'" << index << ", size:" << _valueArray->size();
                    throw library::utils::Exception(stm.str().c_str());
                }
            }

            // 获取数据
            virtual XmfValuePtr Item(size_t index)
            {
                if (index < _valueArray->size())
                {
                    return (*_valueArray)[index];
                }
                else
                {
                    std::stringstream stm;
                    stm << "XmfArray Item overflow. index:'" << index << ", size:" << _valueArray->size();
                    throw library::utils::Exception(stm.str().c_str());
                }
            }

            // 迭代器
            virtual XmfIteratorPtr GetChildIterator();

        private:
            std::shared_ptr<XmfValueArray> _valueArray;
        };

        class XMF_EXPORT XmfObject : public XmfValue
        {
        public:
            XmfObject() { _valueMap = std::make_shared<XmfValueMap>(); }

            // 数据类型
            virtual XmfType DataType() { return XmfType::XMF_OBJECT; }

            // 子节点数量
            virtual size_t GetChildCount() { return _valueMap->size(); }

            // 添加数据
            void AddData(const std::string key, bool value) { (*_valueMap)[key] = std::make_shared<XmfBool>(value); }
            void AddData(const std::string key, int value) { (*_valueMap)[key] = std::make_shared<XmfInt>(value); }
            void AddData(const std::string key, uint value) { (*_valueMap)[key] = std::make_shared<XmfUInt>(value); }
            void AddData(const std::string key, int64_t value) { (*_valueMap)[key] = std::make_shared<XmfInt64>(value); }
            void AddData(const std::string key, uint64_t value) { (*_valueMap)[key] = std::make_shared<XmfUInt64>(value); }
            void AddData(const std::string key, double value) { (*_valueMap)[key] = std::make_shared<XmfDouble>(value); }
            void AddData(const std::string key, const std::string &value) { (*_valueMap)[key] = std::make_shared<XmfString>(value); }
            void AddData(const std::string key, const char *value) { (*_valueMap)[key] = std::make_shared<XmfString>(value); }
            void AddData(const std::string key, XmfValuePtr valuePtr) { (*_valueMap)[key] = valuePtr == nullptr ? XmfNull::Null : valuePtr; }

            // 删除数据
            void Delete(const std::string key)
            {
                auto it = _valueMap->find(key);
                if (it != _valueMap->end())
                {
                    _valueMap->erase(it);
                }
            }

            // 取值
            virtual XmfValuePtr Item(const char *key)
            {
                auto it = _valueMap->find(key);
                if (it != _valueMap->end())
                {
                    return it->second;
                }
                else
                {
                    std::stringstream stm;
                    stm << "XmfObject Item['" << key << "'] not exist!";
                    throw library::utils::Exception(stm.str().c_str());
                }
            }

            // 查找
            XmfValuePtr Find(const std::string key)
            {
                auto it = _valueMap->find(key);
                if (it != _valueMap->end())
                {
                    return it->second;
                }
                else
                {
                    return nullptr;
                }
            }

            // 迭代器
            virtual XmfIteratorPtr GetChildIterator();

        private:
            std::shared_ptr<XmfValueMap> _valueMap;
        };
    } // namespace xmf
} // namespace library
