/*
 * testsutils.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 7 авг. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef TESTS_INC_TESTSUTILS_H_
#define TESTS_INC_TESTSUTILS_H_
#include <inttypes.h>
#include <stddef.h>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <fstream>
#include <math.h>

namespace my::tests {

	template<typename TValue, size_t DataCount>
	struct sample {
		using value_t = TValue;
		inline static constexpr size_t data_count = DataCount;
		inline static constexpr size_t data_size = data_count * sizeof(value_t);
		value_t data[data_count];
	};

	template<typename TValue, size_t DataCount>
	struct class_samples {
		using sample_t = my::tests::sample<TValue, DataCount>;
		using value_t = typename sample_t::value_t;
		inline static constexpr size_t data_count = sample_t::data_count;
		inline static constexpr size_t data_size = sample_t::data_size;
		std::string name;
		std::vector<sample_t> data;
		class_samples(std::string_view name, std::optional<size_t> reserve = std::nullopt)
			: name(name), data() {
			if (reserve) {
				data.reserve(*reserve);
			}
		}
	};

	std::string memToString(double value, size_t idx = 0) {
		using namespace std;
		static const std::string suffix[] { " Байт"s, " КБайт"s, " МБайт"s, " ГБайт"s, " ТБайт"s };
		static const size_t suffix_length = sizeof(suffix) / sizeof(string);
		return (value < 1024 || idx >= suffix_length - 1) ? (to_string(value) + suffix[idx]) : memToString(value / 1024, idx + 1);
	}


	template<typename TValue, size_t DataCount>
	struct sample_loader {
		using class_samples_t = my::tests::class_samples<TValue, DataCount>;
		using sample_t = my::tests::sample<TValue, DataCount>;
		std::vector<class_samples_t> to_check;
		std::vector<class_samples_t> to_train;
		std::filesystem::directory_iterator class_itr;
		size_t train_vs_check;
		std::ostream& ostr;


		sample_loader() : train_vs_check(10) {}

		void loadTestsData() {
			using namespace std;
			using namespace std::filesystem;
			size_t memt = 0, memc;
			for(auto& p_class: class_itr) {
				size_t current_child_count = distance(directory_iterator(p_class.path()), directory_iterator());
				class_samples_t& current_train = to_train.emplace_back(p_class.path().filename().generic_string(), (current_child_count * (train_vs_check -1)) / train_vs_check + 1);
				class_samples_t& current_check = to_check.emplace_back(p_class.path().filename().generic_string(), current_child_count / train_vs_check + 1);
				for(auto& p2: directory_iterator(p_class.path())) {
					sample_t& data = (std::rand() % train_vs_check == 0) ?
							current_check.data.emplace_back() :
							current_train.data.emplace_back();
					if (fstream fstr(p2.path(), ios_base::in); fstr.is_open()) {
						fstr.read(reinterpret_cast<char*>(data.data), data.data_size);
					}
				}
				memt += current_train.data.size() * sizeof(sample_t);
				memc += current_check.data.size() * sizeof(sample_t);
				ostr << " * Класс \"" << current_train.name << "\"" << endl;
				ostr << "\t * Образцов: для обучения " << current_train.data.size() << " для проверки " << current_check.data.size() << endl;
			}
			ostr << "Образцы загружены. Памяти занято:" << endl;
			ostr << "\t - Образцы для обучения: " << memToString(memt) << endl;
			ostr << "\t - Образцы для проверки: " << memToString(memc) << endl;
		}

	};

}

#endif /* TESTS_INC_TESTSUTILS_H_ */
