/*
 * smart_from_json.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_FROM_JSON_H
#define LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_FROM_JSON_H

#include <iostream>
#include <list>
#include <smart_objects.h>

namespace l4ai::smart {

    enum class SmartLogLevel {
        Error, Warning, Info
    };

    struct SmartLogRecord {
        SmartLogLevel logLevel;
        std::string message;

        SmartLogRecord(SmartLogLevel logLevel, std::string message);

        explicit operator std::string() const;
    };

    struct SmartTypeRequirement {
        const std::optional<SmartType> objectType;
        const std::optional<SmartIntSubType> intSubType;
        const std::optional<SmartFloatSubType> floatSubType;
        const std::optional<SmartStringSubType> stringSubType;
        const std::optional<SmartArraySubType> arraySubType;

        explicit SmartTypeRequirement(SmartType objectType) : objectType(objectType), intSubType(), floatSubType(),
                                                              stringSubType(), arraySubType() {}

        explicit SmartTypeRequirement(SmartArraySubType arraySubType) : objectType(SmartType::Array), intSubType(),
                                                                        floatSubType(), stringSubType(),
                                                                        arraySubType(arraySubType) {}

        SmartTypeRequirement(SmartIntSubType intSubType, bool isArray)
                : objectType(isArray ? SmartType::Array : SmartType::Int), intSubType(intSubType), floatSubType(),
                  stringSubType(), arraySubType(isArray ? std::optional(SmartArraySubType::Int) : std::nullopt) {}

        SmartTypeRequirement(SmartFloatSubType floatSubType, bool isArray)
                : objectType(isArray ? SmartType::Array : SmartType::Float), intSubType(), floatSubType(floatSubType),
                  stringSubType(), arraySubType(isArray ? std::optional(SmartArraySubType::Float) : std::nullopt) {}

        explicit SmartTypeRequirement(SmartStringSubType stringSubType)
                : objectType(SmartType::String), intSubType(), floatSubType(),
                  stringSubType(stringSubType), arraySubType() {}
    };

    class SmartFromJson {
    private:
        std::istream &in;
        size_t row_num;
        size_t col_num;
        std::list<SmartLogRecord> logList;
        size_t errors_count;
        size_t warnings_count;
        std::optional<char> last_peek, last_getc;

        void log(SmartLogLevel logLevel, std::string_view text);

        inline void err(std::string_view text) { log(SmartLogLevel::Error, text); }

        inline void wrn(std::string_view text) { log(SmartLogLevel::Warning, text); }

        inline void inf(std::string_view text) { log(SmartLogLevel::Info, text); }

        int peek();

        int getc();

        /**
         * Парсит массив Json
         * @return
         */
        [[nodiscard]] std::shared_ptr<SmartArrayObject> getArray(std::string_view needSmartClass = "");

        [[nodiscard]] std::shared_ptr<SmartMapObject> getMap(std::string_view needSmartClass = "");

        [[nodiscard]] std::shared_ptr<SmartObject> getNumber(std::string_view needSmartClass = "");

        [[nodiscard]] std::shared_ptr<SmartStringObject> getString(std::string_view needSmartClass = "");

        /**
         * С данногометода начинается парсинг любого Json объекта.
         * Ничего особенного данный метод не делает. Он лишь определяет какого типа конкретно объект ожидается по следующему символу, который лежит в потоке.
         * Сам символ, при этом, просто проверяется, но не достаётся из потока.
         * @return
         */
        [[nodiscard]] std::shared_ptr<SmartObject> getObject(std::string_view needSmartClass = "");

        [[nodiscard]] bool can_read() const;

        std::string getMapFieldName();

        uint64_t getInt();

        double getFloat(double value = 0);

        bool nextChar(char c, char ignore = '\0');

        bool is_digit();

        std::shared_ptr<SmartObject> checkClassObject(const std::shared_ptr<SmartMapObject> &sharedPtr);

        std::shared_ptr<SmartObject> &getIntObject(std::string &smartClass, std::shared_ptr<SmartObject> &result,
                                                   const SmartTypeRequirement &requirement);

        std::shared_ptr<SmartObject> &getFloatObject(std::string &smartClass, std::shared_ptr<SmartObject> &result,
                                                     const SmartTypeRequirement &requirement);

    public:
        std::unordered_map<std::string, SmartTypeRequirement> classTypes;

        [[nodiscard]] std::shared_ptr<SmartObject> get();

        explicit SmartFromJson(std::istream &in);

        std::shared_ptr<SmartObject> &
        getStringObject(std::string &smartClass, std::shared_ptr<SmartObject> &result,
                        const SmartTypeRequirement &requirement);

        std::shared_ptr<SmartObject> &
        getArrayObject(std::string &smartClass, std::shared_ptr<SmartObject> &result,
                       SmartTypeRequirement &requirement);

        std::istream &getIn() const;

        size_t getRowNum() const;

        size_t getColNum() const;

        const std::list<SmartLogRecord> &getLogList() const;

        size_t getErrorsCount() const;

        size_t getWarningsCount() const;

        const std::optional<char> &getLastPeek() const;

        const std::optional<char> &getLastGetc() const;

        const std::unordered_map<std::string, SmartTypeRequirement> &getClassTypes() const;
    };

}

#endif //LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_FROM_JSON_H
