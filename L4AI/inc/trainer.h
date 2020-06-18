/*
 * trainer.h
 *
 *  Created on: 18 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_TRAINER_H_
#define L4AI_TRAINER_H_
#include <instance.h>
#include <memory.h>

namespace l4ai::algs {

		enum class ErrorFunctionType {
			/**
			 * @brief Квадрат средне-квадратичного отклонения. То есть сумма квадратов отклонений делённая на количество.
			 */
			StandardDeviationSquare,
			/**
			 * @brief Среднеарифметическое модулей отклонений. То есть сумма модулей отклонений делённая на количество.
			 */
			ArithmeticMeanAbsoluteDeviation
		};

		template<typename TValue>
		class Trainer {
		public:
			using value_t = TValue;
			template<class TClass> using ptr_t = std::shared_ptr<TClass>;
			using trainer_t = Trainer<value_t>;
			using trainer_ptr_t = ptr_t<trainer_t>;
			using instance_t = Instance<value_t>;
			using instance_ptr_t = ptr_t<instance_t>;
			using error_function_t = value_t (*)(size_t count, const value_t* origin, const value_t* calculate);
		protected:
			instance_ptr_t instance;
			const ErrorFunctionType error_function_type;
			const error_function_t error_function;
			/**
			 * @brief Последнее значение ошибки. То есть значение ошибки полученное при последнем запуске @link l4ai::algs::Trainer::toTrain @endlink.
			 */
			value_t last_error_value;
			/**
			 * @brief Скользящее среднее функции ошибки.
			 * Вычисляется в конце каждого успешного вызова @link l4ai::algs::Trainer::toTrain @endlink как среднее арифметическое последнего скользящего среднего с новой величины ошибки.
			 */
			value_t float_average_error;
			Trainer(instance_ptr_t&& instance, ErrorFunctionType error_function_type);
			Trainer(instance_ptr_t&& instance);
		public:
			virtual ~Trainer();
			virtual bool toTrain(value_t* in_data, value_t* out_data) const = 0;
			static trainer_ptr_t make(instance_ptr_t&& inst);
		};

} /* namespace l4ai::algs */

#endif /* L4AI_TRAINER_H_ */
