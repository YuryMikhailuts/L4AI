/*
 * trainer.hpp
 *
 *  Created on: 18 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_INC_TRAINER_HPP_
#define L4AI_INC_TRAINER_HPP_
#include <trainer.h>
#include <algorithm.h>
#include <perceptron.h>
#include <perceptroncalculator.hpp>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	using error_function_t = typename Trainer<TValue>::error_function_t;

	template<typename TValue>
	error_function_t<TValue> getErrorFunction(ErrorFunctionType error_function_type) {
		using value_t = TValue;
		switch (error_function_type) {
			case ErrorFunctionType::StandardDeviationSquare:
				return [](size_t count, const value_t* origin, const value_t* calculate)->value_t{
					value_t result = value_t();
					for(size_t i = 0; i < count; ++i) {
						value_t diff = origin[i] - calculate[i];
						result = diff * diff;
					}
					result = result / count;
					return result;
				};
			case ErrorFunctionType::ArithmeticMeanAbsoluteDeviation:
				return [](size_t count, const value_t* origin, const value_t* calculate)->value_t{
					value_t result = value_t();
					for(size_t i = 0; i < count; ++i) {
						value_t diff = abs(origin[i] - calculate[i]);
						result = diff;
					}
					result = result / count;
					return result;
				};
			default:
				throw runtime_error("no implement this error function.");
		}
	}

	template<typename TValue>
	Trainer<TValue>::Trainer(instance_ptr_t&& instance) : Trainer(move(instance), ErrorFunctionType::StandardDeviationSquare) {}

	template<typename TValue>
	Trainer<TValue>::Trainer(instance_ptr_t&& instance, ErrorFunctionType error_function_type)
		: instance(move(instance)), error_function_type(error_function_type), error_function(getErrorFunction<TValue>(error_function_type)) {}


	template<typename TValue>
	Trainer<TValue>::~Trainer() {}


} /* namespace algs::l4ai */


#endif /* L4AI_INC_TRAINER_HPP_ */
