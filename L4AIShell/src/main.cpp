#include <iostream>
#include <instance.h>
#include <perceptroninstance.h>
#include <flatperceptron.h>
#include <Calculator.h>

using namespace std;
using namespace l4ai::algs;

int main() {
	cout << "Hello World!" << endl;
	cin.getline(nullptr, 0);
	FlatPerceptron fp (10, 3);
	fp.setWeightsRows(7);
	fp.setWeightsColumns(3);
	fp.setFunction(ActivationFunctions::Heaviside);
	fp.setRowsOffset(2);
	fp.setUseShiftInput(true);
	PerceptronInstF32* if32 = new PerceptronInstF32(move(fp));
	for (size_t r = 0; r < 6; ++r) {
		for (size_t c = 0; c < 3; ++c) {
			if32->getWeight(r, c) = c + 1;
		}
	}
	for (size_t c = 0; c < 3; ++c) {
		if32->getWeight(6, c) = 0.1 * (c + 2);
	}
	auto calc = CalculatorF32::make(CalculatorF32::instance_ptr_t(if32));
	float* pin = new float[11] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	float* pout = new float[3] {};
	calc->calculate(pin, pout);
	cout << pout[0] << ", " << pout[1] << ", " << pout[2];
	return 0;
}
