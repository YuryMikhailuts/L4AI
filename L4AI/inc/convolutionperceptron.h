#ifndef CONVOLUTIONPERCEPTRON_H
#define CONVOLUTIONPERCEPTRON_H
#include <perceptron.h>
#include <memory>

namespace l4ai::algs {

	/**
	 * @brief Данный класс описывает свёрточный перцептрон.
	 *
	 * Такой перцептрон устроен следующим образом:
	 *  + Входной и выходной наборы данных представляются как участок некоторого N-мерного пространства.
	 *  + По входному набору данных пробегается окно фиксированного размера и той же размерности, что и пространство входных данных.
	 *  + Попавшие в окно данные становятся входным вектором и умножаются на матрицу так, как это описано в @link l4ai::algs::Perceptron @endlink.
	 *  + Из результирующих векторов формируется пространство той же размерности, что и входное.
	 *  + Результирующие вектора в выходном пространстве располагаются так, что бы отображение пространства входных векторов в выходные было непрерывным, то есть для любых двух входных векторов, сдвинутых друг относительно друга лишь на 1 столбец или на 1 строку во входном пространстве, соответствующие вектора в выходном пространстве должны располагаться по соседству.
	 */
	class L4AI_EXPORT ConvolutionPerceptron : public Perceptron {
	private:
		/**
		 * @brief Размерность пространства, в котором производится свёртка.
		 * @remarks
		 *  Количество стрипов всегда на 1 меньше размерности пространства.
		 *  Так для 1-мерного пространства поля @link l4ai::algs::ConvolutionPerceptron::input_strips @endlink и @link l4ai::algs::ConvolutionPerceptron::window_strips @endlink будут равны @codeline nullptr@endcode.
		 */
		size_t dimension;
		/**
		 * @brief Массив, содержащий размеры набора входных данных по каждому из измерений, кроме последнего. Так для 3-мерного пространства данный массив будет содержать 2 значения.
		 */
		std::shared_ptr<size_t[]> input_strips;
		/**
		 * @brief Массив, содержащий размеры окна по каждому из измерений, кроме последнего. Так для 3-мерного пространства данный массив будет содержать 2 значения.
		 */
		std::shared_ptr<size_t[]> window_strips;
	public:
		ConvolutionPerceptron();
		ConvolutionPerceptron ( size_t input_length, size_t output_length, size_t dimension );
		size_t& getInputStrip ( size_t index );
		const size_t& getInputStrip ( size_t index ) const;
		void setInputStrip ( size_t index, const size_t& value );
		void setInputStrip ( size_t index, size_t&& value );
		size_t& getWindowStrip ( size_t index );
		const size_t& getWindowStrip ( size_t index ) const;
		void setWindowStrip ( size_t index, const size_t& value );
		void setWindowStrip ( size_t index, size_t&& value );
		size_t getDimension() const;
		size_t getStripsLength() const;
		/**
		 * @brief Устанавливает новую размерность пространства для данного свёрточного перцептрона.
		 * @param value новое значение размерности пространства.
		 * @remark Обратите внимание на то, что данный метод не только меняет значение поля @link l4ai::algs::ConvolutionPerceptron::dimension @endlink, но и перевыделяет мессивы @link l4ai::algs::ConvolutionPerceptron::input_strips @endlink и @link l4ai::algs::ConvolutionPerceptron::window_strips @endlink.
		 */
		void setDimension ( size_t value );
	};

}	// l4ai::algs

#endif // CONVOLUTIONPERCEPTRON_H
