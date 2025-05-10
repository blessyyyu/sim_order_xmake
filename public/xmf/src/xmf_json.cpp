/**
 * @file xmf_json.cpp
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
#include "xmf/xmf_json.h"

#include <fstream>
#include <list>
#include <sstream>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"

namespace library
{
    namespace xmf
    {
        using namespace rapidjson;

        /**
         * @brief Json SAX模式解析处理器
         */
        class JsonSaxHandler : public BaseReaderHandler<UTF8<>, JsonSaxHandler>
        {
        public:
            JsonSaxHandler(XmfValuePtr &valuePtr)
                : _valuePtr(valuePtr)
            {
            }
            bool Null()
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(XmfNull::Null);
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, XmfNull::Null);
                }

                return true;
            }

            bool Bool(bool value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfBool>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfBool>(value));
                }

                return true;
            }

            bool Int(int value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfInt>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfInt>(value));
                }

                return true;
            }

            bool Uint(unsigned value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfUInt>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfUInt>(value));
                }

                return true;
            }

            bool Int64(int64_t value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfInt64>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfInt64>(value));
                }

                return true;
            }

            bool Uint64(uint64_t value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfUInt64>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfUInt64>(value));
                }

                return true;
            }

            bool Double(double value)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfDouble>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfDouble>(value));
                }

                return true;
            }

            bool String(const char *value, SizeType length, bool copy)
            {
                auto parent = _parentListPtr.back();
                if (parent->DataType() == XmfType::XMF_ARRAY)
                {
                    ((XmfArray *)parent)->AddData(std::make_shared<XmfString>(value));
                }
                else if (parent->DataType() == XmfType::XMF_OBJECT)
                {
                    ((XmfObject *)parent)->AddData(_curKey, std::make_shared<XmfString>(value));
                }

                return true;
            }

            bool StartObject()
            {
                auto objPtr = std::make_shared<XmfObject>();

                if (!_valuePtr)
                {
                    _valuePtr = objPtr;
                }

                if (_parentListPtr.size() > 0)
                {
                    auto parent = _parentListPtr.back();
                    if (parent->DataType() == XmfType::XMF_ARRAY)
                    {
                        ((XmfArray *)parent)->AddData(objPtr);
                    }
                    else if (parent->DataType() == XmfType::XMF_OBJECT)
                    {
                        ((XmfObject *)parent)->AddData(_curKey, objPtr);
                    }
                }

                _parentListPtr.push_back(objPtr.get());

                return true;
            }

            bool Key(const char *value, SizeType length, bool copy)
            {
                _curKey = std::string(value, length);
                return true;
            }

            bool EndObject(SizeType memberCount)
            {
                _parentListPtr.pop_back();
                return true;
            }

            bool StartArray()
            {
                auto arrPtr = std::make_shared<XmfArray>();

                if (!_valuePtr)
                {
                    _valuePtr = arrPtr;
                }

                if (_parentListPtr.size() > 0)
                {
                    auto parent = _parentListPtr.back();
                    if (parent->DataType() == XmfType::XMF_ARRAY)
                    {
                        ((XmfArray *)parent)->AddData(arrPtr);
                    }
                    else if (parent->DataType() == XmfType::XMF_OBJECT)
                    {
                        ((XmfObject *)parent)->AddData(_curKey, arrPtr);
                    }
                }

                _parentListPtr.push_back(arrPtr.get());

                return true;
            }

            bool EndArray(SizeType elementCount)
            {
                _parentListPtr.pop_back();
                return true;
            }

        private:
            XmfValuePtr &_valuePtr;
            std::string _curKey;
            std::list<XmfValue *> _parentListPtr;
        };

        XmfValuePtr XmfJson::Read(const char *fileName)
        {
            _errMsg.clear();
            std::ifstream in(fileName);
            if (!in.is_open())
            {
                std::stringstream stm;
                stm << "Can not open the file : " << fileName;
                _errMsg = stm.str();
                return nullptr;
            }

            return Read(in);
        }

        XmfValuePtr XmfJson::Read(std::istream &in)
        {
            _errMsg.clear();

            XmfValuePtr valuePtr = nullptr;
            JsonSaxHandler handler(valuePtr);
            Reader reader;
            IStreamWrapper wrapper(in);
            auto rst = reader.Parse(wrapper, handler);
            if (!rst)
            {
                std::stringstream stm;
                stm << "There is an error at the offset : " << rst.Offset();
                _errMsg = stm.str();
                return nullptr;
            }

            return valuePtr;
        }

        bool XmfJson::Write(const char *fileName, XmfValuePtr valuePtr, bool pretty)
        {
            _errMsg.clear();
            std::ofstream out(fileName);
            if (!out.is_open())
            {
                std::stringstream stm;
                stm << "Can not open the file : " << fileName;
                _errMsg = stm.str();
                return false;
            }

            return Write(out, valuePtr, pretty);
        }

        template <typename Writer>
        static void WriteImpl(Writer &writer, XmfValuePtr valuePtr)
        {
            switch (valuePtr->DataType())
            {
            case XmfType::XMF_NULL:
            {
                writer.Null();
                break;
            }
            case XmfType::XMF_BOOL:
            {
                writer.Bool(valuePtr->ToBool());
                break;
            }
            case XmfType::XMF_INT:
            {
                writer.Int(valuePtr->ToInt());
                break;
            }
            case XmfType::XMF_UINT:
            {
                writer.Uint(valuePtr->ToUInt());
                break;
            }
            case XmfType::XMF_INT64:
            {
                writer.Int64(valuePtr->ToInt64());
                break;
            }
            case XmfType::XMF_UINT64:
            {
                writer.Uint64(valuePtr->ToUInt64());
                break;
            }
            case XmfType::XMF_DOUBLE:
            {
                writer.Double(valuePtr->ToDouble());
                break;
            }
            case XmfType::XMF_STRING:
            {
                writer.String(valuePtr->ToString().c_str());
                break;
            }
            case XmfType::XMF_ARRAY:
            {
                writer.StartArray();
                auto itor = valuePtr->GetChildIterator();
                while (!itor->IsEof())
                {
                    WriteImpl(writer, itor->GetValue());
                    itor->MoveNext();
                }
                writer.EndArray();
                break;
            }
            case XmfType::XMF_OBJECT:
            {
                writer.StartObject();
                auto itor = valuePtr->GetChildIterator();
                while (!itor->IsEof())
                {
                    writer.Key(itor->GetKey().c_str());
                    WriteImpl(writer, itor->GetValue());
                    itor->MoveNext();
                }
                writer.EndObject();
                break;
            }
            default:
                break;
            }
        }

        bool XmfJson::Write(std::ostream &out, XmfValuePtr rootPtr, bool pretty)
        {
            _errMsg.clear();

            if (rootPtr->DataType() == XmfType::XMF_ARRAY || rootPtr->DataType() == XmfType::XMF_OBJECT)
            {
                OStreamWrapper wrapper(out);
                if (pretty)
                {
                    PrettyWriter<OStreamWrapper> writer(wrapper);
                    WriteImpl(writer, rootPtr);
                }
                else
                {
                    Writer<OStreamWrapper> writer(wrapper);
                    WriteImpl(writer, rootPtr);
                }
            }
            else
            {
                std::stringstream stm;
                stm << "Json string must start with an object or an array";
                _errMsg = stm.str();
                return false;
            }

            return true;
        }
    } // namespace xmf
} // namespace library
