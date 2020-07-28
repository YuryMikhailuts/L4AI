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
#include <options.h>
#include <memory>
using namespace std;
using namespace my::tests;
using namespace l4ai;
using namespace l4ai::algs;

open_group(Перцептрон)

open_group(Тесты соответствия спецификации)

newTest(Тест порядка вычислений на матрице с уникальными элементами) {
	static constexpr size_t rows = 10;
	static constexpr size_t cols = 10;
	for(size_t useShift = 0; useShift <= 1; ++useShift) {
		for (int rowsOffset = 0; rowsOffset < rows - useShift; ++rowsOffset) {
			inf << "******** " << (useShift ? "useShift; "s : ""s) << "rowsOffset: "s << rowsOffset << "  *******"s << endl;
			const size_t in_length = rows - useShift + (cols - 1) * rowsOffset;
			const size_t out_length = cols;
			unique_ptr<float[]> out_data (new float[out_length]);
			unique_ptr<float[]> expected_out_data (new float[out_length]);
			unique_ptr<float[]> in_data (new float[in_length]);
			for (int i = 0; i < in_length; ++i) {
				in_data[i] = i;
			}
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
						percInst->getWeight(r, c) = (r * cols + c) * cols * rows;
					}
				}
				float current_offset = 0;
				for (size_t i = 0; i < out_length; ++i) {
					expected_out_data[i] = 0;
					float* weight_column = percInst->getWeightColumn(i);
					for (size_t r = 0; r < rows - useShift; ++r) {
						expected_out_data[i] += in_data[r + current_offset] * weight_column[r];
					}
					if (useShift != 0) {
						expected_out_data[i] += weight_column[rows - 1];
					}
					current_offset += specification->getRowsOffset();
				}
			}
			shared_ptr<CalculatorF32> calc (CalculatorF32::make(instance));
			assert(calc->calculate(in_data.get(), out_data.get()));
			for (size_t i = 0; i < out_length; ++i) {
				if (expected_out_data[i] != out_data[i])
					err << '#' << i << "expected: " << expected_out_data[i] << " actual: " << out_data[i] << endl;
			}
			inf << flush;
			err << flush;
		}
	}
}



close_groups()


