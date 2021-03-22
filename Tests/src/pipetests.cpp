/*
 * pipelinetests.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 5 авг. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <mytests.h>
#include <flatperceptron.h>
#include <pipeline.h>
#include <perceptroninstance.h>
#include <pipelineinstance.h>
#include <calculator.h>
#include <options.h>
#include <trainer.h>
#include <memory>
#include <vector>
#include <fstream>
#include <math.h>
using namespace std;
using namespace std::filesystem;
using namespace my::tests;
using namespace l4ai;
using namespace l4ai::algs;


open_group(Конвейер)
open_group(Линейный конвейер)


newTest(Распознавание чисел){
	vector<Algorithm*> layers;
	layers.push_back(FlatPerceptron::make(256, 128, ActivationFunctions::FakeGauss, true));
	layers.push_back(FlatPerceptron::make(128, 128, ActivationFunctions::Trivial, false));
	layers.push_back(FlatPerceptron::make(128, 64, ActivationFunctions::HyperbolicTangent, true));
	layers.push_back(FlatPerceptron::make(64, 5, ActivationFunctions::Logistic, false));
	PipeLine* pipeline = new PipeLine(layers.size(), layers.data());


}


close_groups()
