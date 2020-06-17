/*
 * calculator.hpp
 *
 *  Created on: 14 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_INC_CALCULATOR_HPP_
#define L4AI_INC_CALCULATOR_HPP_
#include <algorithm.h>
#include <perceptron.h>
#include <Calculator.h>
#include <perceptroncalculator.hpp>

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
				return calculator_ptr_t(nullptr);
				break;
			default:
				return calculator_ptr_t(nullptr);
				break;
		}

	}

} /* namespace algs::l4ai */


#endif /* L4AI_INC_CALCULATOR_HPP_ */
