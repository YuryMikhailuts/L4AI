/*
 * perceptrontests.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 21 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */
#include <mytests.h>
#include <flatperceptron.h>
#include <perceptroninstance.h>
#include <calculator.h>
#include <memory>
using namespace std;
using namespace my::tests;
using namespace l4ai;
using namespace l4ai::algs;


open_group(Перцептрон)

open_group(Тесты соответствия спецификации)

newTest(Тест порядка вычислений на матрице с уникальными элементами) {
	static constexpr size_t in_length = 10;
	static constexpr size_t out_length = 10;
	for(size_t useShift = 0; useShift <= 1; ++useShift) {
		for (int rowsOffset = 0; rowsOffset < in_length; ++rowsOffset) {
			unique_ptr<float[]> out_data (new float[out_length]);
			unique_ptr<float[]> expected_out_data (new float[out_length]);
			unique_ptr<float[]> in_data (new float[in_length]);
			for (int i = 0; i < in_length; ++i) {
				in_data[i] = 10000 * i;
			}
			inf << endl << flush;
			err << endl << flush;
			size_t rows = in_length + useShift;
			size_t cols = out_length;
			rows += (cols - 1) * rowsOffset;
			FlatPerceptron* specification = new FlatPerceptron{in_length, out_length};
			specification->setWeightsColumns(cols);
			specification->setWeightsRows(rows);
			specification->setUseShiftInput(static_cast<bool>(useShift & 1));
			specification->setRowsOffset(rowsOffset);
			specification->setFunction(ActivationFunctions::Trivial);
			shared_ptr<InstanceF32> instance(move(InstanceF32::make(specification)));
			if (PerceptronInstF32* percInst = static_cast<PerceptronInstF32*>(&*instance); percInst != nullptr) {
				for (size_t r = 0; r < rows; ++r) {
					for (size_t c = 0; c < cols; ++c) {
						percInst->getWeight(r, c) = r * cols + c;
					}
				}
				float current_offset = 0;
				for (size_t i = 0; i < out_length; ++i) {
					expected_out_data[i] = 0;
					float* weight_column = percInst->getWeightColumn(i);
					for (size_t r = 0; r < in_length; ++r) {
						expected_out_data[i] += in_data[r + current_offset] * weight_column[r];
					}
					if (useShift != 0) {
						expected_out_data[i] += weight_column[in_length];
					}
					current_offset += specification->getRowsOffset();
				}
			}
			shared_ptr<CalculatorF32> calc (CalculatorF32::make(instance));
			assert(calc->calculate(in_data.get(), out_data.get()));
			for (size_t i = 0; i < out_length; ++i) {
				if (expected_out_data[i] != out_data[i])
					err << '#' << i << "expected: " << expected_out_data[i] << " actual: " << out_data[i] << endl;
				else
					inf << '#' << i << "expected: " << expected_out_data[i] << " actual: " << out_data[i] << endl;
			}
			inf << endl << flush;
			err << endl << flush;
		}
	}
	inf << "Когда-то тут, несомненно, будет тест перцептрона."s << endl;
}

close_groups()


