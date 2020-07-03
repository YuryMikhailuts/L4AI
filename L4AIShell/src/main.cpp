#include <calculator.h>
#include <iostream>
#include <instance.h>
#include <perceptroninstance.h>
#include <flatperceptron.h>

using namespace std;
using namespace l4ai::algs;

int main() {
	cout << "Hello World!" << endl;
	cin.getline(nullptr, 0);
	FlatPerceptron fp (10, 3);
	fp.setWeightsRows(7);
	fp.setWeightsColumns(3);
	fp.setFunction(ActivationFunctions::Logistic);
	fp.setRowsOffset(2);
	fp.setUseShiftInput(true);
	std::shared_ptr<PerceptronInstF32> if32 (new PerceptronInstF32(move(fp)));
	for (size_t r = 0; r < 7; ++r) {
		for (size_t c = 0; c < 3; ++c) {
			if32->getWeight(r, c) = 1;
		}
	}

	return 0;
}
