/*
 * pipe.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include "pipe.h"

namespace l4ai::algs {

	Pipe::Pipe(size_t layer_count) : Algorithm(), type(PipeType::Abstract), layer_count(layer_count) {
		alg_type = AlgorithmType::Pipe;
	}

	Pipe::Pipe() : Pipe(size_t()) {}

	size_t Pipe::getLayerCount() const {
		return layer_count;
	}

	void Pipe::setLayerCount(size_t value) {
		layer_count = value;
	}

}
