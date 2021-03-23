/*
 * configurable.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <configurable.h>

namespace l4ai::configure {

    Configurable::~Configurable() = default;

    InvalidConfigurationException::InvalidConfigurationException(const std::string &arg) : invalid_argument(arg) {}

    InvalidConfigurationException::InvalidConfigurationException(const char *string) : invalid_argument(string) {}
}