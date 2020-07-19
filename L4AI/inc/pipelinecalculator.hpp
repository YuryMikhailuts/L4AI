/*
 * pipelinecalculator.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 11 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PIPELINECALCULATOR_HPP_
#define L4AI_INC_PIPELINECALCULATOR_HPP_
#include <calculator.h>
#include <pipelineinstance.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	class PipeLineCalculator : public Calculator<TValue> {
	public:
		using calculator_t = Calculator<TValue>;
		using value_t = typename calculator_t::value_t;
		using instance_ptr_t = typename calculator_t::instance_ptr_t;
		using activation_function_t = value_t (*)(const value_t& );
		template<typename T>
		using ptr_t = shared_ptr<T>;
		template<typename T>
		using array_ptr_t = ptr_t<T[]>;
		using calculator_ptr_t = ptr_t<calculator_t>;
	protected:
		size_t max_layer_length;
		value_t* internal_data[2];
		array_ptr_t<calculator_ptr_t> internal_calculators;

		static size_t findMaxLength(const PipeLineInstance<TValue>& instance) {
			size_t max_length = 0;
			size_t int_count = instance.getLayersCount();
			if (int_count > 0) {
				for(size_t i = 0; i < int_count; ++i) {
					max_length = max(max_length, instance.getLayer(i).getAlgorithm().getOutputLength());
				}
			}
			return max_length;
		}

		static array_ptr_t<calculator_ptr_t> makeInternalCalculators(PipeLineInstance<TValue>& instance) {
			size_t internal_calculator_count = instance.getLayersCount();
			if (internal_calculator_count > 0) {
				array_ptr_t<calculator_ptr_t> internal_calculators (new calculator_ptr_t[internal_calculator_count]);
				for(size_t index = 0; index < internal_calculator_count; ++index) {
					internal_calculators[index] = move(calculator_t::make(instance.getLayerPtr(index)));
				}
				return internal_calculators;
			} else {
				return nullptr;
			}
		}

		const PipeLineInstance<TValue>& getPipeLineInstance() const {
			const Instance<TValue>& inst = *calculator_t::instance;
			return static_cast<const PipeLineInstance<TValue>&>(inst);
		}

		PipeLineInstance<TValue>& getPipeLineInstance() {
			Instance<TValue>& inst = *calculator_t::instance;
			return static_cast<PipeLineInstance<TValue>&>(inst);
		}

		virtual bool calculate(value_t* in_data, value_t* out_data) const override {
			const PipeLineInstance<TValue>& instance =  getPipeLineInstance();
			size_t layers_count = instance.getLayersCount();
			bool success = true;
			value_t* idp = in_data;
			value_t* odp = internal_data[0];
			for(size_t i = 0; success && i < layers_count - 1; ++i) {
				success &= internal_calculators[i]->calculate(idp, odp);
				idp = internal_data[i & 1];
				odp = internal_data[(i + 1) & 1 ];
				size_t idp_len = instance.getLayer(i).getAlgorithm().getOutputLength();
				if (max_layer_length > idp_len)
					memset(&idp[idp_len], 0, max_layer_length - idp_len);
			}
			success &= internal_calculators[layers_count - 1]->calculate(idp, out_data);
			return success;
		}

	public:
		PipeLineCalculator(instance_ptr_t&& instance) :
			calculator_t::Calculator(move(instance)),
			max_layer_length(findMaxLength(getPipeLineInstance())),
			internal_data{ new value_t[max_layer_length], new value_t[max_layer_length]},
			internal_calculators(makeInternalCalculators(getPipeLineInstance())) {}
	};



}

#endif /* L4AI_INC_PIPELINECALCULATOR_HPP_ */
