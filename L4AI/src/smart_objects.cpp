/*
 * smart_objects.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <calculator.h>
#include "smart_objects.h"

using namespace std;

namespace l4ai::smart {

    SmartObject::SmartObject(std::string_view smartClass) : smartClass(smartClass) {}

    std::shared_ptr<SmartIntObject> SmartObject::asInt() {
        return std::dynamic_pointer_cast<SmartIntObject>(shared_from_this());
    }

    std::shared_ptr<class SmartFloatObject> SmartObject::asFloat() {
        return std::dynamic_pointer_cast<SmartFloatObject>(shared_from_this());
    }

    std::shared_ptr<class SmartStringObject> SmartObject::asString() {
        return std::dynamic_pointer_cast<SmartStringObject>(shared_from_this());
    }

    std::shared_ptr<class SmartArrayObject> SmartObject::asArray() {
        return std::dynamic_pointer_cast<SmartArrayObject>(shared_from_this());
    }

    std::shared_ptr<class SmartMapObject> SmartObject::asMap() {
        return std::dynamic_pointer_cast<SmartMapObject>(shared_from_this());
    }

    SmartObject::~SmartObject() = default;

    template<>
    SmartIntSubType SmartInt<uint8_t>::intSubType() const { return SmartIntSubType::UInt8; }

    template<>
    SmartIntSubType SmartInt<uint16_t>::intSubType() const { return SmartIntSubType::UInt16; }

    template<>
    SmartIntSubType SmartInt<uint32_t>::intSubType() const { return SmartIntSubType::UInt32; }

    template<>
    SmartIntSubType SmartInt<uint64_t>::intSubType() const { return SmartIntSubType::UInt64; }

    template<>
    SmartIntSubType SmartInt<int8_t>::intSubType() const { return SmartIntSubType::SInt8; }

    template<>
    SmartIntSubType SmartInt<int16_t>::intSubType() const { return SmartIntSubType::SInt16; }

    template<>
    SmartIntSubType SmartInt<int32_t>::intSubType() const { return SmartIntSubType::SInt32; }

    template<>
    SmartIntSubType SmartInt<int64_t>::intSubType() const { return SmartIntSubType::SInt64; }

    template<>
    SmartIntSubType SmartIntArray<uint8_t>::intSubType() const { return SmartIntSubType::UInt8; }

    template<typename TInt>
    void SmartIntArray<TInt>::resize(size_t size) {
        data.resize(size);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, int8_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, int16_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, int32_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, int64_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, uint8_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, uint16_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, uint32_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::set(size_t index, uint64_t value) {
        data[index] = (TInt) value;
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(int8_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(int16_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(int32_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(int64_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(uint8_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(uint16_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(uint32_t value) {
        data.push_back((TInt) value);
    }

    template<typename TInt>
    void SmartIntArray<TInt>::push_back(uint64_t value) {
        data.push_back((TInt) value);
    }

    template<>
    SmartIntSubType SmartIntArray<uint16_t>::intSubType() const { return SmartIntSubType::UInt16; }

    template<>
    SmartIntSubType SmartIntArray<uint32_t>::intSubType() const { return SmartIntSubType::UInt32; }

    template<>
    SmartIntSubType SmartIntArray<uint64_t>::intSubType() const { return SmartIntSubType::UInt64; }

    template<>
    SmartIntSubType SmartIntArray<int8_t>::intSubType() const { return SmartIntSubType::SInt8; }

    template<>
    SmartIntSubType SmartIntArray<int16_t>::intSubType() const { return SmartIntSubType::SInt16; }

    template<>
    SmartIntSubType SmartIntArray<int32_t>::intSubType() const { return SmartIntSubType::SInt32; }

    template<>
    SmartIntSubType SmartIntArray<int64_t>::intSubType() const { return SmartIntSubType::SInt64; }

    template<>
    SmartFloatSubType SmartFloat<float>::floatSubType() const { return SmartFloatSubType::Float32; }

    template<>
    SmartFloatSubType SmartFloat<double>::floatSubType() const { return SmartFloatSubType::Float64; }

    template<>
    SmartFloatSubType SmartFloatArray<float>::floatSubType() const { return SmartFloatSubType::Float32; }

    template<typename TFloat>
    void SmartFloatArray<TFloat>::resize(size_t size) {
        data.resize(size);
    }

    template<typename TFloat>
    void SmartFloatArray<TFloat>::set(size_t index, float value) {
        data[index] = (TFloat) value;
    }

    template<typename TFloat>
    void SmartFloatArray<TFloat>::set(size_t index, double value) {
        data[index] = (TFloat) value;
    }

    template<typename TFloat>
    void SmartFloatArray<TFloat>::push_back(float value) {
        data.push_back((TFloat) value);
    }

    template<typename TFloat>
    void SmartFloatArray<TFloat>::push_back(double value) {
        data.push_back((TFloat) value);
    }

    template<>
    SmartFloatSubType SmartFloatArray<double>::floatSubType() const { return SmartFloatSubType::Float64; }

    template<>
    SmartStringSubType SmartString<char>::stringSubType() const { return SmartStringSubType::String; }

    template<>
    SmartStringSubType SmartString<wchar_t>::stringSubType() const { return SmartStringSubType::WString; }

    std::shared_ptr<SmartObjectArray> SmartObjectArray::addObjectArray(std::string_view smartClass) {
        auto result = SmartObject::create<SmartObjectArray>(smartClass);
        data.template emplace_back(result);
        return result;
    }

    std::shared_ptr<SmartMapObject> SmartObjectArray::addMap(std::string_view smartClass) {
        auto result = SmartObject::create<SmartMapObject>(smartClass);
        data.template emplace_back(result);
        return result;
    }

    std::optional<SmartIntSubType> SmartObjectArray::canConvertToIntArray() {
        std::optional<SmartIntSubType> result;
        for (const auto &item : data) {
            if (item->smartType() == SmartType::Int) {
                auto intItem = item->asInt();
                if (result) {
                    if (intItem->intSubType() >= *result)
                        result = intItem->intSubType();
                } else {
                    result = intItem->intSubType();
                }
            } else {
                break;
            }
        }
        return result;
    }

    std::optional<SmartFloatSubType> SmartObjectArray::canConvertToFloatArray() {
        std::optional<SmartFloatSubType> result;
        for (const auto &item : data) {
            if (item->smartType() == SmartType::Int) {
                auto intItem = item->asInt();
                auto itemFloatType = minFloatType(intItem->intSubType());
                if (result) {
                    if (itemFloatType >= *result)
                        result = itemFloatType;
                } else {
                    result = itemFloatType;
                }
            } else if (item->smartType() == SmartType::Float) {
                auto intItem = item->asFloat();
                if (result) {
                    if (intItem->floatSubType() >= *result)
                        result = intItem->floatSubType();
                } else {
                    result = intItem->floatSubType();
                }
            } else {
                break;
            }
        }
        return result;
    }

    std::shared_ptr<SmartIntArrayObject>
    SmartObjectArray::convertToIntArray(std::optional<SmartIntSubType> intSubType) {
        std::shared_ptr<SmartIntArrayObject> result;
        if (auto intType = canConvertToIntArray(); intType) {
            result = SmartIntArrayObject::create(*intType);
            result->resize(size());
            auto iType = intSubType ? *intSubType : *intType;
            switch (iType) {
                case SmartIntSubType::SInt8:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (int8_t) *item);
                    }
                    break;
                case SmartIntSubType::SInt16:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (int16_t) *item);
                    }
                    break;
                case SmartIntSubType::SInt32:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (int32_t) *item);
                    }
                    break;
                case SmartIntSubType::SInt64:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (int64_t) *item);
                    }
                    break;
                case SmartIntSubType::UInt8:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (uint8_t) *item);
                    }
                    break;
                case SmartIntSubType::UInt16:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (uint16_t) *item);
                    }
                    break;
                case SmartIntSubType::UInt32:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (uint32_t) *item);
                    }
                    break;
                case SmartIntSubType::UInt64:
                    for (size_t i = 0; i < size(); ++i) {
                        auto item = data[i]->asInt();
                        result->set(i, (uint64_t) *item);
                    }
                    break;
            }
        }
        return result;
    }

    std::shared_ptr<SmartFloatArrayObject>
    SmartObjectArray::convertToFloatArray(std::optional<SmartFloatSubType> floatSubType) {
        std::shared_ptr<SmartFloatArrayObject> result;
        if (auto floatType = canConvertToFloatArray(); floatType) {
            result = SmartFloatArrayObject::create(*floatType);
            result->resize(size());
            auto fType = floatSubType ? *floatSubType : *floatType;
            switch (fType) {
                case SmartFloatSubType::Float32:
                    for (size_t i = 0; i < size(); ++i) {
                        if (auto intItem = data[i]->asInt(); intItem) {
                            result->set(i, (float) *intItem);
                        } else if (auto floatItem = data[i]->asFloat(); floatItem) {
                            result->set(i, (float) *floatItem);
                        }
                    }
                    break;
                case SmartFloatSubType::Float64:
                    for (size_t i = 0; i < size(); ++i) {
                        if (auto intItem = data[i]->asInt(); intItem) {
                            result->set(i, (double) *intItem);
                        } else if (auto floatItem = data[i]->asFloat(); floatItem) {
                            result->set(i, (double) *floatItem);
                        }
                    }
                    break;
            }
        }
        return result;
    }

    void SmartObjectArray::resize(size_t size) {
        data.resize(size);
    }

    std::shared_ptr<SmartObjectArray>
    SmartMapObject::setObjectArray(std::string_view name, std::string_view smartClass) {
        auto result = SmartObject::create<SmartObjectArray>(smartClass);
        data.emplace(std::string(name), result);
        return result;
    }

    std::shared_ptr<class SmartMapObject> SmartMapObject::setMap(std::string_view name, std::string_view smartClass) {
        auto result = SmartObject::create<SmartMapObject>(smartClass);
        data.emplace(std::string(name), result);
        return result;
    }

    std::shared_ptr<SmartObject> SmartMapObject::operator[](std::string_view name) {
        return data[string(name)];
    }


    template class SmartInt<uint8_t>;
    template class SmartInt<uint16_t>;
    template class SmartInt<uint32_t>;

    template
    class SmartInt<uint64_t>;

    template
    class SmartInt<int8_t>;

    template
    class SmartInt<int16_t>;

    template
    class SmartInt<int32_t>;

    template
    class SmartInt<int64_t>;

    template
    class SmartFloat<float>;

    template
    class SmartFloat<double>;

    template
    class SmartString<char>;

    template
    class SmartString<wchar_t>;

    std::shared_ptr<class SmartObjectArray> SmartArrayObject::asObjectArray() {
        return std::dynamic_pointer_cast<SmartObjectArray>(shared_from_this());
    }

    std::shared_ptr<class SmartIntArrayObject> SmartArrayObject::asIntArray() {
        return std::dynamic_pointer_cast<SmartIntArrayObject>(shared_from_this());
    }

    std::shared_ptr<class SmartFloatArrayObject> SmartArrayObject::asFloatArray() {
        return std::dynamic_pointer_cast<SmartFloatArrayObject>(shared_from_this());
    }

    shared_ptr<SmartIntArrayObject>
    SmartIntArrayObject::create(SmartIntSubType subType, std::string_view smartClassName) {
        switch (subType) {
            case SmartIntSubType::SInt8:
                return SmartObject::create<SmartIntArray<int8_t>>(smartClassName);
            case SmartIntSubType::SInt16:
                return SmartObject::create<SmartIntArray<int16_t>>(smartClassName);
            case SmartIntSubType::SInt32:
                return SmartObject::create<SmartIntArray<int32_t>>(smartClassName);
            case SmartIntSubType::SInt64:
                return SmartObject::create<SmartIntArray<int64_t>>(smartClassName);
            case SmartIntSubType::UInt8:
                return SmartObject::create<SmartIntArray<uint8_t>>(smartClassName);
            case SmartIntSubType::UInt16:
                return SmartObject::create<SmartIntArray<uint16_t>>(smartClassName);
            case SmartIntSubType::UInt32:
                return SmartObject::create<SmartIntArray<uint32_t>>(smartClassName);
            case SmartIntSubType::UInt64:
                return SmartObject::create<SmartIntArray<uint64_t>>(smartClassName);
            default:
                return shared_ptr<SmartIntArrayObject>();
        }
    }

    shared_ptr<SmartFloatArrayObject>
    SmartFloatArrayObject::create(SmartFloatSubType subType, std::string_view smartClassName) {
        switch (subType) {
            case SmartFloatSubType::Float32:
                return SmartObject::create<SmartFloatArray<float>>(smartClassName);
            case SmartFloatSubType::Float64:
                return SmartObject::create<SmartFloatArray<double>>(smartClassName);
            default:
                return shared_ptr<SmartFloatArrayObject>();
        }
    }

    std::string to_string(SmartType smartType) {
        switch (smartType) {
            case SmartType::Int:
                return "Int"s;
            case SmartType::Float:
                return "Float"s;
            case SmartType::String:
                return "String"s;
            case SmartType::Array:
                return "Array"s;
            case SmartType::Map:
                return "Map"s;
            default:
                return "Unknown"s;
        }
    }

    std::string to_string(SmartIntSubType smartIntSubType) {
        switch (smartIntSubType) {
            case SmartIntSubType::SInt64:
                return "SInt64"s;
            case SmartIntSubType::SInt8:
                return "SInt8"s;
            case SmartIntSubType::SInt16:
                return "SInt16"s;
            case SmartIntSubType::SInt32:
                return "SInt32"s;
            case SmartIntSubType::UInt8:
                return "UInt8"s;
            case SmartIntSubType::UInt16:
                return "UInt16"s;
            case SmartIntSubType::UInt32:
                return "UInt32"s;
            case SmartIntSubType::UInt64:
                return "UInt64"s;
            default:
                return "Unknown"s;
        }
    }

    std::string to_string(SmartFloatSubType smartFloatSubType) {
        switch (smartFloatSubType) {
            case SmartFloatSubType::Float32:
                return "Float32"s;
            case SmartFloatSubType::Float64:
                return "Float64"s;
            default:
                return "Unknown"s;
        }
    }

    std::string to_string(SmartStringSubType smartStringSubType) {
        switch (smartStringSubType) {
            case SmartStringSubType::String:
                return "String"s;
            case SmartStringSubType::WString:
                return "WString"s;
            default:
                return "Unknown"s;
        }
    }

    std::string to_string(SmartArraySubType smartArraySubType) {
        switch (smartArraySubType) {
            case SmartArraySubType::Object:
                return "ObjectArray"s;
            case SmartArraySubType::Int:
                return "IntArray"s;
            case SmartArraySubType::Float:
                return "FloatArray"s;
            default:
                return "UnknownArray"s;
        }
    }


}