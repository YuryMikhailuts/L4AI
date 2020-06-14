#include <iostream>
#include <instance.h>
#include <perceptroninstance.h>
#include <flatperceptron.h>


using namespace std;
using namespace l4ai::algs;

int main() {
	cout << "Hello World!" << endl;
	cin.getline(nullptr, 0);
	FlatPerceptron fp (10, 20);
	fp.setWeightsColumns(10);
	fp.setWeightsRows(20);
	fp.setFunction(ActivationFunctions::Heaviside);
	InstanceF32* if32 = new PerceptronInstF32(move(fp));
	if32->getAlgorithm().getInputLength();
	return 0;
}
