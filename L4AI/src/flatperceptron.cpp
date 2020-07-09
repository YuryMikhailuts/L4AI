/*
 * flatperceptron.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include "flatperceptron.h"

namespace l4ai::algs {

	size_t FlatPerceptron::getRowsOffset() const {
		return rows_offset;
	}

	void FlatPerceptron::setRowsOffset ( const size_t& value ) {
		rows_offset = value;
	}

	void FlatPerceptron::setRowsOffset ( size_t&& value ) {
		rows_offset = value;
	}

	bool FlatPerceptron::getUseShiftInput() const {
		return use_shift_input;
	}

	void FlatPerceptron::setUseShiftInput ( bool value ) {
		use_shift_input = value;
	}

	FlatPerceptron::FlatPerceptron() : Perceptron (), rows_offset(), use_shift_input ( false )  { Perceptron::type = PerceptronType::Flat; }

	FlatPerceptron::FlatPerceptron ( size_t input_length, size_t output_length )
		: Perceptron ( input_length, output_length ), rows_offset(), use_shift_input ( false )  { Perceptron::type = PerceptronType::Flat; }

}	// l4ai::algs
