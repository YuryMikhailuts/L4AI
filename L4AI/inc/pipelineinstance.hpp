/*
 * pipelineinstance.hpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 10 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <pipelineinstance.h>
#include <pipe.h>
#include <memory>

namespace l4ai::algs {
	using namespace std;
    using namespace l4ai::smart;
    using namespace l4ai::configure;

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( std::unique_ptr<PipeLine>&& algorithm )
		: instance_t::Instance(std::unique_ptr<Algorithm>(algorithm.release())), layers(move(makeLayers(getPipeLine()))) {}

	template<typename TValue>
	PipeLineInstance<TValue>::PipeLineInstance ( PipeLine*&& algorithm )
		: PipeLineInstance(move(std::unique_ptr<PipeLine>(algorithm))) {}

	template<typename TValue>
	const PipeLine& PipeLineInstance<TValue>::getPipeLine() const {
		return static_cast<const PipeLine&>(instance_t::getAlgorithm());
	}

	template<typename TValue>
	PipeLine& PipeLineInstance<TValue>::getPipeLine() {
		return static_cast<PipeLine&>(instance_t::getAlgorithm());
	}

	template<typename TValue>
	shared_ptr<shared_ptr<Instance<TValue>>[]> PipeLineInstance<TValue>::makeLayers(PipeLine& algorithm) {
		size_t count = algorithm.getLayersCount();
		array_ptr_t<instance_ptr_t> layers (new instance_ptr_t[count]);
		for(size_t i = 0; i < count; ++i) {
			layers[i] = move(instance_ptr_t(instance_t::make(&algorithm.getLayer(i))));
		}
		return layers;
	}

	template<typename TValue>
	size_t PipeLineInstance<TValue>::getLayersCount() const {
		return static_cast<const PipeLine&>(Instance<TValue>::getAlgorithm()).getLayersCount();
	}

	template<typename TValue>
	Instance<TValue>& PipeLineInstance<TValue>::getLayer(size_t index) {
		return *layers[index];
	}

	template<typename TValue>
	const Instance<TValue>& PipeLineInstance<TValue>::getLayer(size_t index) const {
		return *layers[index];
	}

	template<typename TValue>
	shared_ptr<Instance<TValue>> PipeLineInstance<TValue>::getLayerPtr(size_t index) {
		return layers[index];
	}

    template<typename TValue>
    void PipeLineInstance<TValue>::setConfiguration(std::shared_ptr<smart::SmartObject> configuration) {
        switch (configuration->smartType()) {
            case SmartType::Array: {
                if (configuration->smartClass != "PipeLineInstance")
                    throw InvalidConfigurationException(
                            "Класс Смарт объекта конфигурации для 'PipeLineInstance' должен быть 'PipeLineInstance'.");
                auto confArray = configuration->asArray();
                if (confArray->size() != getLayersCount())
                    throw InvalidConfigurationException(
                            "Количество слоёв Смарт объекта конфигурации не совпадает с количеством слоёв конфигурируемого объекта 'PipeLineInstance'.");
                for(size_t i = 0; i < getLayersCount(); ++i) {
                    getLayer(i).setConfiguration(confArray->at(i));
                }
                break;
            }
            default:
                throw InvalidConfigurationException("Тип Смарт объекта конфигурации для 'PerceptronInstance' должен быть SmartType::Map.");
        }
    }

    template<typename TValue>
    std::shared_ptr<smart::SmartObject> PipeLineInstance<TValue>::getConfiguration() const {
        auto result = SmartObject::create<SmartObjectArray>("PipeLineInstance");
        for(size_t i = 0; i < getLayersCount(); ++i) {
            result->data.push_back(getLayer(i).getConfiguration());
        }
        return result;
    }

}

