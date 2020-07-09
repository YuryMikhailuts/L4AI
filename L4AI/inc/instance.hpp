/*
 * instance.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "instance.h"

namespace l4ai::algs {
	using namespace std;

	template<typename TValue> Instance<TValue>::Instance ( unique_ptr<Algorithm>&& algorithm ) : algorithm ( move ( algorithm ) ) {}

	template<typename TValue> Instance<TValue>::Instance ( Algorithm*&& algorithm ) : algorithm ( move ( algorithm ) ) { algorithm = nullptr; }

	template<typename TValue>
	const Algorithm& Instance<TValue>::getAlgorithm() const { return *algorithm; }

}

#endif // INSTANCE_HPP
