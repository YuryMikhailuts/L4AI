/*
 * smarttests.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <sstream>
#include <smart_objects.h>
#include <smart_to_json.h>
#include <smart_from_json.h>
#include <mytests.h>


using namespace std;
using namespace std::filesystem;
using namespace my::tests;
using namespace l4ai;
using namespace l4ai::smart;


open_group(Тесты SmartObject)
open_group(Тесты сериализации)

newTest(Сериализация и Десериализация в Json) {
    auto expected = SmartObject::create<SmartMapObject>();
    expected->setInt("IntField", 123);
    expected->setInt("IntFieldC", 125, "IntClass");
    expected->setFloat("FloatField", 123.4);
    expected->setFloat("FloatFieldC", 123.456, "FloatClass");
    expected->setString<char>("StringField", "1234");
    expected->setString<char>("StringFieldC", "123489", "StringClass");
    int intA[] = {1, 2, 3, 4, 5};
    expected->setIntArray("IntArray", intA, sizeof(intA) / sizeof(intA[0]));
    int intAC[] = {1, 2, 3, 4, 5, 6};
    expected->setIntArray("IntArrayC", intAC, sizeof(intAC) / sizeof(intAC[0]), "IntArrayClass");
    double floatA[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
    expected->setFloatArray("FloatArray", floatA, sizeof(floatA) / sizeof(floatA[0]));
    double floatAC[] = {1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9};
    expected->setFloatArray("FloatArrayC", floatAC, sizeof(floatAC) / sizeof(floatAC[0]), "FloatArrayClass");
    stringstream ss;
    SmartToJson smartToJson(ss);
    smartToJson.save(expected);
    //inf << "expected: " << ss.str() << endl << endl;
    ss.seekp(0, ios_base::beg);
    ss.seekg(0, ios_base::beg);
    SmartFromJson smartFromJson(ss);
    smartFromJson.classTypes.emplace("IntClass"s, SmartTypeRequirement(SmartIntSubType::SInt32, false));
    smartFromJson.classTypes.emplace("FloatClass"s, SmartTypeRequirement(SmartFloatSubType::Float64, false));
    smartFromJson.classTypes.emplace("StringClass"s, SmartTypeRequirement(SmartStringSubType::String));
    smartFromJson.classTypes.emplace("IntArrayClass"s, SmartTypeRequirement(SmartIntSubType::SInt32, true));
    smartFromJson.classTypes.emplace("FloatArrayClass"s, SmartTypeRequirement(SmartFloatSubType::Float64, true));
    auto actual = smartFromJson.get();
    ss = stringstream{};
    smartToJson.save(actual);
    //inf << "actual: " << ss.str() << endl << endl;
    assertEqual(*actual, *expected);
}


close_groups()