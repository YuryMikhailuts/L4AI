/*
 * mytests.cpp
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 22 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#include <mytests.h>
#include <tuple>
#include <regex>
#include <list>
#include <time.h>

namespace my::tests {

	using namespace std;

	inline static constexpr char group_split_char = '/';

	template<typename TChar, typename TValue>
	inline static basic_ostream<TChar>& operator<<(basic_ostream<TChar>& ostr, optional<TValue> value) {
		return value ? (ostr << *value) : ostr;
	}

	template<typename TChar>
	inline static basic_ostream<TChar>& operator<<(basic_ostream<TChar>& ostr, const TestBase& test) {
		ostr << '\'';
		for(const string& grp : test.group) {
			ostr << grp << group_split_char;
		}
		ostr << "" << test.name << "'" << endl;
		return ostr;
	}

	std::ostream* TestSet::stream_out = &cout;

	std::ostream* TestSet::stream_err = &cerr;

	static inline bool operator==(const list<string>& left, const list<string>& right);

	static bool tests_compare(const TestBase* first, const TestBase* second);

	TestBase::TestBase(list<string>&& group, std::string_view name, test_function_ptr_t test_function)
		: group(move(group)), name(name), test_function(test_function) { TestSet::current().tests.push_back(this); }

	TestBase::TestBase(std::string_view name, test_function_ptr_t test_function) : TestBase(list<string>(), name, test_function) {}

	int TestSet::run(std::optional<std::string_view> group, std::optional<std::string_view> name_pattern) {
		ostream& out = *stream_out;
		out << "Формируется список тестов для запуска."s << endl;
		list<TestBase*> candidates;
		if (group) {
			list<string> group_list = splitGroups(*group);
			if (name_pattern && !name_pattern->empty()) {
				regex re {string(*name_pattern)};
				for(TestBase* test: TestSet::current().tests) {
					if (test->group == group_list) {
						if (regex_match(test->name, re)) {
							out << *test << endl;
							candidates.push_back(test);
						}
					}
				}
			} else {
				for(TestBase* test: TestSet::current().tests) {
					if (test->group == group_list) {
						out << *test << endl;
						candidates.push_back(test);
					}
				}
			}
		} else {
			if (name_pattern && !name_pattern->empty()) {
				regex re {string(*name_pattern)};
				for(TestBase* test: TestSet::current().tests) {
					if (regex_match(test->name, re)) {
						out << *test << endl;
						candidates.push_back(test);
					}
				}
			} else {
				for(TestBase* test: TestSet::current().tests) {
					out << *test << endl;
					candidates.push_back(test);
				}
			}
		}
		candidates.sort(tests_compare);
		size_t success = 0;
		size_t failed = 0;
		for(TestBase* test: candidates) {
			out << "========================================" << endl;
			out << "Старт теста " << *test << endl;
			out << "----------------------------------------" << endl;
			try {
				clock_t clk = clock();
				test->test_function();
				clk = clock() - clk;
				++success;
				out << "Тест успешно проден за "s << clk / CLOCKS_PER_SEC << " секунд."s << endl;
			} catch (std::exception& e) {
				++failed;
				err << "Тест провален с ошибкой: "s << e.what() << endl;
			} catch (...) {
				++failed;
				err << "Тест провален с неизвестной ошибкой."s << endl;
			}
		}
		out << "========================================" << endl;
		out << "=======Выполнено " << candidates.size() << " тестов из " << TestSet::current().tests.size() << " =====" << endl;
		out << "========================================" << endl;
		if (!group) {
			if (!name_pattern || name_pattern->empty() || name_pattern == ".*"s) {
				out << "Успешно пройдено "s << success << " и провалено "s << failed << " из "s << candidates.size() << " тестов."s << endl;
			} else {
				out << "Успешно пройдено "s << success << " и провалено "s << failed << " из "s << candidates.size() << " тестов отобранных по шаблону имени '"s << name_pattern << "'."s << endl;
			}
		} else {
			if (!name_pattern || name_pattern->empty() || name_pattern == ".*"s) {
				out << "Успешно пройдено " << success << " и провалено " << failed << " из " << candidates.size() << " тестов в группе ";
				for(auto i = group->begin(); i != group->end(); ++i) {
					out << *i << group_split_char;
				}
				out << endl;
			} else {
				out << "Успешно пройдено "s << success << " и провалено "s << failed << " из "s << candidates.size() << " тестов отобранных по шаблону имени '"s << name_pattern << " в группе "s;
				for(auto i = group->begin(); i != group->end(); ++i) {
					out << *i << group_split_char;
				}
				out << endl;
			}
		}
		if (failed == 0) {
			return 0;
		} else if (success == 0) {
			return -1;
		} else {
			return -2;
		}
	}

	int TestSet::run(std::optional<std::string_view> name_pattern) {
		return run(nullopt, name_pattern);
	}

	int TestSet::run() {
		return run(nullopt);
	}


	static inline bool operator==(const list<string>& left, const list<string>& right) {
		list<string>::const_iterator litr, ritr;
		for((litr = left.begin(), ritr = right.begin()); litr != left.end() && ritr != right.end(); (++litr, ++ritr))
			if (*litr != *ritr) return false;
		return litr == left.end() && ritr == right.end();
	}

	static inline bool operator<(const list<string>& left, const list<string>& right) {
		list<string>::const_iterator litr, ritr;
		for((litr = left.begin(), ritr = right.begin()); litr != left.end() && ritr != right.end(); (++litr, ++ritr))
			if (*litr > *ritr) return false;
		return left.size() < right.size();
	}

	static bool tests_compare(const TestBase* first, const TestBase* second) {
		return first->group < second->group && first->name < second->name;
	}

	list<std::string> splitGroups(string_view group) {
		list<string> result;
		string group_name;
		for(char c : group) {
			if (c != group_split_char) {
				group_name += c;
			} else {
				result.push_back(group_name);
				group_name.clear();
			}
		}
		if (!group_name.empty())
			result.push_back(group_name);
		return result;
	}

	TestSet::TestSet() : tests(), current_group_name() {}

	TestSet* TestSet::currentTestSet = nullptr;

	TestSet& TestSet::current() {
		if (currentTestSet == nullptr) {
			currentTestSet = new TestSet();
		}
		return *currentTestSet;
	}


}	// my::tests


using namespace std;
using namespace my::tests;

int main(int argc, char* argv[]) {
	switch (argc) {
		case 1:
			return TestSet::current().run();
		case 2:
			return TestSet::current().run(argv[1]);
		case 3:
		default:
			return TestSet::current().run(argv[1], argv[2]);
	}
}









