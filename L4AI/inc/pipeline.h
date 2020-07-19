/*
 * pipeline.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef L4AI_INC_PIPELINE_H_
#define L4AI_INC_PIPELINE_H_
#include <pipe.h>

namespace l4ai::algs {

	class L4AI_EXPORT PipeLine : public Pipe {
	private:
		Algorithm** layers;
	public:
		PipeLine();
		PipeLine(size_t layer_count, Algorithm**const layers = nullptr);

		bool isEmptyLayer(size_t index);
		Algorithm& getLayer(size_t index);
		const Algorithm& getLayer(size_t index) const;
		void setLayer(size_t index, Algorithm* algorithm);
	};

}

#endif /* L4AI_INC_PIPELINE_H_ */
