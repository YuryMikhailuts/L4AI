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

template<typename TValue, size_t DataCount>
struct sample {
	using value_t = TValue;
	inline static constexpr size_t data_count = DataCount;
	inline static constexpr size_t data_size = data_count * sizeof(value_t);
	value_t data[data_count];
};

template<typename TValue, size_t DataCount>
struct class_samples {
	using sample_t = ::sample<TValue, DataCount>;
	using value_t = typename sample_t::value_t;
	inline static constexpr size_t data_count = sample_t::data_count;
	inline static constexpr size_t data_size = sample_t::data_size;
	string name;
	vector<sample_t> data;
	class_samples(string_view name, optional<size_t> reserve = nullopt)
		: name(name), data() {
		if (reserve) {
			data.reserve(*reserve);
		}
	}
};

string memToString(double value, size_t idx = 0) {
	static const string suffix[] { " Байт"s, " КБайт"s, " МБайт"s, " ГБайт"s, " ТБайт"s };
	static const size_t suffix_length = sizeof(suffix) / sizeof(string);
	return (value < 1024 || idx >= suffix_length - 1) ? (to_string(value) + suffix[idx]) : memToString(value / 1024, idx + 1);
}

FlatPerceptron* makeFlatPerceptronSpec(size_t input_length, size_t output_length, ActivationFunctions afun, bool useShift = true, size_t rows_offset = 0) {
	FlatPerceptron* layer = new FlatPerceptron{input_length, output_length};
	layer->setWeightsRows(input_length);
	layer->setWeightsColumns(output_length);
	layer->setUseShiftInput(useShift);
	layer->setRowsOffset(rows_offset);
	layer->setFunction(afun);
	return layer;
}


newTest(Распознавание чисел){
	static const size_t train_vs_check = 10;
	static const size_t iterations_count = 1000;
	static const size_t class_repeat_count = 1;
	static const size_t img_w = 16;
	static const size_t img_h = 16;
	static constexpr size_t internal_length = 80;
	using class_samples_t = class_samples<float, img_w * img_h>;
	using sample_t = typename class_samples_t::sample_t;
	path base_path = canonical(tests_directory) / "Числа"s;
	path prepared_path = base_path / "Подготовленные";
	size_t prepared_child_count = distance(directory_iterator(prepared_path), directory_iterator());
	vector<class_samples_t> to_check;
	vector<class_samples_t> to_train;
	to_check.reserve(prepared_child_count);
	to_train.reserve(prepared_child_count);
	size_t memt = 0, memc;
	for(auto& p_class: directory_iterator(prepared_path)) {
    	size_t current_child_count = distance(directory_iterator(p_class.path()), directory_iterator());
		class_samples_t& current_train = to_train.emplace_back(p_class.path().filename().generic_string(), (current_child_count * (train_vs_check -1)) / train_vs_check + 1);
		class_samples_t& current_check = to_check.emplace_back(p_class.path().filename().generic_string(), current_child_count / train_vs_check + 1);
        for(auto& p2: directory_iterator(p_class.path())) {
        	sample_t& data = (rand() % train_vs_check == 0) ?
        			current_check.data.emplace_back() :
					current_train.data.emplace_back();
        	if (fstream fstr(p2.path(), ios_base::in); fstr.is_open()) {
        		fstr.read(reinterpret_cast<char*>(data.data), data.data_size);
        	}
        }
        memt += current_train.data.size() * sizeof(sample_t);
        memc += current_check.data.size() * sizeof(sample_t);
        inf << " * Класс \"" << current_train.name << "\"" << endl;
        inf << "\t * Образцов: для обучения " << current_train.data.size() << " для проверки " << current_check.data.size() << endl;
    }
	inf << "Образцы загружены. Памяти занято:" << endl;
	inf << "\t - Образцы для обучения: " << memToString(memt) << endl;
	inf << "\t - Образцы для проверки: " << memToString(memc) << endl;

	shared_ptr<InstanceF32> layer0 (InstanceF32::make(makeFlatPerceptronSpec(16 * 16, internal_length, ActivationFunctions::HyperbolicTangent, true, 1)));
	shared_ptr<InstanceF32> layer1 (InstanceF32::make(makeFlatPerceptronSpec(internal_length, 5, ActivationFunctions::Logistic, false, 0)));

	auto train0 = TrainerF32::make(layer0);
	train0->setTrainSpeed(0.001);
	auto train1 = TrainerF32::make(layer1);
	train1->setTrainSpeed(0.01);
	train1->onUseLastErrorValue();
	train1->onUseFloatAverageError();
	auto cntx0 = train0->makeContext();
	auto cntx1 = train1->makeContext();
	float internal_buffer[internal_length];
	float out_buffer[5];
	size_t classes_count = to_train.size();
	unique_ptr<float[]> etalons (new float[classes_count * classes_count]);
	for (size_t i = 0; i < classes_count; ++i) {
		inf << "\t - " << to_train.at(i).name << ": (";
		for (size_t j = 0; j < classes_count; ++j) {
			float cvl = (i == j) ? 1 : 0;
			etalons[i + classes_count * j] = cvl;
			inf << cvl << ((j + 1 < classes_count) ? ", " : ")");
		}
		inf << endl;
	}
	inf << "Старт обучения."s << endl;
	for (size_t i = 0; i < iterations_count; ++i) {
		size_t class_idx = rand() % classes_count;
		class_samples_t& class_samples = to_train.at(class_idx);
		size_t samples_count = class_samples.data.size();
		float* etalon = &etalons[class_idx * classes_count];
		for (size_t j = 0; j < class_repeat_count; ++j) {
			size_t sample_idx = rand() % samples_count;
			sample_t& sample = class_samples.data.at(sample_idx);
			assert(train0->train(*cntx0, sample.data, internal_buffer));
			assert(train1->train(*cntx1, internal_buffer, out_buffer, etalon));
			train1->fix(*cntx1, nullptr, internal_buffer);
			train0->fix(*cntx0, internal_buffer);
		}
		if (i % 2000 == 0) {
			inf << " #" << i << "(" << class_idx << "). train1 last err: " << train1->getLastErrorValue();
			inf << "; train1 float err: " << train1->getFloatAverageError() << endl;
		}
	}
	inf << "Обучение завершено."s << endl;


	auto calc0 = CalculatorF32::make(layer0);
	auto calc1 = CalculatorF32::make(layer1);

	float error = 0;
	size_t count = 0;
	for(size_t i = 0; i < to_check.size(); ++i) {
		class_samples_t& csch  = to_check[i];
		float* etalon = &etalons[i * classes_count];
		for(sample_t& sch : csch.data) {
			calc0->calculate(sch.data, internal_buffer);
			calc1->calculate(internal_buffer, out_buffer);
			for(size_t j = 0; j < classes_count; ++j) {
				float diff = out_buffer[j] - etalon[j];
				error += diff * diff;
			}
		}
		count += classes_count * csch.data.size();
	}
	error = sqrt(error / count);
	inf << "Проверено на " << size_t(count / classes_count) << " экземплярах не участвующих в обучении." << endl;
	inf << "СКО: " << error << endl;
	assertBelow(error, 0.5);
}


close_group(1)	// Тест с числами


close_groups()














