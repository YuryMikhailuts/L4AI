/*
 * perceptroncalculator.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PERCEPTRONCALCULATOR_HPP_
#define L4AI_INC_PERCEPTRONCALCULATOR_HPP_
#include <perceptronexecutorbase.h>
#include <calculator.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	class PerceptronCalculator : public Calculator<TValue>, public PerceptronExecutorBase<TValue> {
	public:
		using calculator_t = Calculator<TValue>;
		using perceptron_executor_base_t = PerceptronExecutorBase<TValue>;
		using value_t = typename calculator_t::value_t;
		using instance_ptr_t = typename calculator_t::instance_ptr_t;
		using activation_function_t = value_t (*)(const value_t& );
	protected:
		const PerceptronInstance<TValue>& getPerceptronInstance() const {
			const Instance<TValue>& inst = *calculator_t::instance;
			return static_cast<const PerceptronInstance<TValue>&>(inst);
		}

		PerceptronCalculator(instance_ptr_t&& instance) :
			calculator_t::Calculator(move(instance)),
			perceptron_executor_base_t::PerceptronExecutorBase(getPerceptronInstance().getPerceptron().getFunction()) {}
	};

	template<typename TValue>
	class FlatPerceptronCalculator : public PerceptronCalculator<TValue> {
	public:
		using perceptron_calculator_t = PerceptronCalculator<TValue>;
		using perceptron_executor_base_t = typename perceptron_calculator_t::perceptron_executor_base_t;
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
			try	{
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
					if (part1_length > 0) perceptron_executor_base_t::RowMulCol(part1_length, &in_data[rows_offset], &weight_col[0], out_data[i]);
					if (part2_length > 0) perceptron_executor_base_t::RowMulCol(part2_length, &in_data[rows_offset], &weight_col[part1_length], out_data[i]);
					if (use_shift) out_data[i] += inst.getWeight(rows_count - 1, i);
					// Пересчитываем смещение индекса входного вектора.
					rows_offset += alg.getRowsOffset();
					rows_offset = rows_offset % in_length;
				}
				perceptron_executor_base_t::activation_func(out_data, out_data, out_length);
				result = true;
			} catch (runtime_error& re) {
				result = false;
			} catch (runtime_error* pe) {
				result = false;
			} catch (logic_error& re) {
				result = false;
			} catch (logic_error* pe) {
				result = false;
			}
			return result;
		}
	public:
		FlatPerceptronCalculator(instance_ptr_t&& instance) : perceptron_calculator_t::PerceptronCalculator(move(instance)) {}
	};


} /* namespace algs::l4ai */


#endif /* L4AI_INC_PERCEPTRONCALCULATOR_HPP_ */
