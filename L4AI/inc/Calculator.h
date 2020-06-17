/*
 * calculator.h
 *
 *  Created on: 14 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_INC_CALCULATOR_H_
#define L4AI_INC_CALCULATOR_H_
#include <instance.h>
#include <memory.h>

namespace l4ai::algs {

	template<typename TValue>
	class Calculator {
	public:
		using value_t = TValue;
		template<class TClass> using ptr_t = std::unique_ptr<TClass>;
		using calculator_t = Calculator<value_t>;
		using calculator_ptr_t = ptr_t<calculator_t>;
		using instance_t = Instance<value_t>;
		using instance_ptr_t = ptr_t<instance_t>;
	protected:
		instance_ptr_t instance;
		Calculator(instance_ptr_t&& instance);
	public:
		virtual ~Calculator();
		virtual bool calculate(value_t* in_data, value_t* out_data) const = 0;
		static calculator_ptr_t make(instance_ptr_t&& inst);
	};

	using CalculatorF32 = Calculator<float>;
	using CalculatorF64 = Calculator<double>;
} /* namespace algs::l4ai */

#endif /* L4AI_INC_CALCULATOR_H_ */
