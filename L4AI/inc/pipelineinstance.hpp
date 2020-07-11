/*
 * pipelineinstance.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 10 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <pipelineinstance.h>
#include <pipe.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( std::unique_ptr<PipeLine>&& algorithm )
		: instance_t::Instance(move(algorithm)), layers(makeLayers(*algorithm)) {}

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( PipeLine*&& algorithm )
		: PipeLineInstance(move(std::unique_ptr<PipeLine>(algorithm))) {}

	template<typename TValue>
	Instance<TValue>** PipeLineInstance<TValue>::makeLayers(PipeLine& algorithm) {
		size_t count = algorithm.getLayersCount();
		instance_t** layers = new instance_t*[count];
		for(size_t i = 0; i < count; ++i) {
			layers[i] = instance_t::make(&algorithm.getLayer(i)).release();
		}
		return layers;
	}

	template<typename TValue>
	size_t PipeLineInstance<TValue>::getLayersCount() const {
		return static_cast<const PipeLine&>(Instance<TValue>::getAlgorithm()).getLayersCount();
	}

	template<typename TValue>
	Instance<TValue>& PipeLineInstance<TValue>::getLayer(size_t index) {
		return *layers[index];
	}

	template<typename TValue>
	const Instance<TValue>& PipeLineInstance<TValue>::getLayer(size_t index) const {
		return *layers[index];
	}

}

