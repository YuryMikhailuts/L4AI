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
#include <vector>
#include <fstream>
using namespace std;
using namespace std::filesystem;
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

close_group(1)	// Тесты соответствия спецификации


open_group(Тесты классификации)

template<size_t width, size_t height>
struct sample {
	inline static constexpr size_t data_size = width * height;
	float data[data_size];
};

template<size_t width, size_t height>
struct class_samples {
	using sample_t = ::sample<width, height>;
	string name;
	vector<sample_t> data;
	class_samples(string_view name) : name(name), data() {}
};

string memToString(double value, size_t idx = 0) {
	static const string suffix[] { " Байт"s, " КБайт"s, " МБайт"s, " ГБайт"s, " ТБайт"s };
	static const size_t suffix_length = sizeof(suffix) / sizeof(string);
	return (value < 1024 || idx >= suffix_length - 1) ? (to_string(value) + suffix[idx]) : memToString(value / 1024, idx + 1);
}

newTest(Распознавание чисел){
	using class_samples_t = class_samples<16, 16>;
	using samples_t = class_samples_t::sample_t;
	path base_path = canonical(tests_directory) / "Числа"s;
	path prepared_path = base_path / "Подготовленные";
	vector<class_samples_t> classes;
	size_t mem = 0;
	for(auto& p_class: directory_iterator(prepared_path)) {
		class_samples_t& current = classes.emplace_back(p_class.path().filename().generic_string());
        for(auto& p2: directory_iterator(p_class.path())) {
        	samples_t& data = current.data.emplace_back();
        	if (fstream fstr(p2.path(), ios_base::in); fstr.is_open()) {
        		fstr.read(reinterpret_cast<char*>(data.data), data.data_size);
        	}
        }
        mem += current.data.size() * sizeof(samples_t);
    }
	inf << "Образцы загружены. Памяти занято: " << memToString(mem) << endl;


}


close_group(1)	// Тест с числами


close_groups()




