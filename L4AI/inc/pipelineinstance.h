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
		template<typename T>
		using ptr_t = std::shared_ptr<T>;
		template<typename T>
		using array_ptr_t = std::shared_ptr<T[]>;
		using instance_ptr_t = ptr_t<instance_t>;
	private:
		array_ptr_t<instance_ptr_t> layers;
		static array_ptr_t<instance_ptr_t> makeLayers(PipeLine& algorithm);
		const PipeLine& getPipeLine() const;
		PipeLine& getPipeLine();
	public:
		PipeLineInstance ( std::unique_ptr<PipeLine>&& algorithm );
		PipeLineInstance ( PipeLine*&& algorithm );
		size_t getLayersCount() const;
		instance_t& getLayer(size_t index);
		const instance_t& getLayer(size_t index) const;
		instance_ptr_t getLayerPtr(size_t index);

        void setConfiguration(std::shared_ptr<smart::SmartObject> configuration) override;

        [[nodiscard]] std::shared_ptr<smart::SmartObject> getConfiguration() const override;
    };

	using PipeLineInstanceF32 = PipeLineInstance<float>;
	using PipeLineInstanceF64 = PipeLineInstance<double>;

}

#endif /* L4AI_INC_PIPELINEINSTANCE_H_ */
