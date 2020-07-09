/*
 * trainer.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <trainer.hpp>

namespace l4ai::algs {

	template<>
	const float Trainer<float>::default_train_speed = 0.01f;

	template<>
	const double Trainer<double>::default_train_speed = 0.01;

	template class Trainer<float>;
	template class Trainer<double>;

} /* namespace l4ai::algs */
