#include "algorithm.h"

namespace l4ai::algs {

	size_t Algorithm::getInputLength() const {
		return input_length;
	}

	void Algorithm::setInputLength ( const size_t& value ) {
		input_length = value;
	}

	void Algorithm::setInputLength ( size_t&& value ) {
		input_length = value;
	}

	size_t Algorithm::getOutputLength() const {
		return output_length;
	}

	void Algorithm::setOutputLength ( const size_t& value ) {
		output_length = value;
	}

	void Algorithm::setOutputLength ( size_t&& value ) {
		output_length = value;
	}

	Algorithm::Algorithm() : input_length(), output_length(), alg_type(AlgorithmType::Unknown) {}

	Algorithm::Algorithm ( size_t input_length, size_t output_length )
		: input_length(input_length), output_length(output_length), alg_type(AlgorithmType::Unknown) {}

} // l4ai
