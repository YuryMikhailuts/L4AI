/*
 * instance.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <instance.h>
#include <perceptroninstance.h>
#include <pipelineinstance.h>

namespace l4ai::algs {
	using namespace std;

	template<typename TValue> Instance<TValue>::Instance ( unique_ptr<Algorithm>&& algorithm ) : algorithm ( move ( algorithm ) ) {}

	template<typename TValue> Instance<TValue>::Instance ( Algorithm*&& algorithm ) : algorithm ( move ( algorithm ) ) { algorithm = nullptr; }

	template<typename TValue>
	const Algorithm& Instance<TValue>::getAlgorithm() const { return *algorithm; }

	template<typename TValue>
	std::unique_ptr<Instance<TValue>> Instance<TValue>::make(std::unique_ptr<Algorithm>&& algorithm) {
		return make(move(&*algorithm));
	}

	template<typename TValue>
	std::unique_ptr<Instance<TValue>> Instance<TValue>::make(Algorithm*&& algorithm) {
		Instance<TValue>* result = nullptr;
		switch (algorithm->type()) {
			case AlgorithmType::Perceptron: {
				Perceptron* perceptron = static_cast<Perceptron*>(algorithm);
				result = new PerceptronInstance<TValue>(move(perceptron));
				break;
			}
			case AlgorithmType::Pipe: {
				Pipe* pipe = static_cast<Pipe*>(algorithm);
				switch (pipe->getPipeType()) {
					case PipeType::Line: {
						PipeLine* pipeline = static_cast<PipeLine*>(pipe);
						result = new PipeLineInstance<TValue>(move(pipeline));
						break;
					}
					case PipeType::Tree: {
						break;
					}
				}
				break;
			}
		}
		return std::unique_ptr<Instance<TValue>>(result);
	}
}

#endif // INSTANCE_HPP
