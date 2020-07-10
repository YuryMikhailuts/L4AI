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

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( std::unique_ptr<PipeLine>&& algorithm )
		: instance_t::Instance(move(algorithm)), layers(new instance_t*[algorithm->getLayerCount()]) {}

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( PipeLine*&& algorithm )
		: PipeLineInstance(move(std::unique_ptr<PipeLine>(algorithm))) {}


}

