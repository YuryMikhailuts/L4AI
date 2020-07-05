#include <calculator.h>
#include <iostream>
#include <instance.h>
#include <perceptroninstance.h>
#include <flatperceptron.h>
#include <trainer.h>
#include <math.h>

static constexpr float pi_value = 3.1415926535897932384626433832795;

using namespace std;
using namespace l4ai::algs;

using trainer_ptr_t = typename Trainer<float>::trainer_ptr_t;
using trainer_context_t = typename Trainer<float>::trainer_context_t;
using context_ptr_t = std::unique_ptr<trainer_context_t>;
using calculator_ptr_t = typename CalculatorF32::calculator_ptr_t;

std::shared_ptr<PerceptronInstF32> makeFP(size_t in_len, size_t out_len, ActivationFunctions af);
void InitWeights(std::shared_ptr<PerceptronInstF32> ifp, float min, float max);
float test_func(float arg);
float frnd(float min = -1, float max = 1);
float err(float left, float right);
float err_diff(float left, float right);
void step();
float show_error();
void show_result();

trainer_ptr_t tfp1, tfp2;
context_ptr_t cfp1, cfp2;
calculator_ptr_t calc1, calc2;

static constexpr size_t INTERNAL_LAYER_LENGTH = 30;

int main() {
	std::shared_ptr<PerceptronInstF32> ifp1 = makeFP(1, INTERNAL_LAYER_LENGTH, ActivationFunctions::Logistic);
	std::shared_ptr<PerceptronInstF32> ifp2 = makeFP(INTERNAL_LAYER_LENGTH, 1, ActivationFunctions::Trivial);
	InitWeights(ifp1, -1, 1);
	InitWeights(ifp2, -1, 1);
	calc1 = CalculatorF32::make(ifp1);
	calc2 = CalculatorF32::make(ifp2);
	tfp1 = Trainer<float>::make(ifp1);
	tfp2 = Trainer<float>::make(ifp2);
	cfp1 = move(tfp1->makeContext());
	cfp2 = move(tfp2->makeContext());
	float last_error = show_error();
	uint32_t NK = 1;
	for(size_t i = 0; i < 1000 * 1000; ++i) {
		step();
		if (i % 1000 == 0) {
			last_error = show_error();
		}
		if (i % (100000 * NK) == 0) {
//			if (NK == 1 && last_error < 550) {
//				tfp1->setTrainSpeed(0.01);
//				tfp2->setTrainSpeed(0.01);
//				NK = 10;
//			}
			cout << "Error: " << last_error << "\n";
		}
	}
	cout << "============================================\n";
	cout << "============================================\n\n";
	show_result();
	return 0;
}


void show_result() {
	cout << "arg;out;etl\n";
	float tmp[INTERNAL_LAYER_LENGTH];
	for(float arg = -pi_value; arg < pi_value; arg += 2 * pi_value / 100) {
		float out, etl = test_func(arg);
		calc1->calculate(&arg, tmp);
		calc2->calculate(tmp, &out);
		cout << arg << ";" << out << ";" << etl << "\n";
	}
}

void step() {
	float in_data = frnd(-pi_value, pi_value);
	float etalon = test_func(in_data);
	float out_data = 0;
	float internal_data[INTERNAL_LAYER_LENGTH];
	tfp1->train(*cfp1, &in_data, internal_data);
	tfp2->train(*cfp2, internal_data, &out_data);
	float error_data = err(out_data, etalon);
	float err_diff_data = err_diff(out_data, etalon);
	tfp2->fix(*cfp2, &err_diff_data, internal_data);
	tfp1->fix(*cfp1, internal_data, nullptr);
}

float show_error() {
	float tmp[INTERNAL_LAYER_LENGTH];
	uint64_t count = 0;
	float sko = 0;
	for(float arg = -pi_value; arg < pi_value; arg += 2 * pi_value / 100) {
		float out, etl = test_func(arg);
		calc1->calculate(&arg, tmp);
		calc2->calculate(tmp, &out);
		float oo = abs(out - etl) / abs(etl);
		sko += oo * oo;
		count++;
	}
	sko = sqrt(sko) / count;
	return sko;
}

float test_func(float arg) {
	return sin(arg);
}

float frnd(float min, float max) {
	static constexpr double DIVIDOR = double(int64_t(RAND_MAX));
	int64_t result = rand();
	return (result / DIVIDOR) * (max - min) + min;
}

float err(float left, float right) {
	float diff = (left - right);
	diff = diff * diff;
	return diff;
}

float err_diff(float left, float right) {
	return left - right;
}

void InitWeights(std::shared_ptr<PerceptronInstF32> ifp, float min, float max) {
	for (size_t r = 0; r < ifp->getPerceptron().getWeightsRows(); ++r) {
		for (size_t c = 0; c < ifp->getPerceptron().getWeightsColumns(); ++c) {
			ifp->getWeight(r, c) = frnd(min, max);
		}
	}
}

std::shared_ptr<PerceptronInstF32> makeFP(size_t in_len, size_t out_len, ActivationFunctions af) {
	FlatPerceptron fp1 (in_len, out_len);
	fp1.setWeightsRows(in_len + 1);
	fp1.setWeightsColumns(out_len);
	fp1.setFunction(af);
	fp1.setRowsOffset(0);
	fp1.setUseShiftInput(true);
	return std::shared_ptr<PerceptronInstF32> (new PerceptronInstF32(move(fp1)));
}















