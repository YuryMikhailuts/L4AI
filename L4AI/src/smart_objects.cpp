/*
 * smart_objects.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
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
    SmartFloatSubType SmartFloat<float>::floatSubType() const { return SmartFloatSubType::Float32; }

    template<>
    SmartFloatSubType SmartFloat<double>::floatSubType() const { return SmartFloatSubType::Float64; }


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
    template class SmartInt<uint64_t>;
    template class SmartInt<int8_t>;
    template class SmartInt<int16_t>;
    template class SmartInt<int32_t>;
    template class SmartInt<int64_t>;
    template class SmartFloat<float>;
    template class SmartFloat<double>;
    template class SmartString<char>;
    template class SmartString<wchar_t>;

}