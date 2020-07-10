/*
 * pipelineinstance.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 10 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PIPELINEINSTANCE_H_
#define L4AI_INC_PIPELINEINSTANCE_H_
#include <type_traits>
#include <instance.h>
#include <pipeline.h>

namespace l4ai::algs {

	template<typename TValue>
	class PipeLineInstance : public Instance<TValue> {
	public:
		using instance_t = Instance<TValue>;
		using value_t = typename instance_t::value_t;
	private:
		instance_t** layers;
		static instance_t** makeLayers(PipeLine& algorithm);
	public:
		PipeLineInstance ( std::unique_ptr<PipeLine>&& algorithm );
		PipeLineInstance ( PipeLine*&& algorithm );
		size_t getLayersCount() const;
		instance_t& getLayer(size_t index);
		const instance_t& getLayer(size_t index) const;
	};

}

#endif /* L4AI_INC_PIPELINEINSTANCE_H_ */
