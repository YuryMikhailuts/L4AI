/*
 * calculator.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
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
		template<class TClass> using ptr_t = std::shared_ptr<TClass>;
		using calculator_t = Calculator<value_t>;
		using calculator_ptr_t = ptr_t<calculator_t>;
		using instance_t = Instance<value_t>;
		using instance_ptr_t = ptr_t<instance_t>;
	protected:
		instance_ptr_t instance;
		Calculator(instance_ptr_t&& instance);
	public:
		virtual ~Calculator();
		/**
		 * @brief Запускает алгоритм @link l4ai::algs::Calculator::intance @endlink на входных данных, переданых через аргумент @codeline in_data @endcode, помещая результат вычислений в массив, переданый через аргумент @codeline out_data @endcode.
		 * @return Возвращает @codeline true @endcode если вычисления прошли успешно и @codeline false @endcode в случае ошибки.
		 * @remark Значение @codeline false @endcode возвращается только в случае, если произошли логические ошибки, такие как деление на 0 (если это запрещено для выбранного типа данных). В случае же критических ошибок, таких как разыменование нуля или переполнение стека, даный метод будет бросать исключения, как и все нормальные люди (т.е. методы).
		 */
		virtual bool calculate(value_t* in_data, value_t* out_data) const = 0;
		static calculator_ptr_t make(instance_ptr_t&& inst);
	};

	using CalculatorF32 = Calculator<float>;
	using CalculatorF64 = Calculator<double>;
} /* namespace algs::l4ai */

#endif /* L4AI_INC_CALCULATOR_H_ */
