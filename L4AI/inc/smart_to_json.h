/*
 * configurable.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 марта 2021 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */


#ifndef LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_TO_JSON_H
#define LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_TO_JSON_H

#include <smart_objects.h>
#include <iostream>

namespace l4ai::smart {

    class SmartToJson {
    private:
        std::ostream &out;
        std::string tab;

        void tab_inc();

        void tab_dec();

        void saveImpl(const std::shared_ptr<SmartIntObject> &smartInt);

        void saveImpl(const std::shared_ptr<SmartFloatObject> &smartFloat);

        void saveImpl(const std::shared_ptr<SmartArrayObject> &smartArray);

        void saveImpl(const std::shared_ptr<SmartObjectArray> &smartArray);

        void saveImpl(const std::shared_ptr<SmartIntArrayObject> &smartArray);

        void saveImpl(const std::shared_ptr<SmartFloatArrayObject> &smartArray);

        void saveImpl(const std::shared_ptr<SmartMapObject> &smartMap);

        void saveImpl(const std::shared_ptr<SmartStringObject> &smartString);

    public:
        explicit SmartToJson(std::ostream &out);

        void save(const std::shared_ptr<SmartObject> &smartObject);
    };

}

#endif //LIBRARY4_ARTIFICIAL_INTELLIGENCE_SMART_TO_JSON_H
