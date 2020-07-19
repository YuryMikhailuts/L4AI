/*
 * pipelinetrainer.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 15 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PIPELINETRAINER_HPP_
#define L4AI_INC_PIPELINETRAINER_HPP_
#include <pipelineinstance.h>
#include <trainer.h>


namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	class PipeLineContext : public ITrainerContext<TValue> {
	public:
		using trainer_t = Trainer<TValue>;
		using trainer_ptr_t = typename trainer_t::trainer_ptr_t;
		using trainer_context_t = ITrainerContext<TValue>;
		using value_t = TValue;
		static constexpr std::unique_ptr<PipeLineContext<TValue>> make(trainer_ptr_t* trainers, size_t layers_count) {
			ITrainerContext<TValue>** contexts = new ITrainerContext<TValue>*[layers_count];
			for(size_t i = 0; i < layers_count; ++i) {
				contexts[i] = trainers[i]->makeContext().release();
			}
			return std::unique_ptr<PipeLineContext<TValue>>(new PipeLineContext<TValue>(contexts));
		}

		ITrainerContext<TValue>** contexts;

		PipeLineContext(ITrainerContext<TValue>** contexts)
			: trainer_context_t::ITrainerContext(AlgorithmType::Pipe),
			  contexts(contexts) { }

		virtual ~PipeLineContext(){
			if (contexts != nullptr) {
				delete[] contexts;
				contexts = nullptr;
			}
		}
	};

	template<typename TValue>
	class PipeLineTrainer : public Trainer<TValue> {
	public:
		using trainer_t = Trainer<TValue>;
		using trainer_ptr_t = typename trainer_t::trainer_ptr_t;
		using value_t = typename trainer_t::value_t;
		using instance_ptr_t = typename trainer_t::instance_ptr_t;
		using trainer_context_t = typename trainer_t::trainer_context_t;
		using pipeline_context_t = PipeLineContext<TValue>;
	protected:
		size_t max_layer_length;
		value_t* internal_data[2];
		trainer_ptr_t* trainers;

		static size_t findMaxLength(const PipeLineInstance<TValue>& instance) {
			size_t max_length = 0;
			size_t int_count = instance.getLayersCount();
			if (int_count > 0) {
				for(size_t i = 0; i < int_count; ++i) {
					max_length = max(max_length, instance.getLayer(i).getAlgorithm().getOutputLength());
				}
			}
			return max_length;
		}

		static trainer_ptr_t* makeTrainers(PipeLineInstance<TValue>& instance, ErrorFunctionType error_function_type) {
			size_t layers_count = instance.getLayersCount();
			trainer_ptr_t* trainers = new trainer_ptr_t[layers_count];
			for(size_t i = 0; i < layers_count; ++i) {
				trainers[i] = trainer_t::make(instance.getLayerPtr(i), error_function_type);
			}
			return trainers;
		}

		PipeLineInstance<TValue>& getPipeLineInstance() const {
			Instance<TValue>& inst = *trainer_t::instance;
			return static_cast<PipeLineInstance<TValue>&>(inst);
		}

		PipeLineInstance<TValue>& getPipeLineInstance() {
			Instance<TValue>& inst = *trainer_t::instance;
			return static_cast<PipeLineInstance<TValue>&>(inst);
		}


	public:

		PipeLineTrainer(instance_ptr_t&& instance)
			:	trainer_t::Trainer(move(instance)),
				max_layer_length(findMaxLength(getPipeLineInstance())),
				internal_data{ new value_t[max_layer_length], new value_t[max_layer_length]},
				trainers(makeTrainers(getPipeLineInstance(), trainer_t::error_function_type)) {}

		virtual ~PipeLineTrainer() {
			if (trainers != nullptr) {
				delete[] trainers;
				trainers = nullptr;
			}
		}

		virtual std::unique_ptr<trainer_context_t> makeContext() const override final {
			return std::unique_ptr<trainer_context_t>(move(pipeline_context_t::make(trainers, getPipeLineInstance().getLayersCount())));
		}


		virtual bool train(trainer_context_t& context, value_t* in_data, value_t* out_data, value_t* etalon = nullptr) const override {
			if (context.target != AlgorithmType::Pipe)  throw runtime_error("Функция обучения конвейера получила неверный контекст."s);
			pipeline_context_t& plcntx = static_cast<pipeline_context_t&>(context);
			const PipeLineInstance<TValue>& instance =  getPipeLineInstance();
			size_t layers_count = instance.getLayersCount();
			bool result = true;
			value_t* idp = in_data;
			value_t* odp = internal_data[0];
			for(size_t i = 0; result && i < layers_count - 1; ++i) {
				result &= trainers[i]->train(*plcntx.contexts[i], idp, odp);
				idp = internal_data[i & 1];
				odp = internal_data[(i + 1) & 1 ];
				size_t idp_len = instance.getLayer(i).getAlgorithm().getOutputLength();
				if (max_layer_length > idp_len)
					memset(&idp[idp_len], 0, max_layer_length - idp_len);
			}
			result &= trainers[layers_count - 1]->train(*plcntx.contexts[layers_count - 1], idp, out_data, etalon);
			return result;
		}

		virtual void fix(trainer_context_t& context, /*in*/ value_t* err_diff_out = nullptr, /*out*/ value_t* err_diff_in = nullptr) override {
			if (context.target != AlgorithmType::Pipe)  throw runtime_error("Функция обучения конвейера получила неверный контекст."s);
			pipeline_context_t& plcntx = static_cast<pipeline_context_t&>(context);
			const PipeLineInstance<TValue>& instance =  getPipeLineInstance();
			size_t layers_count = instance.getLayersCount();
			value_t* idp = err_diff_in;
			value_t* odp = internal_data[(layers_count - 1) & 1];
			for(size_t i = layers_count - 1; i > 0; --i) {
				trainers[i]->fix(*plcntx.contexts[i], idp, odp);
				idp = internal_data[i & 1];
				odp = internal_data[(i - 1) & 1 ];
				size_t idp_len = instance.getLayer(i).getAlgorithm().getInputLength();
				if (max_layer_length > idp_len)
					memset(&idp[idp_len], 0, max_layer_length - idp_len);
			}
			trainers[0]->fix(*plcntx.contexts[0], idp, err_diff_in);
		}

	};

}



#endif /* L4AI_INC_PIPELINETRAINER_HPP_ */
