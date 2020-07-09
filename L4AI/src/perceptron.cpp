/*
 * perceptron.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include "perceptron.h"

namespace l4ai::algs {

	size_t Perceptron::getWeightsColumns() const {
		return weights_columns;
	}

	void Perceptron::setWeightsColumns ( const size_t& value ) {
		weights_columns = value;
	}

	void Perceptron::setWeightsColumns ( size_t&& value ) {
		weights_columns = value;
	}

	size_t Perceptron::getWeightsRows() const {
		return weights_rows;
	}

	void Perceptron::setWeightsRows ( const size_t& value ) {
		weights_rows = value;
	}

	void Perceptron::setWeightsRows ( size_t&& value ) {
		weights_rows = value;
	}

	ActivationFunctions Perceptron::getFunction() const {
		return function;
	}

	void Perceptron::setFunction ( const ActivationFunctions& activation ) {
		this->function = activation;
	}

	void Perceptron::setFunction ( ActivationFunctions&& activation ) {
		this->function = activation;
	}

	Perceptron::Perceptron() : Algorithm(), weights_columns(), weights_rows(), function(), type(PerceptronType::Abstract) {
		alg_type = AlgorithmType::Perceptron;
	}

	Perceptron::Perceptron ( size_t input_length, size_t output_length )
		: Algorithm(input_length, output_length), weights_columns(), weights_rows(), function(), type(PerceptronType::Abstract) {
		alg_type = AlgorithmType::Perceptron;
	}

}	// l4ai::algs
