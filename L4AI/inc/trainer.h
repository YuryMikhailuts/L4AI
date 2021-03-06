/*
 * trainer.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
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
			using error_diff_function_t = void (*)(size_t count, const value_t* origin, const value_t* calculate, value_t* target, bool multiply);
			using trainer_context_t = ITrainerContext<value_t>;
			static const value_t default_train_speed;
		protected:
			instance_ptr_t instance;
			const ErrorFunctionType error_function_type;
			const error_function_t error_function;
			const error_diff_function_t error_diff_function;
			/**
			 * @brief Последнее значение ошибки. То есть значение ошибки полученное при последнем запуске @link l4ai::algs::Trainer::train @endlink.
			 */
			mutable value_t last_error_value;
			/**
			 * @brief Скользящее среднее функции ошибки.
			 * Вычисляется в конце каждого успешного вызова @link l4ai::algs::Trainer::toTrain @endlink как среднее арифметическое последнего скользящего среднего и новой величины ошибки.
			 */
			mutable value_t float_average_error;

			bool use_last_error_value;

			bool use_float_average_error;

			value_t train_speed;
			Trainer(instance_ptr_t&& instance, std::optional<ErrorFunctionType> error_function_type = std::nullopt);
		public:

			inline value_t getTrainSpeed() const { return train_speed; }

			inline void setTrainSpeed(value_t value) { train_speed = value; }

			/**
			 * @brief Последнее значение ошибки. То есть значение ошибки полученное при последнем запуске @link l4ai::algs::Trainer::train @endlink.
			 */
			value_t getLastErrorValue() const { return last_error_value; }
			/**
			 * @brief Скользящее среднее функции ошибки.
			 * Вычисляется в конце каждого успешного вызова @link l4ai::algs::Trainer::toTrain @endlink как среднее арифметическое последнего скользящего среднего и новой величины ошибки.
			 */
			value_t getFloatAverageError() const { return float_average_error; }

			inline bool getUseLastErrorValue() const { return use_last_error_value; }

			inline bool getUseFloatAverageError() const { return use_float_average_error; }

			inline void setUseLastErrorValue(bool value) { use_last_error_value = value; }

			inline void setUseFloatAverageError(bool value) { use_float_average_error = value; }

			inline void onUseLastErrorValue() { setUseLastErrorValue(true); }

			inline void onUseFloatAverageError() { setUseFloatAverageError(true); }

			inline void offUseLastErrorValue() { setUseLastErrorValue(false); }

			inline void offUseFloatAverageError() { setUseFloatAverageError(false); }

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
			virtual bool train(trainer_context_t& context, value_t* in_data, value_t* out_data, value_t* etalon = nullptr) const = 0;

			/**
			 * @brief Корректирует параметры алгоритма, учитывая производную функции ошибки по выходным данным (@codeline err_diff_out @endcode) и ранее вычисленные значения внутренних производных, передаваемые через контекст (@codeline context @endcode).
			 * Также данный метод может вычислить значение производную функции ошибки по входным данным, если @codeline err_diff_in != nullptr @endcode.
			 *
			 * @param err_diff_out 	Указатель на массив, хранящий производную функции ошибки по выходным данным.
			 * 						Должен быть передан обязательно. Может быть вычислен функцией ошибки.
			 * @param err_diff_in	Указатель на массив, в который будет сохранена производная функции ошибки по входным данным.
			 * 						Если данный аргумент равен @codeline nullptr @endcode, то вычисление производной функции ошибки по входным данным не выполняется.
			 */
			virtual void fix(trainer_context_t& context, /*in*/ value_t* err_diff_out = nullptr, /*out*/ value_t* err_diff_in = nullptr) = 0;
			static trainer_ptr_t make(instance_ptr_t inst, std::optional<ErrorFunctionType> error_function_type = std::nullopt);
		};

		using TrainerF32 = Trainer<float>;
		using TrainerF64 = Trainer<double>;

} /* namespace l4ai::algs */

#endif /* L4AI_TRAINER_H_ */
