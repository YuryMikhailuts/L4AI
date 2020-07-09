/*
 * pipe.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef PIPE_H
#define PIPE_H
#include <algorithm.h>

namespace l4ai::algs {

	enum class PipeType { Abstract, Line, Tree };


	class L4AI_EXPORT Pipe : public Algorithm {
	private:
		size_t layer_count;
	protected:
		PipeType type;
	public:
		Pipe();
		Pipe(size_t layer_count);
		size_t getLayerCount() const;
		void setLayerCount(size_t value);
		inline PipeType getPipeType() const { return Pipe::type; }
	};

}

#endif // PIPE_H
