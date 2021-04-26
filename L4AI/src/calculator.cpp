/*
 * calculator.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <L4AI_global.h>
#include <calculator.hpp>

namespace l4ai::algs {

	template class L4AI_EXPORT Calculator<float>;
	template class L4AI_EXPORT Calculator<double>;

} /* namespace algs::l4ai */
