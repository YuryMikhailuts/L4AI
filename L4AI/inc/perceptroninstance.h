/*
 * perceptroninstance.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef PERCEPTRONINSTANCE_H
#define PERCEPTRONINSTANCE_H
#include <type_traits>
#include <instance.h>
#include <perceptron.h>

namespace l4ai::algs {

	/**
	 * @brief Данный класс описывает модель абстрактного перцептрона.
	 * Веса представлены тут в виде матрицы, а входные данные как вектор.
	 * Таким образом выходные знаения предполагается вычислять как произведение входного вектора на матрицу (матрица справа): Vout = Vin * W.
	 *
	 *                            | W[0][0]  W[0][1]  W[0][2]  W[0][3] |     ( Vin[0] * W[0][0] + Vin[1] * W[1][0] + Vin[2] * W[2][0],
	 * (Vin[0], Vin[1], Vin[2]) * | W[1][0]  W[1][1]  W[1][2]  W[1][3] | =     Vin[0] * W[0][1] + Vin[1] * W[1][1] + Vin[2] * W[2][1],
	 *                            | W[2][0]  W[2][1]  W[2][2]  W[2][3] |       Vin[0] * W[0][2] + Vin[1] * W[1][2] + Vin[2] * W[2][2],
	 *                                                                         Vin[0] * W[0][3] + Vin[1] * W[1][3] + Vin[2] * W[2][3] )
	 */
	template<typename TValue>
	class PerceptronInstance : public Instance<TValue> {
	public:
		using value_t = typename Instance<TValue>::value_t;
	private:
		size_t cols_count;
		size_t rows_count;
		/**
		 * @brief Поскольку Матрица в умножении стоит справа, входной вектор будет умножаться на матрицу по столбцам. Значит и распологаться матрица должна по столбцам, например:
		 *
		 *        | W[0][0]->0  W[0][1]->3  W[0][2]->6  W[0][3]->9 |
		 *        | W[1][0]->1  W[1][1]->4  W[1][2]->7  W[1][3]->10|
		 *        | W[2][0]->2  W[2][1]->5  W[2][2]->8  W[2][3]->11|
		 *
		 * для матрицы 3x4.
		 */
		value_t* weights;
	public:
		PerceptronInstance ( std::unique_ptr<Perceptron>&& perceptron );
		PerceptronInstance ( Perceptron*&& perceptron );
		~PerceptronInstance();
		template<class TPerceptron, typename = std::enable_if_t<std::is_base_of<Perceptron, TPerceptron>::value>>
		PerceptronInstance ( TPerceptron && perceptron ) : PerceptronInstance(new TPerceptron(std::move(perceptron))) {}
		inline const Perceptron& getPerceptron() const {
			const Algorithm& alg = Instance<TValue>::getAlgorithm();
			return alg.as<Perceptron>();
		}

		size_t getWeightsCols() const;
		size_t getWeightsRows() const;
		inline static constexpr const value_t* column(const value_t* weights, size_t col, size_t rows_count) { return &weights[col  * rows_count]; }
		inline static constexpr value_t* column(value_t* weights, size_t col, size_t rows_count) { return &weights[col  * rows_count]; }
		inline static constexpr const value_t& at(const value_t* weights, size_t col, size_t row, size_t rows_count) { return column(weights, col, rows_count)[row]; }
		inline static constexpr value_t& at(value_t* weights, size_t col, size_t row, size_t rows_count) { return column(weights, col, rows_count)[row]; }
		inline value_t& getWeight(size_t row, size_t col) { return at(weights, col, row, rows_count); }
		inline const value_t& getWeight(size_t row, size_t col) const { return at(weights, col, row, rows_count); }
		inline value_t* getWeightColumn(size_t col) { return column(weights, col, rows_count); }
		inline const value_t* getWeightColumn(size_t col) const { return column(weights, col, rows_count); }

        void setConfiguration(std::shared_ptr<smart::SmartObject> configuration) override;

        [[nodiscard]] std::shared_ptr<smart::SmartObject> getConfiguration() const override;
    };

	using PerceptronInstF32 = PerceptronInstance<float>;
	using PerceptronInstF64 = PerceptronInstance<double>;
}

#endif // PERCEPTRONINSTANCE_H
