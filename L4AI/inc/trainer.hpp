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
#include <perceptrontrainer.hpp>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	using error_function_t = typename Trainer<TValue>::error_function_t;

	template<typename TValue>
	ITrainerContext<TValue>::ITrainerContext(AlgorithmType target) : target(target) {}

	template<typename TValue>
	ITrainerContext<TValue>::~ITrainerContext() {}


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
	Trainer<TValue>::Trainer(instance_ptr_t&& instance, std::optional<ErrorFunctionType> error_function_type)
		: instance(move(instance)),
		  error_function_type(error_function_type ? *error_function_type : ErrorFunctionType::StandardDeviationSquare),
		  error_function(getErrorFunction<TValue>(this->error_function_type)) {}


	template<typename TValue>
	Trainer<TValue>::~Trainer() {}

	template<typename TValue>
	typename Trainer<TValue>::trainer_ptr_t Trainer<TValue>::make(instance_ptr_t&& inst, std::optional<ErrorFunctionType> error_function_type) {
		instance_t& instance = *inst;
		auto& alg = instance.getAlgorithm();
		switch (alg.type()) {
			case AlgorithmType::Perceptron: {
				const Perceptron& perc = static_cast<const Perceptron&>(alg);
				switch (perc.getPerceptronType()) {
					case PerceptronType::Flat:
						return trainer_ptr_t(new FlatPerceptronTrainer<TValue>(move(inst)));
					default:
						return trainer_ptr_t(nullptr);
				}
			}
			case AlgorithmType::Pipe:
				return trainer_ptr_t(nullptr);
				break;
			default:
				return trainer_ptr_t(nullptr);
				break;
		}
	}

} /* namespace algs::l4ai */


#endif /* L4AI_INC_TRAINER_HPP_ */
