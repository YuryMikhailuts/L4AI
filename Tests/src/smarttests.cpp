/*
 * smarttests.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 23 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <mytests.h>
#include <sstream>
#include <smart_objects.h>
#include <smart_to_json.h>

using namespace std;
using namespace std::filesystem;
using namespace my::tests;
using namespace l4ai;
using namespace l4ai::smart;


open_group(Тесты SmartObject)
open_group(Тесты сериализации)

static constexpr char etalon1[] = R"=({
	"FloatArray": [ 1, 2, 3, 4, 5, 6, 7],
	"IntArrayC": {
		"@class": "IntCLass",
		"@value": [ 1, 2, 3, 4, 5, 6]
	},
	"IntArray": [ 1, 2, 3, 4, 5],
	"StringFieldC": "123489",
	"StringField": "1234",
	"FloatFieldC": {
		"@class": "FloatClass",
		"@value": 123.456
	},
	"FloatArrayC": {
		"@class": "FloatCLass",
		"@value": [ 1, 2, 3, 4, 5, 6, 7, 8]
	},
	"FloatField": 123.4,
	"IntFieldC": {
		"@class": "IntClass",
		"@value": 125
	},
	"IntField": 123
}
)=";


newTest(Сериализация в Json) {
    auto mapPtr = SmartObject::create<SmartMapObject>();
    mapPtr->setInt("IntField", 123);
    mapPtr->setInt("IntFieldC", 125, "IntClass");
    mapPtr->setFloat("FloatField", 123.4);
    mapPtr->setFloat("FloatFieldC", 123.456, "FloatClass");
    mapPtr->setString<char>("StringField", "1234");
    mapPtr->setString<char>("StringFieldC", "123489", "StringClass");
    int intA[] = {1, 2, 3, 4, 5};
    mapPtr->setIntArray("IntArray", intA, sizeof(intA) / sizeof(intA[0]));
    int intAC[] = {1, 2, 3, 4, 5, 6};
    mapPtr->setIntArray("IntArrayC", intAC, sizeof(intAC) / sizeof(intAC[0]), "IntCLass");
    double floatA[] = {1, 2, 3, 4, 5, 6, 7};
    mapPtr->setFloatArray("FloatArray", floatA, sizeof(floatA) / sizeof(floatA[0]));
    double floatAC[] = {1, 2, 3, 4, 5, 6, 7, 8};
    mapPtr->setFloatArray("FloatArrayC", floatAC, sizeof(floatAC) / sizeof(floatAC[0]), "FloatCLass");
    stringstream ss;
    SmartToJson smartToJson(ss);
    smartToJson.save(mapPtr);
    string actual = ss.str();
    assertEqual(actual, string(etalon1));
}


close_groups()