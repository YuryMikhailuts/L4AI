/*
 * perceptroninstance.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef PERCEPTRONINSTANCE_HPP
#define PERCEPTRONINSTANCE_HPP
#include <perceptroninstance.h>
#include <math.h>

namespace l4ai::algs {
    using namespace std;
    using namespace l4ai::smart;
    using namespace l4ai::configure;


	template<typename TValue>
	PerceptronInstance<TValue>::PerceptronInstance ( unique_ptr<Perceptron>&& perceptron )
		: Instance<TValue>::Instance(move(perceptron)),
		  cols_count(getPerceptron().getWeightsColumns()),
		  rows_count(getPerceptron().getWeightsRows()),
		  weights(new value_t[cols_count * rows_count]) {
		for(size_t c = 0; c < cols_count; ++c) {
			for(size_t r = 0; r < rows_count; ++r) {
				getWeight(r, c) = (((double)rand()) / RAND_MAX) * 2 - 1;
			}
		}
	}

	template<typename TValue>
	PerceptronInstance<TValue>::~PerceptronInstance() {
		if (weights != nullptr) {
			delete[] weights;
			weights = nullptr;
		}
	}

	template<typename TValue>
	PerceptronInstance<TValue>::PerceptronInstance ( Perceptron*&& perceptron )
		: PerceptronInstance(unique_ptr<Perceptron>(move(perceptron))) {}

	template<typename TValue>
	size_t PerceptronInstance<TValue>::getWeightsCols() const {
		return cols_count;
	}

	template<typename TValue>
	size_t PerceptronInstance<TValue>::getWeightsRows() const {
		return rows_count;
	}

    template<typename TValue>
    void PerceptronInstance<TValue>::setConfiguration(std::shared_ptr<SmartObject> configuration) {
        switch (configuration->smartType()) {
            case SmartType::Map: {
                if (configuration->smartClass != "PerceptronInstance")
                    throw InvalidConfigurationException(
                            "Класс Смарт объекта конфигурации для 'PerceptronInstance' должен быть 'PerceptronInstance'.");
                auto confMap = configuration->asMap();
                cols_count = (size_t)*(*confMap)["cols_count"]->asInt();
                rows_count = (size_t)*(*confMap)["rows_count"]->asInt();
                auto weightsArray = (*confMap)["weights"]->asArray();
                switch (weightsArray->arraySubType()) {
                    case SmartArraySubType::Int:
                    case SmartArraySubType::Float:
                        weightsArray->fill(weights, cols_count * rows_count);
                        break;
                    default:
                        throw InvalidConfigurationException(
                                "Поле 'weights' смарт объекта конфигурации для 'PerceptronInstance' должено быть массивом 'Int' или 'Float'.");
                }
                break;
            }
            default:
                throw InvalidConfigurationException("Тип Смарт объекта конфигурации для 'PerceptronInstance' должен быть SmartType::Map.");
        }
    }

    template<typename TValue>
    std::shared_ptr<SmartObject> PerceptronInstance<TValue>::getConfiguration() const {
        auto result = SmartObject::create<SmartMapObject>("PerceptronInstance");
        result->template setInt("cols_count", cols_count);
        result->template setInt("rows_count", rows_count);
        result->template setFloatArray("weights", weights, cols_count * rows_count);
        return result;
    }

}	// l4ai::algs

#endif // PERCEPTRONINSTANCE_HPP
