/**
 * @file xmf_object.cpp
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
#include "xmf/xmf_object.h"

namespace library
{
    namespace xmf
    {
        XmfValuePtr XmfNull::Null = std::make_shared<XmfNull>();

        // 空迭代器
        class XmfNullIterator : public XmfIterator
        {
        public:
            virtual void MoveFirst() { throw library::utils::Exception("NO Children"); }
            virtual void MoveNext() { throw library::utils::Exception("NO Children"); }
            virtual bool IsEof() { return true; }
            virtual const std::string GetKey() { throw library::utils::Exception("NO Children"); }
            virtual XmfValuePtr GetValue() { throw library::utils::Exception("NO Children"); }

            static XmfIteratorPtr Iterator;
        };

        XmfIteratorPtr XmfNullIterator::Iterator = std::make_shared<XmfNullIterator>();

        // 基本迭代器为空迭代器
        XmfIteratorPtr XmfValue::GetChildIterator()
        {
            return XmfNullIterator::Iterator;
        }

        // 数组迭代器
        class XmfArrayIterator : public XmfIterator
        {
        public:
            XmfArrayIterator(XmfValueArray *pValueArray)
                : _pValueArray(pValueArray)
            {
                _it = _pValueArray->begin();
            }

            virtual void MoveFirst() { _it = _pValueArray->begin(); }
            virtual void MoveNext() { _it++; }
            virtual bool IsEof() { return _it == _pValueArray->end(); }

            virtual const std::string GetKey()
            {
                std::stringstream buf;
                buf << (size_t)(_it - _pValueArray->begin());
                return buf.str();
            }

            virtual XmfValuePtr GetValue()
            {
                if (_it != _pValueArray->end())
                {
                    return *_it;
                }
                else
                {
                    throw library::utils::Exception("Is Eof");
                }
            }

        private:
            XmfValueArray *_pValueArray;
            XmfValueArray::iterator _it;
        };

        // 数组迭代器
        XmfIteratorPtr XmfArray::GetChildIterator()
        {
            return std::make_shared<XmfArrayIterator>(_valueArray.get());
        }

        // 对象迭代器
        class XmfObjectIterator : public XmfIterator
        {
        public:
            XmfObjectIterator(XmfValueMap *valueMap)
                : _pValueMap(valueMap)
            {
                _it = _pValueMap->begin();
            }

            virtual void MoveFirst() { _it = _pValueMap->begin(); }
            virtual void MoveNext() { _it++; }
            virtual bool IsEof() { return (_it == _pValueMap->end()); }

            virtual const std::string GetKey()
            {
                if (_it != _pValueMap->end())
                    return (*_it).first;
                else
                    throw library::utils::Exception("Is Eof");
            }

            virtual XmfValuePtr GetValue()
            {
                if (_it != _pValueMap->end())
                    return (*_it).second;
                else
                    throw library::utils::Exception("Is Eof");
            }

        private:
            XmfValueMap *_pValueMap;
            XmfValueMap::iterator _it;
        };

        // 对象迭代器
        XmfIteratorPtr XmfObject::GetChildIterator()
        {
            return std::make_shared<XmfObjectIterator>(_valueMap.get());
        }
    } // namespace xmf
} // namespace library
