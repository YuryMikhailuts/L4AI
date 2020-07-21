/*
 * perceptroncommon.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PERCEPTRONEXECUTORBASE_H_
#define L4AI_INC_PERCEPTRONEXECUTORBASE_H_
#include <stddef.h>
#include <inttypes.h>
#include <algorithm.h>
#include <flatperceptron.h>
#include <perceptroninstance.h>
#include <math.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	inline static constexpr TValue square(TValue value) { return value * value; }

	template<typename TValue>
	class PerceptronExecutorBase {
	public:
		using value_t = TValue;
		using activation_function_t = void (*)(const value_t*src, value_t*dst, size_t len);
		using activation_diff_type_t = enum class ActivationDiffType : size_t { ByArgument, ByValue, AlwaysZero, AlwaysOne };
	private:
		inline static activation_function_t getActivationFunction(ActivationFunctions af) {
			switch (af) {
				case ActivationFunctions::ArcTangent: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = atan(src[i]); };
				case ActivationFunctions::ReLU: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] =  (src[i] < 0) ? 0 : src[i]; };
				case ActivationFunctions::HyperbolicTangent: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = (exp(src[i]) - exp(-1)) / (exp(src[i]) + exp(-src[i])); };
				case ActivationFunctions::Logistic: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = 1 / (1 + exp(-src[i])); };
				case ActivationFunctions::Gauss: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = exp(-square(src[i])); };
				case ActivationFunctions::FakeGauss: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = 1 / (1 + square(src[i])); };
				case ActivationFunctions::Trivial: [[fallthrough]];
				default: return [](const value_t*src, value_t*dst, size_t len){ for(size_t i = 0; i < len; ++i) dst[i] = src[i]; };
			}
		}
		inline static activation_function_t getActivationDiff(ActivationFunctions af) {
			switch (af) {
				case ActivationFunctions::ArcTangent: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = 1/(src[i]*src[i] + 1); };
				case ActivationFunctions::ReLU: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] =  (src[i] < 0) ? 0 : 1; };
				case ActivationFunctions::HyperbolicTangent: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = 1 - src[i] * src[i]; };
				case ActivationFunctions::Logistic: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = src[i] * (1 - src[i]); };
				case ActivationFunctions::Gauss: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = -2 * src[i] * exp(-square(src[i])); };
				case ActivationFunctions::FakeGauss: return [](const value_t*src, value_t*dst, size_t len) { for(size_t i = 0; i < len; ++i) dst[i] = -2 * src[i] / square(1 + square(src[i])); };
				case ActivationFunctions::Trivial: [[fallthrough]];
				default: return [](const value_t*src, value_t*dst, size_t len){ for(size_t i = 0; i < len; ++i) dst[i] = src[i]; };
			}
		}
		inline static activation_diff_type_t getActivationDiffType(ActivationFunctions af) {
			switch (af) {
				case ActivationFunctions::ArcTangent: [[fallthrough]];
				case ActivationFunctions::ReLU: return activation_diff_type_t::ByArgument;
				case ActivationFunctions::HyperbolicTangent: [[fallthrough]];
				case ActivationFunctions::Gauss: [[fallthrough]];
				case ActivationFunctions::FakeGauss: [[fallthrough]];
				case ActivationFunctions::Logistic: return activation_diff_type_t::ByValue;
				case ActivationFunctions::Trivial: [[fallthrough]];
				default: return activation_diff_type_t::AlwaysOne;
			}
		}
	protected:
		const activation_function_t activation_func;
		const activation_function_t activation_diff;
		const activation_diff_type_t activation_diff_type;

		inline static constexpr void RowMulCol(size_t count, const value_t* row, const value_t* col, value_t& target) {
			for(size_t i = 0; i < count; ++i)
				target += row[i] * col[i];
		}
		inline static constexpr void ArraysMul(size_t count, const value_t* left, const value_t* right, value_t* target) {
			for(size_t i = 0; i < count; ++i)
				target[i] = left[i] * right[i];
		}
	public:
		PerceptronExecutorBase(ActivationFunctions af_type) :
			activation_func(getActivationFunction(af_type)),
			activation_diff(getActivationDiff(af_type)),
			activation_diff_type(getActivationDiffType(af_type)) {}
	};

}

#endif /* L4AI_INC_PERCEPTRONEXECUTORBASE_H_ */
