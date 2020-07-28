/*
 * options.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 28 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef TESTS_INC_OPTIONS_H_
#define TESTS_INC_OPTIONS_H_
#include <filesystem>


#ifndef TESTS_DATA_DIRECTORY
#define TESTS_DATA_DIRECTORY ""
#error "Не удалось обнаружить путь к каталогу с тестовыми данными (-DTESTS_DATA_DIRECTORY=\"Путь к каталогу с тестовыми данными.\")"
#endif

inline static const std::filesystem::path tests_directory (TESTS_DATA_DIRECTORY);





#endif /* TESTS_INC_OPTIONS_H_ */
