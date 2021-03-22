/*
 * smart_objects.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_OBJECTS_H
#define LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_OBJECTS_H
#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstring>

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

    struct SmartObject : public std::enable_shared_from_this<SmartObject> {
        const std::string smartClass;

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
    };


    struct SmartIntObject : public SmartObject {
        explicit SmartIntObject(std::string_view smartClass = "") : SmartObject(smartClass) {}
        [[nodiscard]] virtual SmartIntSubType intSubType() const = 0;
        explicit virtual operator int8_t () const = 0;
        explicit virtual operator int16_t () const = 0;
        explicit virtual operator int32_t () const = 0;
        explicit virtual operator int64_t () const = 0;
        explicit virtual operator uint8_t () const = 0;
        explicit virtual operator uint16_t () const = 0;
        explicit virtual operator uint32_t () const = 0;
        explicit virtual operator uint64_t () const = 0;
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

        explicit operator int8_t() const override  { return value; }

        explicit operator int16_t() const override { return value; }

        explicit operator int32_t() const override { return value; }

        explicit operator int64_t() const override { return value; }

        explicit operator uint8_t() const override { return value; }

        explicit operator uint16_t() const override { return value; }

        explicit operator uint32_t() const override { return value; }

        explicit operator uint64_t() const override { return value; }
    };

    struct SmartFloatObject : public SmartObject {
        explicit SmartFloatObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] virtual SmartFloatSubType floatSubType() const = 0;
        explicit virtual operator float() const = 0;
        explicit virtual operator double() const = 0;
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
    };

    struct SmartStringObject : public SmartObject {
        explicit SmartStringObject(std::string_view smartClass = "") : SmartObject(smartClass) {}

        [[nodiscard]] virtual SmartStringSubType stringSubType() const = 0;
    };

    template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
    struct SmartString : public SmartStringObject {
        using string_view_t = std::basic_string_view<TChar>;
        using string_t = std::basic_string<TChar>;

        explicit SmartString(std::string_view smartClass = "") : SmartStringObject(smartClass) {}

        [[nodiscard]] SmartType smartType() const override { return SmartType::String; }

        [[nodiscard]] SmartStringSubType stringSubType() const override;

        string_t value;

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
    };

    class SmartArrayObject : public SmartObject {
    public:
        explicit SmartArrayObject(std::string_view smartClass = "") : SmartObject(smartClass) {}
        [[nodiscard]] SmartType smartType() const override { return SmartType::Array; }
        [[nodiscard]] virtual SmartArraySubType arraySubType() const = 0;
        virtual std::shared_ptr<SmartObject> at(size_t index) = 0;
        virtual size_t size() const = 0;

        virtual void fill(int8_t * target, size_t target_size) const = 0;
        virtual void fill(int16_t * target, size_t target_size) const = 0;
        virtual void fill(int32_t * target, size_t target_size) const = 0;
        virtual void fill(int64_t * target, size_t target_size) const = 0;
        virtual void fill(uint8_t * target, size_t target_size) const = 0;
        virtual void fill(uint16_t * target, size_t target_size) const = 0;
        virtual void fill(uint32_t * target, size_t target_size) const = 0;
        virtual void fill(uint64_t * target, size_t target_size) const = 0;
        virtual void fill(float * target, size_t target_size) const = 0;
        virtual void fill(double * target, size_t target_size) const = 0;

    };

    template<typename TInt, typename = std::enable_if_t<is_int<TInt>>> class SmartIntArray;
    template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>> class SmartFloatArray;

    class SmartObjectArray : public SmartArrayObject {
    public:
        explicit SmartObjectArray(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}

        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Object; };
        std::vector<std::shared_ptr<SmartObject>> data;
        inline std::shared_ptr<SmartObject> operator[](size_t index) { return data[index]; }

        std::shared_ptr<SmartObject> at(size_t index) override {
            return (*this)[index];
        }

        size_t size() const override { return data.size(); }


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


        void fill(int8_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(int16_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(int32_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(int64_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(uint8_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(uint16_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(uint32_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(uint64_t *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(float *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }

        void fill(double *target, size_t target_size) const override {
            throw std::runtime_error("Не применимо.");
        }
    };


    template<typename TInt>
    class SmartIntArray<TInt> : public SmartArrayObject {
    public:
        explicit SmartIntArray(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}
        std::shared_ptr<SmartObject> at(size_t index) override {
            auto result = SmartObject::create<SmartInt<TInt>>();
            *result = (*this)[index];
            return result;
        }
        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Int; };
        std::vector<TInt> data;

        inline TInt& operator[](size_t index) { return data[index]; }
        inline const TInt& operator[](size_t index) const { return data[index]; }
        size_t size() const override { return data.size(); }

        void fill(int8_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int16_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int32_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int64_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint8_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint16_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint32_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint64_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(float *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(double *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }
    };

    template<typename TFloat>
    class SmartFloatArray<TFloat> : public SmartArrayObject {
    public:
        explicit SmartFloatArray(std::string_view smartClass = "") : SmartArrayObject(smartClass) {}
        std::shared_ptr<SmartObject> at(size_t index) override {
            auto result = SmartObject::create<SmartFloat<TFloat>>();
            *result = (*this)[index];
            return result;
        }
        [[nodiscard]] SmartArraySubType arraySubType() const override { return SmartArraySubType::Float; };
        std::vector<TFloat> data;

        inline TFloat& operator[](size_t index) { return data[index]; }
        inline const TFloat& operator[](size_t index) const { return data[index]; }
        size_t size() const override { return data.size(); }

        void fill(int8_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int16_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int32_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(int64_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint8_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint16_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint32_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(uint64_t *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(float *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }

        void fill(double *target, size_t target_size) const override {
            for(size_t i = 0; i < std::min(target_size, data.size()); ++i) target[i] = data.at(i);
        }
    };


    class SmartMapObject : public SmartObject {
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
            auto result = setIntArray<TInt>(smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TInt));
            return result;
        }

        template<typename TFloat, typename = std::enable_if_t<is_float<TFloat>>>
        std::shared_ptr<SmartFloatArray<TFloat>> setFloatArray(std::string_view name, TFloat* value, size_t size, std::string_view smartClass = "") {
            auto result = setFloatArray<TFloat>(smartClass);
            result->data.resize(size);
            memcpy(result->data.data(), value, size * sizeof(TFloat));
            return result;
        }

        template<typename TChar, typename = std::enable_if_t<is_char<TChar>>>
        std::shared_ptr<SmartString<TChar>> setString(std::string_view name, typename SmartString<TChar>::string_view_t text, std::string_view smartClass = "") {
            auto result = SmartObject::create<SmartString<TChar>>(smartClass);
            data.emplace(std::string(name), result);
            *result = std::string(text);
            return result;
        }

        std::shared_ptr<SmartObjectArray> setObjectArray(std::string_view name, std::string_view smartClass = "");
        std::shared_ptr<class SmartMapObject> setMap(std::string_view name, std::string_view smartClass = "");
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
