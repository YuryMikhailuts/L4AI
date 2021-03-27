/*
 * smart_from_json.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <smart_from_json.h>
#include <sstream>
#include <utility>

namespace l4ai::smart {
    using namespace std;

    shared_ptr<SmartObject> SmartFromJson::get() {
        col_num = 1;
        row_num = 1;
        last_peek = nullopt;
        last_getc = nullopt;
        return getObject();
    }

    void SmartFromJson::log(SmartLogLevel logLevel, string_view text) {
        stringstream ss;
        ss << "строка " << col_num << ", столбец " << row_num << ". " << text;
        logList.emplace_back(logLevel, ss.str());
    }

    int SmartFromJson::peek() {
        last_peek = in.peek();
        return *last_peek;
    }

    int SmartFromJson::getc() {
        int c = in.get();
        ++col_num;
        if (c == '\n' || (c == '\r' && peek() != '\n')) {
            ++row_num;
            col_num = 1;
        }
        if (c != -1) last_getc = (char) c;
        return c;
    }

    SmartFromJson::SmartFromJson(istream &in) : in(in), row_num(1), col_num(1), logList(), errors_count(),
                                                warnings_count(), last_peek(), last_getc(), classTypes() {}

    shared_ptr<SmartObject> SmartFromJson::getObject() {
        shared_ptr<SmartObject> result;
        while (can_read() && !result) {
            switch (peek()) {
                case '{':
                    result = checkClassObject(getMap());
                    break;
                case '[':
                    result = getArray();
                    break;
                case '"':
                case '\'':
                    result = getString();
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '+':
                case '-':
                    result = getNumber();
                    break;
                case -1:
                    err("Неожиданный конец файла.");
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    getc();
                    break;
                default:
                    wrn("Неожиданный символ '"s + (char) peek() + "'.");
                    break;
            }
        }
        return result;
    }

    bool SmartFromJson::can_read() const { return in.good() && !in.eof(); }

    shared_ptr<SmartArrayObject> SmartFromJson::getArray() {
        shared_ptr<SmartObjectArray> smartArrayObject = SmartObject::create<SmartObjectArray>();
        if (peek() == '[') getc(); else return shared_ptr<SmartObjectArray>();
        while (can_read()) {
            switch (peek()) {
                case ']':
                    getc();
                    if (auto intArray = smartArrayObject->convertToIntArray(); intArray)
                        return intArray;
                    if (auto floatArray = smartArrayObject->convertToFloatArray(); floatArray)
                        return floatArray;
                    return smartArrayObject;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    getc();
                    break;
                default: {
                    auto object = getObject();
                    if (object) {
                        smartArrayObject->data.push_back(object);
                        if (nextChar(',')) {
                            getc();
                        } else if (peek() != ']') {
                            wrn("Неожиданный символ '"s + (char) peek() + "', ожидается символ ','.");
                        }
                    }
                    break;
                }
            }
        }
        return smartArrayObject;
    }

    shared_ptr<SmartMapObject> SmartFromJson::getMap() {
        shared_ptr<SmartMapObject> smartMapObject = SmartObject::create<SmartMapObject>();;
        if (peek() == '{') getc(); else return shared_ptr<SmartMapObject>();
        while (can_read()) {
            switch (peek()) {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    getc();
                    break;
                default: {
                    auto name = getMapFieldName();
                    if (can_read()) {
                        getc();
                        auto object = getObject();
                        smartMapObject->data.emplace(name, object);
                        if (nextChar(',')) {
                            getc();
                        } else if (peek() == '}') {
                            getc();
                            return smartMapObject;
                        } else {
                            wrn("Неожиданный символ '"s + (char) peek() + "', ожидается символ ','.");
                        }
                    }
                    break;
                }
            }
        }
        return smartMapObject;
    }

    uint64_t SmartFromJson::getInt() {
        uint64_t value = 0;
        while (can_read() && is_digit())
            value = value * 10 + (getc() - '0');
        return value;
    }

    inline bool SmartFromJson::is_digit() {
        switch (peek()) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
            default:
                return false;
        }
    }

    double SmartFromJson::getFloat(double value) {
        while (can_read() && is_digit())
            value = value * 10 + (getc() - '0');
        if (peek() == '.') {
            getc();
            double k = 10;
            while (can_read() && is_digit()) {
                value = value + (getc() - '0') / k;
                k *= 10;
            }
        }
        return value;
    }

    shared_ptr<SmartStringObject> SmartFromJson::getString() {
        string text;
        char quote = (char) getc();
        while (can_read() && peek() != quote) {
            text += (char) getc();
        }
        getc();
        return SmartStringObject::create<char>(text);
    }

    string SmartFromJson::getMapFieldName() {
        string name;
        switch (getc()) {
            case '\'':
                while (can_read()) {
                    switch (peek()) {
                        case '\'':
                            getc();
                            if (!nextChar(':'))
                                wrn("Неожиданный символ '"s + (char) peek() + "', ожидается символ ':'.");
                            return name;
                        case -1:
                            err("Неожиданный конец файла. Ожидается одинарная кавычка.");
                            return name;
                        default:
                            name += (char) getc();
                    }
                }
                break;
            case '"':
                while (can_read()) {
                    switch (peek()) {
                        case '"':
                            getc();
                            if (!nextChar(':'))
                                wrn("Неожиданный символ '"s + (char) peek() + "', ожидается символ ':'.");
                            return name;
                        case -1:
                            err("Неожиданный конец файла. Ожидается двойная кавычка.");
                            return name;
                        default:
                            name += (char) getc();
                    }
                }
                break;
            case -1:
                return name;
            default:
                while (can_read()) {
                    switch (peek()) {
                        case ':':
                            getc();
                            return name;
                        case -1:
                            err("Неожиданный конец файла. Ожидается двоеточие.");
                            return name;
                        default:
                            name += (char) getc();
                    }
                }
                break;
        }
        return name;
    }

    bool SmartFromJson::nextChar(char c, char ignore) {
        while (can_read()) {
            int p = peek();
            switch (p) {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    getc();
                    break;
                default:
                    if (p == c) return true;
                    else if (p != ignore) return false;
            }
        }
        return false;
    }

    shared_ptr<SmartObject> SmartFromJson::getNumber() {
        shared_ptr<SmartObject> result;
        int p = peek();
        int sign = 1;
        if (p == '-' || p == '+') {
            getc();
            if (p == '-') sign = -1;
        }
        switch (peek()) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                uint64_t uintValue = getInt();
                if (peek() == '.') {
                    result = SmartFloatObject::create(getFloat(uintValue * sign));
                } else {
                    result = SmartIntObject::create(uintValue * sign);
                }
                break;
            }
            default: {
                wrn("Неожиданный символ '"s + (char) peek() + "', ожидается символ '0'-'9'.");
                break;
            }
        }
        return result;
    }

    shared_ptr<SmartObject> SmartFromJson::checkClassObject(const shared_ptr<SmartMapObject> &mapPtr) {
        if (mapPtr->data.count("@class") == 0 || mapPtr->data.count("@value") == 0 ||
            mapPtr->data["@class"]->smartType() != SmartType::String || mapPtr->data.size() > 2) {
            return mapPtr;
        } else {
            auto smartClass = mapPtr->data["@class"]->asString()->asCStr();
            auto result = mapPtr->data["@value"];
            if (classTypes.count(smartClass) > 0) {
                auto &requirement = classTypes.at(smartClass);
                if (requirement.objectType) {
                    switch (*requirement.objectType) {
                        case SmartType::Int:
                            result = getIntObject(smartClass, result, requirement);
                            break;
                        case SmartType::Float:
                            result = getFloatObject(smartClass, result, requirement);
                            break;
                        case SmartType::String:
                            result = getStringObject(smartClass, result, requirement);
                            break;
                        case SmartType::Array: {
                            result = getArrayObject(smartClass, result, requirement);
                            break;
                        }
                        case SmartType::Map:
                            if (result->smartType() != *requirement.objectType)
                                wrn("Ожидается тип '"s + to_string(*requirement.objectType) + "', но обнаружен '" +
                                    to_string(result->smartType()) + "'.");
                            break;
                    }
                }
            }
            result->smartClass = smartClass;
            return result;
        }
    }

    shared_ptr<SmartObject> &
    SmartFromJson::getArrayObject(string &smartClass, shared_ptr<SmartObject> &result,
                                  SmartTypeRequirement &requirement) {
        if (result->smartType() == *requirement.objectType) {
            if (requirement.arraySubType) {
                auto resultArray = result->asArray();
                switch (*requirement.arraySubType) {
                    case SmartArraySubType::Object: {
                        if (resultArray->arraySubType() != *requirement.arraySubType) {
                            auto res = SmartObject::create<SmartObjectArray>(smartClass);
                            for (size_t i = 0; i < resultArray->size(); ++i) {
                                res->data.push_back(resultArray->at(i));
                            }
                            result = res;
                        }
                        break;
                    }
                    case SmartArraySubType::Int: {
                        switch (resultArray->arraySubType()) {
                            case SmartArraySubType::Object: {
                                if (auto intArrayRes = resultArray->asObjectArray()->convertToIntArray(
                                            requirement.intSubType); intArrayRes) {
                                    result = intArrayRes;
                                } else {
                                    wrn("Не удаётся преобразовать Массив объектов в массив целых чисел.");
                                }
                                break;
                            }
                            case SmartArraySubType::Int: {
                                if (auto intArrayRes = resultArray->asIntArray(); intArrayRes) {
                                    if (requirement.intSubType &&
                                        intArrayRes->intSubType() != *requirement.intSubType) {
                                        auto res = SmartIntArrayObject::create(*requirement.intSubType, smartClass);
                                        for (size_t i = 0; i < resultArray->size(); ++i) {
                                            switch (*requirement.intSubType) {
                                                case SmartIntSubType::SInt8:
                                                    res->push_back(intArrayRes->atInt8(i));
                                                    break;
                                                case SmartIntSubType::SInt16:
                                                    res->push_back(intArrayRes->atInt16(i));
                                                    break;
                                                case SmartIntSubType::SInt32:
                                                    res->push_back(intArrayRes->atInt32(i));
                                                    break;
                                                case SmartIntSubType::SInt64:
                                                    res->push_back(intArrayRes->atInt64(i));
                                                    break;
                                                case SmartIntSubType::UInt8:
                                                    res->push_back(intArrayRes->atUInt8(i));
                                                    break;
                                                case SmartIntSubType::UInt16:
                                                    res->push_back(intArrayRes->atUInt16(i));
                                                    break;
                                                case SmartIntSubType::UInt32:
                                                    res->push_back(intArrayRes->atUInt32(i));
                                                    break;
                                                case SmartIntSubType::UInt64:
                                                    res->push_back(intArrayRes->atUInt64(i));
                                                    break;
                                            }
                                        }
                                        result = res;
                                    }
                                } else {
                                    wrn("Не удаётся преобразовать Массив в массив целых чисел.");
                                }
                                break;
                            }
                            case SmartArraySubType::Float:
                                wrn("Невозможно преобразовать массив действительных чисел в массив целых без потери данных.");
                        }
                        break;
                    }
                    case SmartArraySubType::Float: {
                        switch (resultArray->arraySubType()) {
                            case SmartArraySubType::Object: {
                                if (auto floatArrayRes = resultArray->asObjectArray()->convertToFloatArray(
                                            requirement.floatSubType); floatArrayRes) {
                                    result = floatArrayRes;
                                } else {
                                    wrn("Не удаётся преобразовать Массив объектов в массив действительных чисел.");
                                }
                                break;
                            }
                            case SmartArraySubType::Int: {
                                if (auto intArrayRes = resultArray->asIntArray(); intArrayRes) {
                                    auto res = SmartFloatArrayObject::create(*requirement.floatSubType, smartClass);
                                    for (size_t i = 0; i < resultArray->size(); ++i) {
                                        switch (*requirement.floatSubType) {
                                            case SmartFloatSubType::Float32:
                                                res->push_back(intArrayRes->atFloat32(i));
                                                break;
                                            case SmartFloatSubType::Float64:
                                                res->push_back(intArrayRes->atFloat64(i));
                                                break;
                                        }
                                    }
                                    result = res;
                                } else {
                                    wrn("Не удаётся преобразовать Массив в массив целых чисел.");
                                }
                                break;
                            }
                            case SmartArraySubType::Float: {
                                if (auto floatArrayRes = resultArray->asFloatArray(); floatArrayRes) {
                                    if (requirement.floatSubType &&
                                        floatArrayRes->floatSubType() != *requirement.floatSubType) {
                                        auto res = SmartFloatArrayObject::create(*requirement.floatSubType, smartClass);
                                        for (size_t i = 0; i < resultArray->size(); ++i) {
                                            switch (*requirement.floatSubType) {
                                                case SmartFloatSubType::Float32:
                                                    res->push_back(floatArrayRes->atFloat32(i));
                                                    break;
                                                case SmartFloatSubType::Float64:
                                                    res->push_back(floatArrayRes->atFloat64(i));
                                                    break;
                                            }
                                        }
                                        result = res;
                                    }
                                } else {
                                    wrn("Не удаётся преобразовать Массив в массив целых чисел.");
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } else {
            wrn("Ожидается тип '"s + to_string(*requirement.objectType) + "', но обнаружен '" +
                to_string(result->smartType()) + "'.");
        }
        return result;
    }

    shared_ptr<SmartObject> &SmartFromJson::getStringObject(string &smartClass, shared_ptr<SmartObject> &result,
                                                            const SmartTypeRequirement &requirement) {
        switch (result->smartType()) {
            case SmartType::Int:
                switch (*requirement.stringSubType) {
                    case SmartStringSubType::String:
                        result = SmartStringObject::create<char>(std::to_string((int64_t) *result->asInt()),
                                                                 smartClass);
                        break;
                    case SmartStringSubType::WString:
                        result = SmartStringObject::create<wchar_t>(to_wstring((int64_t) *result->asInt()), smartClass);
                        break;
                }
                break;
            case SmartType::Float:
                switch (*requirement.stringSubType) {
                    case SmartStringSubType::String:
                        result = SmartStringObject::create<char>(std::to_string((double) *result->asFloat()),
                                                                 smartClass);
                        break;
                    case SmartStringSubType::WString:
                        result = SmartStringObject::create<wchar_t>(to_wstring((double) *result->asFloat()),
                                                                    smartClass);
                        break;
                }
                break;
            case SmartType::String: {
                auto resultStr = result->asString();
                if (requirement.stringSubType && *requirement.stringSubType != resultStr->stringSubType()) {
                    switch (*requirement.stringSubType) {
                        case SmartStringSubType::String:
                            result = SmartStringObject::create<char>(resultStr->asCStr(), smartClass);
                            break;
                        case SmartStringSubType::WString:
                            result = SmartStringObject::create<wchar_t>(resultStr->asWStr(), smartClass);
                            break;
                    }
                }
                break;
            }
            case SmartType::Array:
            case SmartType::Map:
                if (result->smartType() != *requirement.objectType)
                    wrn("Ожидается тип '"s + to_string(*requirement.objectType) + "', но обнаружен '" +
                        to_string(result->smartType()) + "'.");
                break;
        }
        return result;
    }

    shared_ptr<SmartObject> &SmartFromJson::getFloatObject(string &smartClass, shared_ptr<SmartObject> &result,
                                                           const SmartTypeRequirement &requirement) {
        switch (result->smartType()) {
            case SmartType::Int:
                switch (*requirement.floatSubType) {
                    case SmartFloatSubType::Float32:
                        result = SmartFloatObject::create((float) *result->asInt(), smartClass);
                        break;
                    case SmartFloatSubType::Float64:
                        result = SmartFloatObject::create((double) *result->asInt(), smartClass);
                        break;
                }
                break;
            case SmartType::Float:
                switch (*requirement.floatSubType) {
                    case SmartFloatSubType::Float32:
                        result = SmartFloatObject::create((float) *result->asFloat(), smartClass);
                        break;
                    case SmartFloatSubType::Float64:
                        result = SmartFloatObject::create((double) *result->asFloat(), smartClass);
                        break;
                }
                break;
            case SmartType::String:
                try {
                    switch (*requirement.floatSubType) {
                        case SmartFloatSubType::Float32:
                            result = SmartFloatObject::create(stof(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartFloatSubType::Float64:
                            result = SmartFloatObject::create(stod(result->asString()->asCStr()), smartClass);
                            break;
                    }
                } catch (exception &e) {
                    wrn("Не удаётся преобразовать строку в действительное число по причине: "s + e.what());
                }
                break;
            case SmartType::Array:
            case SmartType::Map:
                if (result->smartType() != *requirement.objectType)
                    wrn("Ожидается тип '"s + to_string(*requirement.objectType) + "', но обнаружен '" +
                        to_string(result->smartType()) + "'.");
                break;
        }
        return result;
    }

    shared_ptr<SmartObject> &SmartFromJson::getIntObject(string &smartClass, shared_ptr<SmartObject> &result,
                                                         const SmartTypeRequirement &requirement) {
        switch (result->smartType()) {
            case SmartType::Int:
                switch (*requirement.intSubType) {
                    case SmartIntSubType::SInt8:
                        result = SmartIntObject::create((int8_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::SInt16:
                        result = SmartIntObject::create((int16_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::SInt32:
                        result = SmartIntObject::create((int32_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::SInt64:
                        result = SmartIntObject::create((int64_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::UInt8:
                        result = SmartIntObject::create((uint8_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::UInt16:
                        result = SmartIntObject::create((uint16_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::UInt32:
                        result = SmartIntObject::create((uint32_t) *result->asInt(), smartClass);
                        break;
                    case SmartIntSubType::UInt64:
                        result = SmartIntObject::create((uint64_t) *result->asInt(), smartClass);
                        break;
                }
                break;
            case SmartType::Float:
                wrn("Невозможно преобразовать действительное число в целое без потери данных.");
                break;
            case SmartType::String:
                try {
                    switch (*requirement.intSubType) {
                        case SmartIntSubType::SInt8:
                            result = SmartIntObject::create((int8_t) stol(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::SInt16:
                            result = SmartIntObject::create((int16_t) stol(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::SInt32:
                            result = SmartIntObject::create((int32_t) stoll(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::SInt64:
                            result = SmartIntObject::create((int64_t) stoll(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::UInt8:
                            result = SmartIntObject::create((uint8_t) stoul(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::UInt16:
                            result = SmartIntObject::create((uint16_t) stoul(result->asString()->asCStr()), smartClass);
                            break;
                        case SmartIntSubType::UInt32:
                            result = SmartIntObject::create((uint32_t) stoull(result->asString()->asCStr()),
                                                            smartClass);
                            break;
                        case SmartIntSubType::UInt64:
                            result = SmartIntObject::create((uint64_t) stoull(result->asString()->asCStr()),
                                                            smartClass);
                            break;
                    }
                } catch (exception &e) {
                    wrn("Не удаётся преобразовать строку в действительное число по причине: "s + e.what());
                }
                break;
            case SmartType::Array:
            case SmartType::Map:
                if (result->smartType() != *requirement.objectType)
                    wrn("Ожидается тип '"s + to_string(*requirement.objectType) + "', но обнаружен '" +
                        to_string(result->smartType()) + "'.");
                break;
        }
        return result;
    }

    SmartLogRecord::SmartLogRecord(SmartLogLevel logLevel, string message) : logLevel(logLevel),
                                                                             message(move(message)) {}

    string to_string(SmartLogLevel logLevel) {
        switch (logLevel) {
            case SmartLogLevel::Error:
                return "Error"s;
            case SmartLogLevel::Warning:
                return "Warning"s;
            case SmartLogLevel::Info:
                return "Info"s;
            default:
                return "Unknown"s;
        }
    }

    SmartLogRecord::operator string() const {
        stringstream ss;
        ss << to_string(logLevel) << ": " << message;
        return ss.str();
    }
}