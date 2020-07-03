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
#include <optional>

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
		struct ITrainerContext {
			const AlgorithmType target;
			explicit ITrainerContext(AlgorithmType target);
			virtual ~ITrainerContext();
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
			using trainer_context_t = ITrainerContext<value_t>;
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
			 * Вычисляется в конце каждого успешного вызова @link l4ai::algs::Trainer::toTrain @endlink как среднее арифметическое последнего скользящего среднего и новой величины ошибки.
			 */
			value_t float_average_error;
			Trainer(instance_ptr_t&& instance, std::optional<ErrorFunctionType> error_function_type = std::nullopt);
		public:

			virtual ~Trainer();

			/**
			 * @brief Создаёт работающий контекст, который идеально совместим именно с тем алгоритмом, с которым, в настоящий момент работает Тренер.
			 * @remark Возвращается уникальный указатель на контекст, посколько это временный объект и он должен существовать лишь в родительском методе обучения и, соответственно, ссылки на него можно распространять без использования умных указателей с подсчётом количества ссылок.
			 */
			virtual std::unique_ptr<trainer_context_t> makeContext() const = 0;

			/**
			 * @brief Выполняет первую часть обучения, а именно:
			 *  + Вычисление выходных значений.
			 *  + Вычисление внутренних производных.
			 *
			 * @param context		Указатель на контекст, в который будут сохранены внутренние производные.
			 * 						В последствии этот же контекст будет передан в метод @link l4ai::algs::Trainer::fix @endlink.
			 * @param in_data 		Должен указывать на входной массив данных для обучения.
			 * 						Длинна массива должна соответствовать размеру входных данных в спецификации алгоритма.
			 * @param out_data 		Должен указывать на массив в который будут помещены результаты вычислений, необходимые для вычисления функции ошибки и, возможно, для следующего алгоритма в цепочке, если используется цепочка алгоритмов.
			 * 						Длинна массива должна соответствовать размеру выходных данных в спецификации алгоритма.
			 */
			virtual bool train(trainer_context_t& context, value_t* in_data, value_t* out_data) const = 0;

			/**
			 * @brief Корректирует параметры алгоритма, учитывая производную функции ошибки по выходным данным (@codeline err_diff_out @endcode) и ранее вычисленные значения внутренних производных, передаваемые через контекст (@codeline context @endcode).
			 * Также данный метод может вычислить значение производную функции ошибки по входным данным, если @codeline err_diff_in != nullptr @endcode.
			 *
			 * @param err_diff_out 	Указатель на массив, хранящий производную функции ошибки по выходным данным.
			 * 						Должен быть передан обязательно. Может быть вычислен функцией ошибки.
			 * @param err_diff_in	Указатель на массив, в который будет сохранена производная функции ошибки по входным данным.
			 * 						Если данный аргумент равен @codeline nullptr @endcode, то вычисление производной функции ошибки по входным данным не выполняется.
			 */
			virtual void fix(trainer_context_t& context, value_t* err_diff_out, value_t* err_diff_in = nullptr) = 0;
			static trainer_ptr_t make(instance_ptr_t&& inst, std::optional<ErrorFunctionType> error_function_type = std::nullopt);
		};

} /* namespace l4ai::algs */

#endif /* L4AI_TRAINER_H_ */
