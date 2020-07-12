/*
 * calculator.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_CALCULATOR_HPP_
#define L4AI_INC_CALCULATOR_HPP_
#include <algorithm.h>
#include <calculator.h>
#include <perceptron.h>
#include <perceptroncalculator.hpp>
#include <pipelinecalculator.hpp>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	Calculator<TValue>::Calculator(instance_ptr_t&& instance)
		: instance(move(instance)) {}

	template<typename TValue>
	Calculator<TValue>::~Calculator() {}

	template<typename TValue>
	typename Calculator<TValue>::calculator_ptr_t Calculator<TValue>::make(instance_ptr_t&& inst) {
		instance_t& instance = *inst;
		auto& alg = instance.getAlgorithm();
		switch (alg.type()) {
			case AlgorithmType::Perceptron: {
				const Perceptron& perc = static_cast<const Perceptron&>(alg);
				switch (perc.getPerceptronType()) {
					case PerceptronType::Flat:
						return calculator_ptr_t(new FlatPerceptronCalculator<TValue>(move(inst)));
					default:
						return calculator_ptr_t(nullptr);
				}
			}
			case AlgorithmType::Pipe:
				const Pipe& pipe = static_cast<const Pipe&>(alg);
				switch (pipe.getPipeType()) {
					case PipeType::Line:
						return calculator_ptr_t(new PipeLineCalculator<TValue>(move(inst)));
						break;
					default:
						return calculator_ptr_t(nullptr);
						break;
				}
			default:
				return calculator_ptr_t(nullptr);
				break;
		}

	}

} /* namespace algs::l4ai */


#endif /* L4AI_INC_CALCULATOR_HPP_ */
