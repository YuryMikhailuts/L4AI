//
// Created by unknown on 22.03.2021.
//

#include <smart_to_json.h>

namespace l4ai::smart {

    SmartToJson::SmartToJson(std::ostream &out) : out(out), tab() {}

    void SmartToJson::save(const std::shared_ptr<SmartObject> &smartObject) {
        switch (smartObject->smartType()) {
            case SmartType::Int:
                saveImpl(smartObject->asInt());
                break;
            case SmartType::Float:
                saveImpl(smartObject->asFloat());
                break;
            case SmartType::String:
                saveImpl(smartObject->asString());
                break;
            case SmartType::Array:
                saveImpl(smartObject->asArray());
                break;
            case SmartType::Map:
                saveImpl(smartObject->asMap());
                break;
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartIntObject> &smartInt) {
        switch (smartInt->intSubType()) {
            case SmartIntSubType::SInt8:
            case SmartIntSubType::SInt16:
            case SmartIntSubType::SInt32:
            case SmartIntSubType::SInt64:
                if (smartInt->smartClass.empty()) {
                    out << (int64_t) *smartInt;
                } else {
                    out << "{\n";
                    tab_inc();
                    out << tab << R"("@class": ")" << smartInt->smartClass << "\",\n";
                    out << tab << R"("@value": )" << (int64_t) *smartInt << "\n";
                    tab_dec();
                    out << tab << "}";
                }
                break;
            case SmartIntSubType::UInt8:
            case SmartIntSubType::UInt16:
            case SmartIntSubType::UInt32:
            case SmartIntSubType::UInt64:
                if (smartInt->smartClass.empty()) {
                    out << (uint64_t) *smartInt;
                } else {
                    out << "{\n";
                    tab_inc();
                    out << tab << R"("@class": ")" << smartInt->smartClass << "\",\n";
                    out << tab << R"("@value": )" << (uint64_t) *smartInt << "\n";
                    tab_dec();
                    out << tab << "}";
                }
                break;
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartFloatObject> &smartFloat) {
        switch (smartFloat->floatSubType()) {
            case SmartFloatSubType::Float32:
                if (smartFloat->smartClass.empty()) {
                    out << (float) *smartFloat;
                } else {
                    out << "{\n";
                    tab_inc();
                    out << tab << R"("@class": ")" << smartFloat->smartClass << "\",\n";
                    out << tab << R"("@value": )" << (float) *smartFloat << "\n";
                    tab_dec();
                    out << tab << "}";
                }
                break;
            case SmartFloatSubType::Float64:
                if (smartFloat->smartClass.empty()) {
                    out << (double) *smartFloat;
                } else {
                    out << "{\n";
                    tab_inc();
                    out << tab << R"("@class": ")" << smartFloat->smartClass << "\",\n";
                    out << tab << R"("@value": )" << (double) *smartFloat << "\n";
                    tab_dec();
                    out << tab << "}";
                }
                break;
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartArrayObject> &smartArray) {
        switch (smartArray->arraySubType()) {
            case SmartArraySubType::Object:
                saveImpl(smartArray->asObjectArray());
                break;
            case SmartArraySubType::Int:
                saveImpl(smartArray->asIntArray());
                break;
            case SmartArraySubType::Float:
                saveImpl(smartArray->asFloatArray());
                break;
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartMapObject> &smartMap) {
        out << tab << "{\n";
        tab_inc();
        if (!smartMap->smartClass.empty()) {
            out << tab << R"("@class": ")" << smartMap->smartClass << "\",\n";
        }
        auto &data = smartMap->data;
        for (auto itr = data.begin(); itr != data.end(); ++itr) {
            auto item = *itr;
            out << tab << "\"" << item.first << "\": ";
            save(item.second);
            if (auto itr2 = itr; ++itr2 != data.end()) {
                out << ",\n";
            } else {
                out << "\n";
            }
        }
        tab_dec();
        out << tab << "}\n";
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartObjectArray> &smartArray) {
        bool onSmartClass = !smartArray->smartClass.empty();
        if (onSmartClass) {
            out << tab << "{\n";
            out << tab << R"("@class": ")" << smartArray->smartClass << "\",\n";
            out << tab << R"("@value": )";
            tab_inc();
        }
        out << tab << "[\n";
        tab_inc();
        auto &data = smartArray->data;
        for (auto itr = data.begin(); itr != data.end(); ++itr) {
            auto item = *itr;
            out << tab;
            save(item);
            if (auto itr2 = itr; ++itr2 != data.end()) {
                out << ",\n";
            } else {
                out << "\n";
            }
        }
        tab_dec();
        out << tab << "]\n";
        if (onSmartClass) {
            tab_dec();
            out << tab << "}";
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartStringObject> &smartString) {
        out << "\"" << smartString->asCStr() << "\"";
    }

    void SmartToJson::tab_inc() {
        tab += "\t";
    }

    void SmartToJson::tab_dec() {
        tab = tab.substr(0, tab.length() - 1);
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartIntArrayObject> &smartArray) {
        bool onSmartClass = !smartArray->smartClass.empty();
        if (onSmartClass) {
            out << "{\n";
            tab_inc();
            out << tab << R"("@class": ")" << smartArray->smartClass << "\",\n";
            out << tab << R"("@value": )";
        }
        out << "[ ";
        tab_inc();
        switch (smartArray->intSubType()) {
            case SmartIntSubType::SInt8:
            case SmartIntSubType::SInt16:
            case SmartIntSubType::SInt32:
            case SmartIntSubType::SInt64:
                for (size_t i = 0; i < smartArray->size(); ++i) {
                    auto item = smartArray->atInt64(i);
                    out << item;
                    if (i + 1 != smartArray->size()) {
                        out << ", ";
                    }
                }
                break;
            case SmartIntSubType::UInt8:
            case SmartIntSubType::UInt16:
            case SmartIntSubType::UInt32:
            case SmartIntSubType::UInt64:
                for (size_t i = 0; i < smartArray->size(); ++i) {
                    auto item = smartArray->atUInt64(i);
                    out << item;
                    if (i + 1 != smartArray->size()) {
                        out << ", ";
                    }
                }
                break;
        }
        tab_dec();
        out << "]";
        if (onSmartClass) {
            tab_dec();
            out << "\n" << tab << "}";
        }
    }

    void SmartToJson::saveImpl(const std::shared_ptr<SmartFloatArrayObject> &smartArray) {
        bool onSmartClass = !smartArray->smartClass.empty();
        if (onSmartClass) {
            out << "{\n";
            tab_inc();
            out << tab << R"("@class": ")" << smartArray->smartClass << "\",\n";
            out << tab << R"("@value": )";
        }
        out << "[ ";
        tab_inc();
        switch (smartArray->floatSubType()) {
            case SmartFloatSubType::Float32:
                for (size_t i = 0; i < smartArray->size(); ++i) {
                    auto item = smartArray->atFloat32(i);
                    out << item;
                    if (i + 1 != smartArray->size()) {
                        out << ", ";
                    }
                }
                break;
            case SmartFloatSubType::Float64:
                for (size_t i = 0; i < smartArray->size(); ++i) {
                    auto item = smartArray->atFloat64(i);
                    out << item;
                    if (i + 1 != smartArray->size()) {
                        out << ", ";
                    }
                }
                break;
        }
        tab_dec();
        out << "]";
        if (onSmartClass) {
            tab_dec();
            out << "\n" << tab << "}";
        }
    }

}