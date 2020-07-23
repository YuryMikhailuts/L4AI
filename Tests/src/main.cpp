/*
 * main.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 21 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <iostream>
#include <testbase.h>

using namespace std;
using namespace my::tests;


newTest(aa) {
	inf << "aaaaa"s << endl;
}

int main(int argc, char* argv[]) {
	switch (argc) {
		case 1:
			return TestSet::current().run();
		case 2:
			return TestSet::current().run(argv[1]);
		case 3:
		default:
			return TestSet::current().run(argv[1], argv[2]);
	}
}
