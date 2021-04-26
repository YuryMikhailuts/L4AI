/*
 * trainer.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <L4AI_global.h>
#include <trainer.hpp>

namespace l4ai::algs {

	template<>
	const float Trainer<float>::default_train_speed = 0.01f;

	template<>
	const double Trainer<double>::default_train_speed = 0.01;

	template class L4AI_EXPORT Trainer<float>;
	template class L4AI_EXPORT Trainer<double>;

} /* namespace l4ai::algs */
