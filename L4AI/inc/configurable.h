/*
 * configurable.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#ifndef LIBRARY4_ARTIFICIAL_INTELLIGENCE_CONFIGURABLE_H
#define LIBRARY4_ARTIFICIAL_INTELLIGENCE_CONFIGURABLE_H
#include <smart_objects.h>
#include <exception>
#include <stdexcept>

namespace l4ai::configure {

    struct Configurable {
        virtual ~Configurable();

        virtual void setConfiguration(std::shared_ptr<smart::SmartObject> configuration) = 0;

        [[nodiscard]] virtual std::shared_ptr<smart::SmartObject> getConfiguration() const = 0;
    };

    class InvalidConfigurationException : std::invalid_argument {
    public:
        explicit InvalidConfigurationException(const std::string &arg);

        explicit InvalidConfigurationException(const char *string);
    };

}
#endif //LIBRARY4_ARTIFICIAL_INTELLIGENCE_CONFIGURABLE_H
