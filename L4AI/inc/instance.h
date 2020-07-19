/*
 * instance.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef INSTANCE_H
#define INSTANCE_H
#include <algorithm.h>
#include <memory>

namespace l4ai::algs {

	template<typename TValue>
	class Instance {
	public:
		using value_t = TValue;
		using instance_t = Instance<TValue>;
	private:
		std::unique_ptr<Algorithm> algorithm;
	protected:
		Instance ( std::unique_ptr<Algorithm>&& _algorithm );
		Instance ( Algorithm* _algorithm );
	public:
		virtual ~Instance();
		const Algorithm& getAlgorithm() const;
		Algorithm& getAlgorithm();
		static std::unique_ptr<instance_t> make(std::unique_ptr<Algorithm>&& algorithm);
		static std::unique_ptr<instance_t> make(Algorithm*&& algorithm);
	};

	using InstanceF32 = Instance<float>;
	using InstanceF64 = Instance<double>;
}

#endif // INSTANCE_H
