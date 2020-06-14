#ifndef PERCEPTRONINSTANCE_HPP
#define PERCEPTRONINSTANCE_HPP
#include <perceptroninstance.h>


namespace l4ai::algs {
	using namespace std;

	template<typename TValue>
	PerceptronInstance<TValue>::PerceptronInstance ( unique_ptr<Perceptron>&& perceptron )
		: Instance<TValue>::Instance(move(perceptron)),
		  cols_count(getPerceptron().getWeightsColumns()),
		  rows_count(getPerceptron().getWeightsRows()),
		  weights(new value_t[cols_count * rows_count]) {}

	template<typename TValue>
	PerceptronInstance<TValue>::~PerceptronInstance() {
		if (weights != nullptr) {
			delete[] weights;
			weights = nullptr;
		}
	}

	template<typename TValue>
	PerceptronInstance<TValue>::PerceptronInstance ( Perceptron*&& perceptron )
		: PerceptronInstance(unique_ptr<Perceptron>(move(perceptron))) {}

	template<typename TValue>
	size_t PerceptronInstance<TValue>::getWeightsCols() const {
		return cols_count;
	}

	template<typename TValue>
	size_t PerceptronInstance<TValue>::getWeightsRows() const {
		return rows_count;
	}

}	// l4ai::algs

#endif // PERCEPTRONINSTANCE_HPP
