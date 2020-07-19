/*
 * pipeline.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <pipeline.h>
#include <string.h>

namespace l4ai::algs {

	PipeLine::PipeLine(size_t layer_count, Algorithm**const layers)
		: Pipe(layer_count),
		  layers(new Algorithm*[layer_count]) {
		if (layer_count > 0 && layers != nullptr) {
			memcpy(this->layers, layers, layer_count * sizeof(Algorithm*));
		}
		Pipe::type = PipeType::Line;
	}

	PipeLine::PipeLine() : PipeLine(size_t(), nullptr) {}

	bool PipeLine::isEmptyLayer(size_t index) {
		return layers[index] == nullptr;
	}

	Algorithm& PipeLine::getLayer(size_t index) {
		return *layers[index];
	}

	const Algorithm& PipeLine::getLayer(size_t index) const {
		return *layers[index];
	}

	void PipeLine::setLayer(size_t index, Algorithm* algorithm) {
		layers[index] = algorithm;
	}


}	// l4ai::algs
