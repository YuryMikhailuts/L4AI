/*
 * perceptrontrainer.hpp
 *
 *  Created on: 20 июн. 2020 г.
 *      Author: unknown
 */

#ifndef L4AI_INC_PERCEPTRONTRAINER_HPP_
#define L4AI_INC_PERCEPTRONTRAINER_HPP_
#include <perceptronexecutorbase.h>
#include <trainer.h>


namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	class PerceptronContext : public ITrainerContext<TValue> {
	public:
		using trainer_context_t = ITrainerContext<TValue>;
		using value_t = TValue;
		static constexpr std::unique_ptr<PerceptronContext<TValue>> make(const Perceptron& algorithm) {
			return std::unique_ptr<PerceptronContext<TValue>>(new PerceptronContext<TValue>(algorithm.getOutputLength(), algorithm.getInputLength()));
		}
		size_t diff_length;
		value_t* out_diff_summ;
		size_t in_length;
		value_t* in_data;

		PerceptronContext(size_t diff_len, size_t in_len)
			: trainer_context_t::ITrainerContext(AlgorithmType::Perceptron),
			  diff_length(diff_len),
			  out_diff_summ(new value_t[diff_length]),
			  in_length(in_len),
			  in_data(new value_t[in_length]) {}

		virtual ~PerceptronContext(){
			if (out_diff_summ != nullptr) {
				delete[] out_diff_summ;
				out_diff_summ = nullptr;
				diff_length = 0;
			}
			if (in_data != nullptr) {
				delete[] in_data;
				in_data = nullptr;
				in_length = 0;
			}
		}
	};

	template<typename TValue>
	class PerceptronTrainer : public Trainer<TValue>, public PerceptronExecutorBase<TValue> {
	public:
		using trainer_t = Trainer<TValue>;
		using value_t = typename trainer_t::value_t;
		using instance_ptr_t = typename trainer_t::instance_ptr_t;
		using activation_function_t = value_t (*)(const value_t& );
		using trainer_context_t = typename trainer_t::trainer_context_t;
		using perceptron_executor_base_t = PerceptronExecutorBase<TValue>;
		using perceptron_context_t = PerceptronContext<TValue>;
	protected:

		PerceptronInstance<TValue>& getPerceptronInstance() const {
			Instance<TValue>& inst = *trainer_t::instance;
			return static_cast<PerceptronInstance<TValue>&>(inst);
		}

		PerceptronTrainer(instance_ptr_t&& instance)
			:	trainer_t::Trainer(move(instance)),
				perceptron_executor_base_t::PerceptronExecutorBase(getPerceptronInstance().getPerceptron().getFunction()) {}
	public:
		virtual std::unique_ptr<trainer_context_t> makeContext() const override final {
			return std::unique_ptr<trainer_context_t>(move(perceptron_context_t::make(getPerceptronInstance().getPerceptron())));
		}
	};


	template<typename TValue>
	class FlatPerceptronTrainer : public PerceptronTrainer<TValue> {
	public:
		using perceptron_trainer_t = PerceptronTrainer<TValue>;
		using perceptron_executor_base_t = typename perceptron_trainer_t::perceptron_executor_base_t;
		using trainer_t = typename perceptron_trainer_t::trainer_t;
		using value_t = typename perceptron_trainer_t::value_t;
		using instance_ptr_t = typename trainer_t::instance_ptr_t;
		using trainer_context_t = typename perceptron_trainer_t::trainer_context_t;
		using perceptron_context_t = typename perceptron_trainer_t::perceptron_context_t;
	private:
		const FlatPerceptron& getFlatPerceptron() const {
			const Algorithm& alg = trainer_t::instance->getAlgorithm();
			return alg.as<FlatPerceptron>();
		}
	protected:

		virtual bool train(trainer_context_t& context, value_t* in_data, value_t* out_data, value_t* etalon = nullptr) const override final {
			if (context.target != AlgorithmType::Perceptron) throw runtime_error("Функция обучения перцептрона получила неверный контекст."s);
			perceptron_context_t& pcntx = static_cast<perceptron_context_t&>(context);
			value_t* out_diff_summ = pcntx.out_diff_summ;
			bool result = false;
			const FlatPerceptron& alg = getFlatPerceptron();
			auto& inst = perceptron_trainer_t::getPerceptronInstance();
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
				out_diff_summ[i] = 0;
				if (part1_length > 0) perceptron_executor_base_t::RowMulCol(part1_length, &in_data[rows_offset], &weight_col[0], out_diff_summ[i]);
				if (part2_length > 0) perceptron_executor_base_t::RowMulCol(part2_length, &in_data[rows_offset], &weight_col[part1_length], out_diff_summ[i]);
				if (use_shift) out_diff_summ[i] += inst.getWeight(rows_count - 1, i);
				// Пересчитываем смещение индекса входного вектора.
				rows_offset += alg.getRowsOffset();
				rows_offset = rows_offset % in_length;
			}
			perceptron_executor_base_t::activation_func(out_diff_summ, out_data, out_length);
			switch (perceptron_executor_base_t::activation_diff_type) {
				case perceptron_executor_base_t::activation_diff_type_t::AlwaysOne:
					for(size_t i = 0; i < pcntx.diff_length; ++i) {
						out_diff_summ[i] = 1;
					}
					break;
				case perceptron_executor_base_t::activation_diff_type_t::AlwaysZero:
					for(size_t i = 0; i < pcntx.diff_length; ++i) {
						out_diff_summ[i] = 0;
					}
					break;
				case perceptron_executor_base_t::activation_diff_type_t::ByArgument:
					perceptron_executor_base_t::activation_diff(out_diff_summ, out_diff_summ, pcntx.diff_length);
					break;
				case perceptron_executor_base_t::activation_diff_type_t::ByValue:
					perceptron_executor_base_t::activation_diff(out_data, out_diff_summ, pcntx.diff_length);
					break;
			}
			memcpy(pcntx.in_data, in_data, in_length);
			if (etalon != nullptr)
				trainer_t::error_diff_function(out_length, etalon, out_data, out_diff_summ, true);
			return result;
		}

		virtual void fix(trainer_context_t& context, value_t* err_diff_out = nullptr, value_t* err_diff_in = nullptr) override final {
			if (context.target != AlgorithmType::Perceptron) throw runtime_error("Функция обучения перцептрона получила неверный контекст."s);
			perceptron_context_t& pcntx = static_cast<perceptron_context_t&>(context);
			const FlatPerceptron& alg = getFlatPerceptron();
			auto& inst = perceptron_trainer_t::getPerceptronInstance();
			value_t* in_data = pcntx.in_data;
			bool use_shift = alg.getUseShiftInput();
			size_t in_length = alg.getInputLength();
			size_t out_length = alg.getOutputLength();
			size_t rows_count = alg.getWeightsRows();
			size_t rows_offset = 0;
			size_t avl_rows_count = rows_count - (use_shift ? 1 : 0);
			value_t speed = trainer_t::train_speed;
			if (err_diff_in != nullptr) {
				memset(err_diff_in, 0, in_length * sizeof(value_t));
				for(size_t i = 0; i < out_length; ++i) {
					value_t err_diff_summ_i = err_diff_out == nullptr ? pcntx.out_diff_summ[i] : err_diff_out[i] * pcntx.out_diff_summ[i];
					value_t* wcol = inst.getWeightColumn(i);
					for(size_t j = 0; j < avl_rows_count; ++j) {
						size_t in_index = (j + rows_offset)  % in_length;
						err_diff_in[in_index] += wcol[j] * err_diff_summ_i;
						wcol[j] -= err_diff_summ_i * in_data[in_index] * speed;
					}
					if (use_shift) {
						wcol[rows_count - 1] -= err_diff_summ_i * speed;
					}
					rows_offset += alg.getRowsOffset();
					rows_offset = rows_offset % in_length;
				}
			} else {
				for(size_t i = 0; i < out_length; ++i) {
					value_t err_diff_summ_i = err_diff_out == nullptr ? pcntx.out_diff_summ[i] : err_diff_out[i] * pcntx.out_diff_summ[i];
					value_t* wcol = inst.getWeightColumn(i);
					for(size_t j = 0; j < avl_rows_count; ++j) {
						size_t in_index = (j + rows_offset)  % in_length;
						wcol[j] -= err_diff_summ_i * in_data[in_index] * speed;
					}
					if (use_shift) {
						wcol[rows_count - 1] -= err_diff_summ_i * speed;
					}
					rows_offset += alg.getRowsOffset();
					rows_offset = rows_offset % in_length;
				}
			}
		}

	public:
		FlatPerceptronTrainer(instance_ptr_t&& instance) : perceptron_trainer_t::PerceptronTrainer(move(instance)) {}
	};


} /* namespace algs::l4ai */




#endif /* L4AI_INC_PERCEPTRONTRAINER_HPP_ */
