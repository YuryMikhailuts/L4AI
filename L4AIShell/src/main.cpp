/*
 * main.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 9 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <calculator.h>
#include <iostream>
#include <instance.h>
#include <perceptroninstance.h>
#include <flatperceptron.h>
#include <trainer.h>
#include <pipeline.h>
#include <math.h>
#include <cmath>

static constexpr float pi_value = 3.1415926535897932384626433832795;

using namespace std;
using namespace l4ai::algs;

using trainer_ptr_t = typename Trainer<float>::trainer_ptr_t;
using trainer_context_t = typename Trainer<float>::trainer_context_t;
using context_ptr_t = std::unique_ptr<trainer_context_t>;
using calculator_ptr_t = typename CalculatorF32::calculator_ptr_t;


Algorithm* makeFP(size_t in_len, size_t out_len, ActivationFunctions af);
Algorithm* makePL(initializer_list<Algorithm*> layers);
float test_func(float arg);
float frnd(float min = -1, float max = 1);
float err(float left, float right);
float err_diff(float left, float right);
void step();
float show_error();
void show_result();

trainer_ptr_t trainer;
context_ptr_t context;
calculator_ptr_t calculator;

static constexpr size_t INTERNAL_LAYER_LENGTH = 20;

#include <time.h>

int main() {
	std::shared_ptr<InstanceF32> instance (InstanceF32::make(
		makePL({
			makeFP(1, INTERNAL_LAYER_LENGTH, ActivationFunctions::ArcTangent),
			makeFP(INTERNAL_LAYER_LENGTH, 1, ActivationFunctions::Trivial)
		})
	));
	calculator = CalculatorF32::make(instance);
	trainer = Trainer<float>::make(instance);
	context = move(trainer->makeContext());

	float last_error = show_error();
	uint32_t NK = 1;
	for(size_t i = 0; i < 100000; ++i) {
		step();
		if (i % 1000 == 0) {
			last_error = show_error();
		}
		if (i % (1000 * NK) == 0) {
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
	for(float arg = -pi_value; arg < pi_value; arg += 2 * pi_value / 100) {
		float out, etl = test_func(arg);
		calculator->calculate(&arg, &out);
		cout << arg << ";" << out << ";" << etl << "\n";
	}
}

void step() {
	float in_data = frnd(-pi_value, pi_value);
	float etalon = test_func(in_data);
	float out_data = 0;
	float internal_data[INTERNAL_LAYER_LENGTH];
	trainer->train(*context, &in_data, &out_data, &etalon);
	trainer->fix(*context);
}

float show_error() {
	uint64_t count = 0;
	float sko = 0;
	for(float arg = -pi_value; arg < pi_value; arg += 2 * pi_value / 100) {
		float out, etl = test_func(arg);
		calculator->calculate(&arg, &out);
		float oo = (out - etl);
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

Algorithm* makeFP(size_t in_len, size_t out_len, ActivationFunctions af) {
	FlatPerceptron* fp1 = new FlatPerceptron(in_len, out_len);
	fp1->setWeightsRows(in_len + 1);
	fp1->setWeightsColumns(out_len);
	fp1->setFunction(af);
	fp1->setRowsOffset(0);
	fp1->setUseShiftInput(true);
	return fp1;
}

Algorithm* makePL(initializer_list<Algorithm*> layers) {
	PipeLine* pl1 = new PipeLine(layers.size(), const_cast<Algorithm**const>(layers.begin()));
	return pl1;
}













