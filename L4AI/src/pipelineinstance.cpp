/*
 * pipelineinstance.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 10 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <pipelineinstance.hpp>
#include <L4AI_global.h>

namespace l4ai::algs {

	template class L4AI_EXPORT PipeLineInstance<float>;
	template class L4AI_EXPORT PipeLineInstance<double>;

}	// l4ai::algs



