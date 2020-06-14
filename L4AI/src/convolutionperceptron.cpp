#include "convolutionperceptron.h"
#include <math.h>
#include <string.h>

namespace l4ai::algs {
	using namespace std;

	size_t ConvolutionPerceptron::getDimension() const {
		return dimension;
	}

	size_t ConvolutionPerceptron::getStripsLength() const {
		return dimension - 1;
	}

	void ConvolutionPerceptron::setDimension ( size_t value ) {
		size_t new_strips_length = value - 1;
		size_t* new_input_strips = new_strips_length == 0 ? nullptr : new size_t[new_strips_length] {};
		size_t* new_window_strips = new_strips_length == 0 ? nullptr : new size_t[new_strips_length] {};
		if ( size_t to_move = min ( new_strips_length, dimension - 1 ); to_move > 0 ) {
			memcpy ( new_input_strips, &input_strips[0], to_move * sizeof ( size_t ) );
			memcpy ( new_window_strips, &window_strips[0], to_move * sizeof ( size_t ) );
		}
		input_strips.reset ( new_input_strips );
		window_strips.reset ( new_window_strips );
		dimension = value;
	}

	ConvolutionPerceptron::ConvolutionPerceptron()
		: Perceptron ( ),
		  dimension ( 0 ),
		  input_strips ( nullptr ),
		  window_strips ( nullptr ) { Perceptron::type = PerceptronType::Convolution; }

	ConvolutionPerceptron::ConvolutionPerceptron ( size_t input_length, size_t output_length, size_t dimension )
		: Perceptron ( input_length, output_length ),
		  dimension ( dimension ),
		  input_strips ( new size_t[dimension - 1] ),
		  window_strips ( new size_t[dimension - 1] ) { Perceptron::type = PerceptronType::Convolution; }

	size_t& ConvolutionPerceptron::getInputStrip ( size_t index ) {
		return input_strips[index];
	}

	const size_t& ConvolutionPerceptron::getInputStrip ( size_t index ) const {
		return input_strips[index];
	}

	void ConvolutionPerceptron::setInputStrip ( size_t index, const size_t& value ) {
		input_strips[index] = value;
	}

	void ConvolutionPerceptron::setInputStrip ( size_t index, size_t&& value ) {
		input_strips[index] = value;
	}

	size_t& ConvolutionPerceptron::getWindowStrip ( size_t index ) {
		return window_strips[index];
	}

	const size_t& ConvolutionPerceptron::getWindowStrip ( size_t index ) const {
		return window_strips[index];
	}

	void ConvolutionPerceptron::setWindowStrip ( size_t index, const size_t& value ) {
		window_strips[index] = value;
	}

	void ConvolutionPerceptron::setWindowStrip ( size_t index, size_t&& value ) {
		window_strips[index] = value;
	}

}	// l4ai::algs
