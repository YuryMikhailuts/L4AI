#ifndef PERCEPTRONSPECIFIER_H
#define PERCEPTRONSPECIFIER_H
#include <inttypes.h>
#include <algorithm.h>

namespace l4ai::algs {

	enum class PerceptronType { Abstract, Flat, Convolution };

	/**
	 * @brief Описывает набор набустимых типов функции активации.
	 * @remark Данное перечисление никак не связано с реализациями функций активации и лишь предоставляет набор их имён для того, что бы можно было указать выбранную функцию.
	 */
	enum class ActivationFunctions : uint32_t {
		/**
		 * @brief f(x) = x
		 **/
		Trivial,
		/**
		 * @brief f(x) = (x < 0) ? 0 : x;
		 */
		ReLU,
		/**
		 * @brief f(x) = σ(x) = 1 / (1 + exp(-x))
		 */
		Logistic,
		/**
		 * @brief f(x) = th(x) = (exp(x) - exp(-1)) / (exp(x) + exp(-x))
		 */
		HyperbolicTangent,
		/**
		 * @brief f(x) = arctg(x)
		 */
		ArcTangent
	};

	/**
	 * @brief Данная разновидность алгоритма называется "Перцептрон". Помимо входов и выходов у него есть, также, матрица "весов" и функция активации.
	 * Работает данный алгоритм следующим образом:
	 *  1. Из входных данных выбираются один или несколько векторов
	 *  2. Каждый из векторов умножается на матрицу, в результате чего волучается новый вектор.
	 *  3. Для каждой компоненты каждого нового вектора вычисляется функция активации, значение которой заменяет значение соответствующей компоненты вектора.
	 *  4. Из всех полученных, описанным выше способом, векторов, если их несколько, формируется выходной набор данных. Если же результирующий вектор один, то он и является выходным набором данных.
	 *
	 * Конкретные реализации Селектора и композиции определяются в классах потомках.
	 *
	 *      _____________________________
	 *     |                            /|
	 * В-->|\                          /К|->В
	 *     |С\  _                     / о|
	 * х-->|е \|В|     ________      /  м|->ы
	 *     |л  |е|    |Матрица|     |   п|
	 * о-->|е  |к| X  |       | ===>|   о|->х
	 *     |к  |т|    |весов  | ===>|   з|
	 * д-->|о /|о|    |_______|      \  ц|->о
	 *     |р/ |р|                    \ и|
	 * ы-->|/                          \я|->д
	 *     |                            \|
	 *     |_____________________________|
	 */
	class L4AI_EXPORT Perceptron : public Algorithm {
	private:
		size_t weights_columns;
		size_t weights_rows;
		ActivationFunctions function;
	protected:
		PerceptronType type;
	public:
		Perceptron();
		Perceptron ( size_t input_length, size_t output_length );
		size_t getWeightsColumns() const;
		void setWeightsColumns ( const size_t& value );
		void setWeightsColumns ( size_t&& value );
		size_t getWeightsRows() const;
		void setWeightsRows ( const size_t& value );
		void setWeightsRows ( size_t&& value );
		ActivationFunctions getFunction() const;
		void setFunction ( const ActivationFunctions& function );
		void setFunction ( ActivationFunctions&& function );
		inline PerceptronType getPerceptronType() const { return Perceptron::type; }

	};

}	// l4ai::algs

#endif // PERCEPTRONSPECIFIER_H
