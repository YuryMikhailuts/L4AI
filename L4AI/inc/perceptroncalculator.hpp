/*
 * perceptroncalculator.hpp
 *
 *  Created on: 14 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_INC_PERCEPTRONCALCULATOR_HPP_
#define L4AI_INC_PERCEPTRONCALCULATOR_HPP_
#include <algorithm.h>
#include <Calculator.h>
#include <flatperceptron.h>
#include <perceptroninstance.h>
#include <math.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	class PerceptronCalculator : public Calculator<TValue> {
	public:
		using calculator_t = Calculator<TValue>;
		using value_t = typename calculator_t::value_t;
		using instance_ptr_t = typename calculator_t::instance_ptr_t;
		using activation_function_t = value_t (*)(const value_t& );
	private:
		inline static activation_function_t getActivationFunction(ActivationFunctions af) {
			switch (af) {
				case ActivationFunctions::ArcTangent: return [](const value_t& x_value) -> value_t { return atan(x_value); };
				case ActivationFunctions::Heaviside: return [](const value_t& x_value) -> value_t { return (x_value < 0) ? 0 : 1; };
				case ActivationFunctions::HyperbolicTangent: return [](const value_t& x_value) -> value_t { return (exp(x_value) - exp(-1)) / (exp(x_value) + exp(-x_value)); };
				case ActivationFunctions::Logistic: return [](const value_t& x_value) -> value_t { return 1 / (1 + exp(-x_value)); };
				case ActivationFunctions::Trivial: return [](const value_t& x_value) -> value_t { return x_value; };
				default: return [](const value_t& x_value){ return x_value; };
			}
		}
	protected:
		const activation_function_t activation_function;
		const PerceptronInstance<TValue>& getPerceptronInstance() const {
			const Instance<TValue>& inst = *calculator_t::instance;
			return static_cast<const PerceptronInstance<TValue>&>(inst);
		}

		void RowMulCol(size_t count, const value_t* row, const value_t* col, value_t& target) const {
			for(size_t i = 0; i < count; ++i)
				target += row[i] * col[i];
		}

		PerceptronCalculator(instance_ptr_t&& instance) : calculator_t::Calculator(move(instance)), activation_function(getActivationFunction(getPerceptronInstance().getPerceptron().getFunction())) {}
	};

	template<typename TValue>
	class FlatPerceptronCalculator : public PerceptronCalculator<TValue> {
	public:
		using perceptron_calculator_t = PerceptronCalculator<TValue>;
		using calculator_t = typename perceptron_calculator_t::calculator_t;
		using value_t = typename perceptron_calculator_t::value_t;
		using instance_ptr_t = typename calculator_t::instance_ptr_t;
	private:
		const FlatPerceptron& getFlatPerceptron() const {
			const Algorithm& alg = calculator_t::instance->getAlgorithm();
			return alg.as<FlatPerceptron>();
		}
	protected:
		virtual bool calculate(value_t* in_data, value_t* out_data) const override final {
			bool result = false;
			const FlatPerceptron& alg = getFlatPerceptron();
			auto& inst = perceptron_calculator_t::getPerceptronInstance();
			bool use_shift = alg.getUseShiftInput();
			size_t in_length = alg.getInputLength();
			size_t out_length = alg.getOutputLength();
			size_t rows_count = alg.getWeightsRows();
			size_t rows_offset = 0;
			size_t avl_rows_count = rows_count - (use_shift ? 1 : 0);
			for(size_t i = 0; i < out_length; ++i) {
				/**
				 * Умножение производится по частям по следующей схеме:
				 *     1. Первая часть - это кусок входного вектора от индекса сдвига до конца вектора.
				 *        Его длинна может быть равна нулю только если индекс сдвига равен длинне вектора.
				 *     2. Вторая часть - это хвост, который не попал во вторую часть.
				 *        Его длинна может быть не равна нулю только когда в середину входного вектора попадает конец набора данных и он оказывается разделён на части.
				 *
				 *  * Вариант, когда есть только первая часть, а длинна второй равна 0.
				 *  Входной вектора выбирается из входного набора данных.
				 *  Входной набор данных: | 0 | 1 | 2 | 3 | 5 | 6 | ...
				 *        Входной вектор:     | 0 | 1 | 2 | 3 |
				 *
				 *  * Вариант, когда есть обе части.
				 *  Входной вектора выбирается из входного набора данных.
				 *  Входной набор данных: | 0 | 1 | 2 | 3 | 5 | ... | N-3 | N-2 | N-1 |
				 *        Входной вектор: | 2 | 3 |                       |  0  |  1  |
				 *
				 */
				// Подготавливаем указатель на начало колонки.
				const value_t* weight_col = inst.getWeightColumn(i);
				//  Вычисляем длины для обоих частей произведения.
				size_t part1_length = min(avl_rows_count, in_length - rows_offset);
				size_t part2_length = avl_rows_count - part1_length;
				// Умножаем каждую часть только, если её длина не равна нулю.
				out_data[i] = 0;
				if (part1_length > 0) perceptron_calculator_t::RowMulCol(part1_length, &in_data[rows_offset], &weight_col[0], out_data[i]);
				if (part2_length > 0) perceptron_calculator_t::RowMulCol(part2_length, &in_data[rows_offset], &weight_col[part1_length], out_data[i]);
				if (use_shift) out_data[i] += inst.getWeight(rows_count - 1, i);
				out_data[i] = perceptron_calculator_t::activation_function(out_data[i]);
				// Пересчитываем смещение индекса входного вектора.
				rows_offset += alg.getRowsOffset();
				rows_offset = rows_offset % in_length;
			}
			return result;
		}
	public:
		FlatPerceptronCalculator(instance_ptr_t&& instance) : perceptron_calculator_t::PerceptronCalculator(move(instance)) {}
	};


} /* namespace algs::l4ai */


#endif /* L4AI_INC_PERCEPTRONCALCULATOR_HPP_ */
