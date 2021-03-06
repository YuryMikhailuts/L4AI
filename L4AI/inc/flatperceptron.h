/*
 * flatperceptron.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef FLATPERCEPTRON_H
#define FLATPERCEPTRON_H
#include <perceptron.h>

namespace l4ai::algs {

	/**
	 * @brief Данный класс описывает самый простой вид перцептрона: плоский перцептрон.
	 * Такой перцептрон просто использует весь входной набор данных в качестве входного вектора, а единственный получившийся результирующий вектор в качестве выходного набора данных.
	 *
	 * При этом, поскольку длинна входного набора данных можнт не совпадать с количеством строк матрицы весов, данный вид перцептрона поддерживает возможность удлинения столбцов матрицы, путём дописывания некоторого количества нулей в начале столбца следующим образом:
	 *
	 *  + Пусть исходная матрица выглядит так:
	 *
	 *   | a b c d |
	 *   | e f g h |
	 *   | i j k l |
	 *   | m n o p |
	 *
	 *  + Тогда матрицы с дописанными нулями будут выглядеть так:
	 *
	 *     1. В начале каждого следующего столбца на 1 ноль больше:
	 *
	 *           | a 0 0 0 |
	 *           | e b 0 0 |
	 *           | i f c 0 |
	 *           | m j g d |
	 *           | 0 n k h |
	 *           | 0 0 o l |
	 *           | 0 0 0 p |
	 *
	 *     2. В начале каждого следующего столбца на 2 нуля больше:
	 *
	 *           | a 0 0 0 |
	 *           | e 0 0 0 |
	 *           | i b 0 0 |
	 *           | m f 0 0 |
	 *           | 0 j c 0 |
	 *           | 0 n g 0 |
	 *           | 0 0 k d |
	 *           | 0 0 o h |
	 *           | 0 0 0 l |
	 *           | 0 0 0 p |
	 *
	 *
	 *     X. И так далее, пока сдвиг каждого столбца не будет равен изначальному количеству строк, что увеличит количество строк в число, равное количеству столбцов. Очевидно, что далее сдвигать стольцы бессмыслено.
	 *
	 * @remarks
	 *  + Если каким-то данным нехватает строк матрицы, то такие данные будут просто игнорироваться алгоритмом.
	 *  + Если каким-то строкам матрицы нехватает данных, то они будут получать на вход 0.
	 */
	class L4AI_EXPORT FlatPerceptron : public Perceptron {
	private:
		/**
		 * @brief Значение отночительного сдвига, на которое каждый следующий столбец будет сдвигаться относительного предыдущего.
		 */
		size_t rows_offset;
		/**
		 * @brief Значение @codeline true@endcode требует от алгоритма добавить к имеющимся входам ещё один, на который всегда будет подаваться 1.
		 * Значение @codeline false@endcode выключает такое поведение.
		 */
		bool use_shift_input;
	public:
		FlatPerceptron();
		FlatPerceptron ( size_t input_length, size_t output_length );
		size_t getRowsOffset() const;
		void setRowsOffset ( const size_t& value );
		void setRowsOffset ( size_t&& value );
		bool getUseShiftInput() const;
		void setUseShiftInput ( bool value );
		inline static FlatPerceptron* make(size_t in_len, size_t out_len, ActivationFunctions af = ActivationFunctions::Trivial, bool useShift = false) {
			FlatPerceptron* fp = new FlatPerceptron(in_len, out_len);
			fp->setWeightsRows(in_len + (useShift ? 1 : 0));
			fp->setWeightsColumns(out_len);
			fp->setFunction(af);
			fp->setRowsOffset(0);
			fp->setUseShiftInput(useShift);
			return fp;
		}
		inline static FlatPerceptron* make(size_t in_len, size_t out_len, size_t rows_offset, ActivationFunctions af = ActivationFunctions::Trivial, bool useShift = false) {
			if (in_len <= (out_len - 1) * rows_offset) return nullptr;
			FlatPerceptron* fp = new FlatPerceptron(in_len, out_len);
			fp->setWeightsRows(in_len - (out_len - 1) * rows_offset + (useShift ? 1 : 0));
			fp->setWeightsColumns(out_len);
			fp->setFunction(af);
			fp->setRowsOffset(rows_offset);
			fp->setUseShiftInput(useShift);
			return fp;
		}
	};

}	// l4ai::algs

#endif // FLATPERCEPTRON_H
