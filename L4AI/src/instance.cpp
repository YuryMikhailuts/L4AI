/*
 * instance.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <L4AI_global.h>
#include "instance.hpp"


namespace l4ai::algs {
	using namespace std;

	template class L4AI_EXPORT Instance<float>;
	template class L4AI_EXPORT Instance<double>;
}
