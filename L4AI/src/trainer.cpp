/*
 * trainer.cpp
 *
 *  Created on: 18 июн. 2020 г.
 *      Author: unknown
 */

#include <trainer.hpp>

namespace l4ai::algs {

	template<>
	const float Trainer<float>::default_train_speed = 0.01f;

	template<>
	const double Trainer<double>::default_train_speed = 0.01;

	template class Trainer<float>;
	template class Trainer<double>;

} /* namespace l4ai::algs */
