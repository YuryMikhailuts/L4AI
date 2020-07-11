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
	protected:
		const PipeLineInstance<TValue>& getPerceptronInstance() const {
			const Instance<TValue>& inst = *calculator_t::instance;
			return static_cast<const PipeLineInstance<TValue>&>(inst);
		}

		PipeLineCalculator(instance_ptr_t&& instance) :
			calculator_t::Calculator(move(instance)) {}
	};



}

#endif /* L4AI_INC_PIPELINECALCULATOR_HPP_ */
