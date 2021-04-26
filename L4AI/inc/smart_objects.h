/*
 * smart_objects.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_OBJECTS_H
#define LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_OBJECTS_H
#include <L4AI_global.h>
#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <codecvt>
#include <locale>
#include <optional>

namespace l4ai::smart {

    enum class SmartType {
        Int, Float, String, Array, Map
    };
    enum class SmartIntSubType {
        SInt8, SInt16, SInt32, SInt64, UInt8, UInt16, UInt32, UInt64
    };
    enum class SmartFloatSubType {
        Float32, Float64
    };
    enum class SmartStringSubType {
        String, WString
    };
    enum class SmartArraySubType {
        Object, Int, Float
    };

    std::string to_string(SmartType smartType);

    std::string to_string(SmartIntSubType smartIntSubType);

    std::string to_string(SmartFloatSubType smartFloatSubType);

    std::string to_string(SmartStringSubType smartStringSubType);

    std::string to_string(SmartArraySubType smartArraySubType);

    inline constexpr bool operator>(SmartIntSubType l, SmartIntSubType r);

    inline constexpr bool operator>=(SmartIntSubType l, SmartIntSubType r);

    inline constexpr bool operator<(SmartIntSubType l, SmartIntSubType r);

    inline constexpr bool operator<=(SmartIntSubType l, SmartIntSubType r);

    inline constexpr SmartFloatSubType minFloatType(SmartIntSubType intSubType);

    inline constexpr bool operator>(SmartFloatSubType l, SmartFloatSubType r);

    inline constexpr bool operator>=(SmartFloatSubType l, SmartFloatSubType r);

    inline constexpr bool operator<(SmartFloatSubType l, SmartFloatSubType r);

    inline constexpr bool operator<=(SmartFloatSubType l, SmartFloatSubType r);

    template<typename T>
    inline static constexpr bool is_signed_int =
            std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t> ||
            std::is_same_v<T, int64_t>;
    template<typename T>
    inline static constexpr bool is_unsigned_int =
            std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, uint64_t>;
    template<typename T>
    inline static constexpr bool is_int = is_signed_int<T> || is_unsigned_int<T>;
    template<typename T>
    inline static constexpr bool is_float = std::is_same_v<T, float> || std::is_same_v<T, double>;
    template<typename T>
    inline static constexpr bool is_char = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;

    struct L4AI_EXPORT SmartObject : public std::enable_shared_from_this<SmartObject> {
        std::string smartClass;

        explicit SmartObject(std::string_view smartClass = "");

        virtual ~SmartObject();

        [[nodiscard]] virtual SmartType smartType() const = 0;

        template<typename TSmartObject, typename = std::enable_if_t<std::is_base_of_v<SmartObject, TSmartObject>>>
        inline static constexpr std::shared_ptr<TSmartObject> create(std::string_view smartClass = "") {
            return std::make_shared<TSmartObject>(smartClass);
        }

        std::shared_ptr<class SmartIntObject> asInt();

        std::shared_ptr<class SmartFloatObject> asFloat();

        std::shared_ptr<class SmartStringObject> asString();

        std::shared_ptr<class SmartArrayObject> asArray();

        std::shared_ptr<class SmartMapObject> asMap();

        virtual bool operator==(const SmartObject &other) const = 0;

        virtual bool operator!=(const SmartObject &other) const {
            return !(*this == other);
        }

    };


    struct L4AI_EXPORT SmartIntObject : public SmartObject {
        explicit SmartIntObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] virtual SmartIntSubType intSubType() const = 0;

        explicit virtual operator int8_t() const = 0;

        explicit virtual operator int16_t() const = 0;

        explicit virtual operator int32_t() const = 0;

        explicit virtual operator int64_t() const = 0;

        explicit virtual operator uint8_t() const = 0;

        explicit virtual operator uint16_t() const = 0;

        explicit virtual operator uint32_t() const = 0;

        explicit virtual operator uint64_t() const = 0;

        explicit virtual operator float() const { return (int64_t) *this; };

        explicit virtual operator double() const { return (int64_t) *this; };

        template<typename TInt>
        static std::shared_ptr<SmartIntObject> create(TInt value, std::string_view smartClass = "");
    };

    template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
    struct SmartInt : public SmartIntObject {
        explicit SmartInt(std::string_view smartClass = "") : SmartIntObject(smartClass) {}

        [[nodiscard]] SmartType smartType() const override { return SmartType::Int; }

        [[nodiscard]] SmartIntSubType intSubType() const override;

        TInt value;

        SmartInt &operator=(const TInt &i) {
            value = i;
            return *this;
        }

        SmartInt &operator=(TInt &&i) {
            value = i;
            return *this;
        }

        explicit operator int8_t() const override { return value; }

        explicit operator int16_t() const override { return value; }

        explicit operator int32_t() const override { return value; }

        explicit operator int64_t() const override { return value; }

        explicit operator uint8_t() const override { return value; }

        explicit operator uint16_t() const override { return value; }

        explicit operator uint32_t() const override { return value; }

        explicit operator uint64_t() const override { return value; }

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Int) return false;
            return this->operator int64_t() == dynamic_cast<const SmartIntObject &>(other).operator int64_t() &&
                   smartClass == other.smartClass;
        }
    };

    template<typename TInt>
    std::shared_ptr<SmartIntObject> SmartIntObject::create(TInt value, std::string_view smartClass) {
        auto result = SmartObject::create<SmartInt<TInt>>(smartClass);
        *result = value;
        return result;
    }

    struct L4AI_EXPORT SmartFloatObject : public SmartObject {
        explicit SmartFloatObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] virtual SmartFloatSubType floatSubType() const = 0;

        explicit virtual operator float() const = 0;

        explicit virtual operator double() const = 0;

        template<typename TFloat>
        static std::shared_ptr<SmartFloatObject> create(TFloat value, std::string_view smartClass = "");
    };

    template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
    struct SmartFloat : public SmartFloatObject {
        explicit SmartFloat(std::string_view smartClass = "") : SmartFloatObject(smartClass) {}

        [[nodiscard]] SmartType smartType() const override { return SmartType::Float; }

        [[nodiscard]] SmartFloatSubType floatSubType() const override;

        TFloat value;

        SmartFloat &operator=(const TFloat &i) {
            value = i;
            return *this;
        }

        SmartFloat &operator=(TFloat &&i) {
            value = i;
            return *this;
        }

        explicit operator float() const override { return value; }

        explicit operator double() const override { return value; }

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Float) return false;
            return this->operator double() == dynamic_cast<const SmartFloatObject &>(other).operator double() &&
                   smartClass == other.smartClass;
        }
    };

    template<typename TFloat>
    std::shared_ptr<SmartFloatObject> SmartFloatObject::create(TFloat value, std::string_view smartClass) {
        auto result = SmartObject::create<SmartFloat<TFloat>>(smartClass);
        *result = value;
        return result;
    }


    struct L4AI_EXPORT SmartStringObject : public SmartObject {
        explicit SmartStringObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] virtual SmartStringSubType stringSubType() const = 0;

        virtual std::string asCStr() const = 0;

        virtual std::wstring asWStr() const = 0;

        template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
        static std::shared_ptr<SmartStringObject>
        create(std::basic_string_view<TChar> text, std::string_view smartClassName = "");

    };

    template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
    struct SmartString : public SmartStringObject {
        using string_view_t = std::basic_string_view<TChar>;
        using string_t = std::basic_string<TChar>;

        explicit SmartString(std::string_view smartClass = "") : SmartStringObject(smartClass) {}

        [[nodiscard]] SmartType smartType() const override { return SmartType::String; }

        [[nodiscard]] SmartStringSubType stringSubType() const override;

        string_t value;

        std::string asCStr() const override {
            if constexpr (std::is_same_v<char, TChar>) {
                return value;
            } else if constexpr (std::is_same_v<wchar_t, TChar>) {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                return converter.to_bytes(value);
            } else {
                static_assert(std::is_same_v<char, TChar> || std::is_same_v<wchar_t, TChar>,
                              "TChar должен быть char или wchar_t.");
            }
        }

        std::wstring asWStr() const override {
            if constexpr (std::is_same_v<char, TChar>) {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                return converter.from_bytes(value);
            } else if constexpr (std::is_same_v<wchar_t, TChar>) {
                return value;
            } else {
                static_assert(std::is_same_v<char, TChar> || std::is_same_v<wchar_t, TChar>,
                              "TChar должен быть char или wchar_t.");
            }
        }

        SmartString &operator=(string_view_t i) {
            value = i;
            return *this;
        }

        SmartString &operator=(const string_t &i) {
            value = i;
            return *this;
        }

        SmartString &operator=(string_t &&i) {
            value = i;
            return *this;
        }

        explicit operator string_t() const {
            return value;
        }

        explicit operator string_view_t() const {
            return value;
        }

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::String) return false;
            const auto &otherStringObject = dynamic_cast<const SmartStringObject &>(other);
            if (otherStringObject.stringSubType() == stringSubType()) {
                const auto &otherString = dynamic_cast<const SmartString<TChar> &>(otherStringObject);
                return value == otherString.value && smartClass == other.smartClass;
            } else {
                return asWStr() == otherStringObject.asWStr() && smartClass == other.smartClass;
            }
        }

    };

    template<typename TChar, typename>
    std::shared_ptr<SmartStringObject>
    SmartStringObject::create(std::basic_string_view<TChar> text, std::string_view smartClassName) {
        auto result = SmartObject::create<SmartString<TChar>>(smartClassName);
        *result = text;
        return result;
    }


    struct L4AI_EXPORT SmartArrayObject : public SmartObject {
        explicit SmartArrayObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] SmartType smartType() const override { return SmartType::Array; }

        [[nodiscard]] virtual SmartArraySubType arraySubType() const = 0;

        virtual std::shared_ptr<SmartObject> at(size_t index) = 0;

        virtual size_t size() const = 0;

        virtual void resize(size_t size) = 0;

        std::shared_ptr<class SmartObjectArray> asObjectArray();

        std::shared_ptr<class SmartIntArrayObject> asIntArray();

        std::shared_ptr<class SmartFloatArrayObject> asFloatArray();

        virtual void get(int8_t *target, size_t target_size) const = 0;

        virtual void get(int16_t *target, size_t target_size) const = 0;

        virtual void get(int32_t *target, size_t target_size) const = 0;

        virtual void get(int64_t *target, size_t target_size) const = 0;

        virtual void get(uint8_t *target, size_t target_size) const = 0;

        virtual void get(uint16_t *target, size_t target_size) const = 0;

        virtual void get(uint32_t *target, size_t target_size) const = 0;

        virtual void get(uint64_t *target, size_t target_size) const = 0;

        virtual void get(float *target, size_t target_size) const = 0;

        virtual void get(double *target, size_t target_size) const = 0;

    };

    template<typename TInt, typename = std::enable_if_t<is_int<TInt>>> class SmartIntArray;
    template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>> class SmartFloatArray;

    struct L4AI_EXPORT SmartObjectArray : public SmartArrayObject {
        explicit SmartObjectArray(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}

        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Object; };
        std::vector<std::shared_ptr<SmartObject>> data;

        inline std::shared_ptr<SmartObject> operator[](size_t index) { return data[index]; }

        std::shared_ptr<SmartObject> at(size_t index) override {
            return (*this)[index];
        }

        size_t size() const override { return data.size(); }

        void resize(size_t size) override;

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartInt<TInt>> addInt(TInt value, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartInt<TInt>>(smartClass);
            data.push_back(result);
            *result = value;
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloat<TFloat>> addFloat(TFloat value, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartFloat<TFloat>>(smartClass);
            data.push_back(result);
            *result = value;
            return result;
        }

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartIntArray<TInt>> addIntArray(std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartIntArray<TInt>>(smartClass);
            data.template emplace_back(result);
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloatArray<TFloat>> addFloatArray(std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartFloatArray<TFloat>>(smartClass);
            data.template emplace_back(result);
            return result;
        }

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartIntArray<TInt>> addIntArray(TInt* value, size_t size, std::string_view smartClass = "") {
            auto result = addIntArray<TInt>(smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TInt));
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloatArray<TFloat>> addFloatArray(TFloat* value, size_t size, std::string_view smartClass = "") {
            auto result = addFloatArray<TFloat>(smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TFloat));
            return result;
        }

        template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
        std::shared_ptr<SmartString<TChar>> addString(typename SmartString<TChar>::string_view_t text, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartString<TChar>>(smartClass);
            data.template emplace_back(result);
            *result = std::string(text);
            return result;
        }

        std::shared_ptr<SmartObjectArray> addObjectArray(std::string_view smartClass = "");
        std::shared_ptr<class SmartMapObject> addMap(std::string_view smartClass = "");


        void get(int8_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(int16_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(int32_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(int64_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(uint8_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(uint16_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(uint32_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(uint64_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(float *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void get(double *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        std::optional<SmartIntSubType> canConvertToIntArray();

        std::optional<SmartFloatSubType> canConvertToFloatArray();

        std::shared_ptr<SmartIntArrayObject>
        convertToIntArray(std::optional<SmartIntSubType> intSubType = std::nullopt);

        std::shared_ptr<SmartFloatArrayObject>
        convertToFloatArray(std::optional<SmartFloatSubType> floatSubType = std::nullopt);

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Array) return false;
            const auto &otherArrayObject = dynamic_cast<const SmartArrayObject &>(other);
            if (otherArrayObject.arraySubType() == arraySubType()) {
                const auto &otherObjectArray = dynamic_cast<const SmartObjectArray &>(otherArrayObject);
                if (data.size() != otherObjectArray.size()) return false;
                for (size_t i = 0; i < data.size(); ++i)
                    if (*data[i] != *otherObjectArray.data[i])
                        return false;
                return smartClass == other.smartClass;
            } else {
                return false;
            }
        }

    };

    struct L4AI_EXPORT SmartIntArrayObject : public SmartArrayObject {
        explicit SmartIntArrayObject(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}

        [[nodiscard]] virtual SmartIntSubType intSubType() const = 0;

        virtual void set(size_t, int8_t) = 0;

        virtual void set(size_t, int16_t) = 0;

        virtual void set(size_t, int32_t) = 0;

        virtual void set(size_t, int64_t) = 0;

        virtual void set(size_t, uint8_t) = 0;

        virtual void set(size_t, uint16_t) = 0;

        virtual void set(size_t, uint32_t) = 0;

        virtual void set(size_t, uint64_t) = 0;

        virtual void push_back(int8_t) = 0;

        virtual void push_back(int16_t) = 0;

        virtual void push_back(int32_t) = 0;

        virtual void push_back(int64_t) = 0;

        virtual void push_back(uint8_t) = 0;

        virtual void push_back(uint16_t) = 0;

        virtual void push_back(uint32_t) = 0;

        virtual void push_back(uint64_t) = 0;

        virtual int8_t atInt8(size_t index) const = 0;

        virtual int16_t atInt16(size_t index) const = 0;

        virtual int32_t atInt32(size_t index) const = 0;

        virtual int64_t atInt64(size_t index) const = 0;

        virtual uint8_t atUInt8(size_t index) const = 0;

        virtual uint16_t atUInt16(size_t index) const = 0;

        virtual uint32_t atUInt32(size_t index) const = 0;

        virtual uint64_t atUInt64(size_t index) const = 0;

        virtual float atFloat32(size_t index) const = 0;

        virtual double atFloat64(size_t index) const = 0;

        static std::shared_ptr<SmartIntArrayObject>
        create(SmartIntSubType subType, std::string_view smartClassName = "");
    };

    template<typename TInt>
    struct SmartIntArray<TInt> : public SmartIntArrayObject {
        explicit SmartIntArray(std::string_view smartClass = "") : SmartIntArrayObject(smartClass) {}

        std::shared_ptr<SmartObject> at(size_t index) override {
            auto result = SmartObject::create<SmartInt<TInt>>();
            *result = (*this)[index];
            return result;
        }

        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Int; };
        std::vector<TInt> data;

        [[nodiscard]] SmartIntSubType intSubType() const override;

        inline TInt &operator[](size_t index) { return data[index]; }

        inline const TInt &operator[](size_t index) const { return data[index]; }

        size_t size() const override { return data.size(); }

        void resize(size_t size) override;

        void set(size_t index, int8_t int8) override;

        void set(size_t index, int16_t int16) override;

        void set(size_t index, int32_t int32) override;

        void set(size_t index, int64_t int64) override;

        void set(size_t index, uint8_t uint8) override;

        void set(size_t index, uint16_t uint16) override;

        void set(size_t index, uint32_t uint32) override;

        void set(size_t index, uint64_t uint64) override;

        void push_back(int8_t int8) override;

        void push_back(int16_t int16) override;

        void push_back(int32_t int32) override;

        void push_back(int64_t int64) override;

        void push_back(uint8_t uint8) override;

        void push_back(uint16_t uint16) override;

        void push_back(uint32_t uint32) override;

        void push_back(uint64_t uint64) override;

        int8_t atInt8(size_t index) const override { return data[index]; }

        int16_t atInt16(size_t index) const override { return data[index]; }

        int32_t atInt32(size_t index) const override { return data[index]; }

        int64_t atInt64(size_t index) const override { return data[index]; }

        uint8_t atUInt8(size_t index) const override { return data[index]; }

        uint16_t atUInt16(size_t index) const override { return data[index]; }

        uint32_t atUInt32(size_t index) const override { return data[index]; }

        uint64_t atUInt64(size_t index) const override { return data[index]; }

        float atFloat32(size_t index) const override { return data[index]; }

        double atFloat64(size_t index) const override { return data[index]; }


        void get(int8_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int16_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int32_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int64_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint8_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint16_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint32_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint64_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(float *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) {
                float value = data.at(i);
                target[i] = value;
            }
        }

        void get(double *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) {
                double value = data.at(i);
                target[i] = value;
            }
        }

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Array) return false;
            const auto &otherArrayObject = dynamic_cast<const SmartArrayObject &>(other);
            if (otherArrayObject.arraySubType() == arraySubType()) {
                const auto &otherIntArrayObject = dynamic_cast<const SmartIntArrayObject &>(otherArrayObject);
                if (data.size() != otherIntArrayObject.size()) return false;
                for (size_t i = 0; i < data.size(); ++i)
                    if (data[i] != otherIntArrayObject.atInt64(i))
                        return false;
                return smartClass == other.smartClass;
            } else {
                return false;
            }
        }

    };

    struct L4AI_EXPORT SmartFloatArrayObject : public SmartArrayObject {
        explicit SmartFloatArrayObject(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}

        [[nodiscard]] virtual SmartFloatSubType floatSubType() const = 0;

        virtual float atFloat32(size_t index) const = 0;

        virtual double atFloat64(size_t index) const = 0;

        virtual void set(size_t index, float value) = 0;

        virtual void set(size_t index, double value) = 0;

        virtual void push_back(float value) = 0;

        virtual void push_back(double value) = 0;

        static std::shared_ptr<SmartFloatArrayObject>
        create(SmartFloatSubType subType, std::string_view smartClassName = "");
    };

    template<typename TFloat>
    class SmartFloatArray<TFloat> : public SmartFloatArrayObject {
    public:
        explicit SmartFloatArray(std::string_view smartClass = "") : SmartFloatArrayObject(smartClass) {}

        std::shared_ptr<SmartObject> at(size_t index) override {
            auto result = SmartObject::create<SmartFloat<TFloat>>();
            *result = (*this)[index];
            return result;
        }

        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Float; };
        std::vector<TFloat> data;

        [[nodiscard]] SmartFloatSubType floatSubType() const override;

        inline TFloat &operator[](size_t index) { return data[index]; }

        inline const TFloat &operator[](size_t index) const { return data[index]; }

        size_t size() const override { return data.size(); }

        void resize(size_t size) override;

        float atFloat32(size_t index) const override { return data[index]; }

        double atFloat64(size_t index) const override { return data[index]; }

        void set(size_t index, float value) override;

        void set(size_t index, double value) override;

        void push_back(float value) override;

        void push_back(double value) override;

        void get(int8_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int16_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int32_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(int64_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint8_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint16_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint32_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(uint64_t *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void get(float *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) {
                float value = data.at(i);
                target[i] = value;
            }
        }

        void get(double *target, size_t target_size) const override {
            for (size_t i = 0; i < std::min(target_size, data.size()); ++i) {
                double value = data.at(i);
                target[i] = value;
            }
        }

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Array) return false;
            const auto &otherArrayObject = dynamic_cast<const SmartArrayObject &>(other);
            if (otherArrayObject.arraySubType() == arraySubType()) {
                const auto &otherFloatArrayObject = dynamic_cast<const SmartFloatArrayObject &>(otherArrayObject);
                if (data.size() != otherFloatArrayObject.size()) return false;
                for (size_t i = 0; i < data.size(); ++i)
                    if (data[i] != otherFloatArrayObject.atFloat64(i))
                        return false;
                return smartClass == other.smartClass;
            } else {
                return false;
            }
        }

    };

    constexpr bool operator>(SmartIntSubType l, SmartIntSubType r) {
        switch (l) {
            case SmartIntSubType::UInt8:
                return false;
            case SmartIntSubType::UInt16:
                switch (r) {
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
            case SmartIntSubType::UInt32:
                switch (r) {
                    case SmartIntSubType::UInt16:
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
            case SmartIntSubType::UInt64:
                switch (r) {
                    case SmartIntSubType::UInt32:
                    case SmartIntSubType::UInt16:
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
            case SmartIntSubType::SInt8:
                return false;
            case SmartIntSubType::SInt16:
                switch (r) {
                    case SmartIntSubType::SInt8:
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
            case SmartIntSubType::SInt32:
                switch (r) {
                    case SmartIntSubType::SInt16:
                    case SmartIntSubType::SInt8:
                    case SmartIntSubType::UInt16:
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
            case SmartIntSubType::SInt64:
                switch (r) {
                    case SmartIntSubType::SInt32:
                    case SmartIntSubType::SInt16:
                    case SmartIntSubType::SInt8:
                    case SmartIntSubType::UInt64:
                    case SmartIntSubType::UInt32:
                    case SmartIntSubType::UInt16:
                    case SmartIntSubType::UInt8:
                        return true;
                    default:
                        return false;
                }
        }
        return false;
    }

    inline constexpr bool operator>=(SmartIntSubType l, SmartIntSubType r) {
        return (l > r) || (l == r);
    }

    inline constexpr bool operator<(SmartIntSubType l, SmartIntSubType r) {
        return (r > l);
    }

    inline constexpr bool operator<=(SmartIntSubType l, SmartIntSubType r) {
        return (l < r) || (l == r);
    }

    inline constexpr SmartFloatSubType minFloatType(SmartIntSubType intSubType) {
        switch (intSubType) {
            case SmartIntSubType::SInt8:
            case SmartIntSubType::UInt8:
            case SmartIntSubType::SInt16:
            case SmartIntSubType::UInt16:
                return SmartFloatSubType::Float32;
            case SmartIntSubType::SInt32:
            case SmartIntSubType::UInt32:
            case SmartIntSubType::SInt64:
            case SmartIntSubType::UInt64:
                return SmartFloatSubType::Float64;
            default:
                throw std::runtime_error("Неизвестный целочисленный тип.");
        }
    }

    inline constexpr bool operator>(SmartFloatSubType l, SmartFloatSubType r) {
        switch (l) {
            case SmartFloatSubType::Float32:
                return false;
            case SmartFloatSubType::Float64:
                switch (r) {
                    case SmartFloatSubType::Float32:
                        return true;
                    default:
                        return false;
                }
            default:
                return false;
        }
    }

    inline constexpr bool operator>=(SmartFloatSubType l, SmartFloatSubType r) {
        return (l > r) || (l == r);
    }

    inline constexpr bool operator<(SmartFloatSubType l, SmartFloatSubType r) {
        return (r > l);
    }

    inline constexpr bool operator<=(SmartFloatSubType l, SmartFloatSubType r) {
        return (l < r) || (l == r);
    }

    class L4AI_EXPORT SmartMapObject : public SmartObject {
    public:
        explicit SmartMapObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        SmartType smartType() const override { return SmartType::Map; }

        std::unordered_map<std::string, std::shared_ptr<SmartObject>> data;

        std::shared_ptr<SmartObject> operator[](std::string_view name);

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartInt<TInt>> setInt(std::string_view name, TInt value, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartInt<TInt>>(smartClass);
            data.emplace(std::string(name), result);
            *result = value;
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloat<TFloat>> setFloat(std::string_view name, TFloat value, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartFloat<TFloat>>(smartClass);
            data.emplace(std::string(name), result);
            *result = value;
            return result;
        }

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartIntArray<TInt>> setIntArray(std::string_view name, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartIntArray<TInt>>(smartClass);
            data.emplace(std::string(name), result);
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloatArray<TFloat>> setFloatArray(std::string_view name, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartFloatArray<TFloat>>(smartClass);
            data.emplace(name, result);
            return result;
        }

        template<typename TInt, typename = std::enable_if_t<is_int<TInt>>>
        std::shared_ptr<SmartIntArray<TInt>> setIntArray(std::string_view name, TInt* value, size_t size, std::string_view smartClass = "") {
            auto result = setIntArray<TInt>(name, smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TInt));
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloatArray<TFloat>> setFloatArray(std::string_view name, TFloat* value, size_t size, std::string_view smartClass = "") {
            auto result = setFloatArray<TFloat>(name, smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TFloat));
            return result;
        }

        template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
        std::shared_ptr<SmartString<TChar>>
        setString(std::string_view name, typename SmartString<TChar>::string_view_t text,
                  std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartString<TChar>>(smartClass);
            data.emplace(std::string(name), result);
            *result = std::string(text);
            return result;
        }

        std::shared_ptr<SmartObjectArray> setObjectArray(std::string_view name, std::string_view smartClass = "");

        std::shared_ptr<class SmartMapObject> setMap(std::string_view name, std::string_view smartClass = "");

        bool operator==(const SmartObject &other) const override {
            if (other.smartType() != SmartType::Map) return false;
            const auto &otherMapObject = dynamic_cast<const SmartMapObject &>(other);
            for (const auto &pair : data) {
                if (otherMapObject.data.count(pair.first) == 0) return false;
                if (*pair.second != *otherMapObject.data.at(pair.first)) return false;
            }
            return smartClass == other.smartClass;
        }

    };

    using SmartUint8 = SmartInt<uint8_t>;
    using SmartUint16 = SmartInt<uint16_t>;
    using SmartUint32 = SmartInt<uint32_t>;
    using SmartUint64 = SmartInt<uint64_t>;
    using SmartInt8 = SmartInt<int8_t>;
    using SmartInt16 = SmartInt<int16_t>;
    using SmartInt32 = SmartInt<int32_t>;
    using SmartInt64 = SmartInt<int64_t>;
    using SmartFloat32 = SmartFloat<float>;
    using SmartFloat64 = SmartFloat<double>;
    using SmartCString = SmartString<char>;
    using SmartWString = SmartString<wchar_t>;

}

#endif //LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_OBJECTS_H
