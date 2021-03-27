/*
 * mytests.h
 *
 *  Создано российским программистом на территории Российской Федерации.
 *  (Created by a Russian programmer on the Russian Federation.)
 *
 *  Дата создания (Created): 21 июл. 2020 г.
 *  Разработчик (Developer): Михайлуц Юрий Вычеславович (aracks@yandex.ru)
 */

#ifndef TESTS_INC_MYTESTS_H_
#define TESTS_INC_MYTESTS_H_
#include <string>
#include <filesystem>
#include <iostream>
#include <list>
#include <functional>
#include <optional>

namespace my::tests {

	inline static void __assert_impl__(bool expression, std::string_view message, std::filesystem::path&& file, size_t line);
	inline static void __expect_impl__(bool expression, std::string_view message, std::filesystem::path&& file, size_t line);

	#define assert(x) __assert_impl__(x, #x, std::filesystem::path(__FILE__), (size_t)__LINE__);

	#define assertNot(x) assert(!(x))

	#define assertEqual(x,y) assert(x == y)

	#define assertAbove(x,y) assert(x > y)

	#define assertBelow(x,y) assert(x < y)

	#define assertAboveEq(x,y) assert(x >= y)

	#define assertBelowEq(x,y) assert(x <= y)

	#define expect(x) __expect_impl__(x, #x, std::filesystem::path(__FILE__), (size_t)__LINE__);

	#define expectNot(x) expect(!(x))

	#define expectEqual(x,y) expect(x == y)

	#define expectAbove(x,y) expect(x > y)

	#define expectBelow(x,y) expect(x < y)

	#define expectAboveEq(x,y) expect(x >= y)

	#define expectBelowEq(x,y) expect(x <= y)

	std::list<std::string> splitGroups(std::string_view group);

	struct TestBase {
		using test_function_t = void();
		using test_function_ptr_t = test_function_t*;
		std::list<std::string> group;
		std::string name;
		test_function_ptr_t test_function;
		TestBase(std::list<std::string>&& group, std::string_view name, test_function_ptr_t test_function = nullptr);
		TestBase(std::string_view name, test_function_ptr_t test_function = nullptr);
		TestBase(TestBase&& other) = delete;
		TestBase(const TestBase& other) = delete;
	};

	class TestSet {
	public:
		static std::ostream* stream_out;
		static std::ostream* stream_err;
	private:
		static TestSet* currentTestSet;
		std::list<TestBase*> tests;
		std::list<std::string> current_group_name;
		TestSet();
		friend TestBase;
		friend struct group_name_setter;
		friend struct group_name_clear;
		friend void __expect_impl__(bool expression, std::string_view message, std::filesystem::path&& file, size_t line);
	public:
		static TestSet& current();
		inline static std::list<std::string> get_current_group_name() {
			return std::list<std::string>(current().current_group_name);
		}
		static int run(std::optional<std::string_view> group, std::optional<std::string_view> name_pattern);
		static int run(std::optional<std::string_view> name_pattern);
		static int run();
	};


	inline static void __assert_impl__(bool expression, std::string_view message, std::filesystem::path&& file, size_t line) {
		if (!expression) throw new std::runtime_error(file.generic_string() + ":" + std::to_string(line) + ": " + std::string(message));
	}

	inline static void __expect_impl__(bool expression, std::string_view message, std::filesystem::path&& file, size_t line) {
		if (!expression) {
			(*TestSet::stream_err) << file << ":" << line << ": " << message << std::endl;
			(*TestSet::stream_err).flush();
		}
	}

	struct group_name_setter {
		group_name_setter(std::string_view group_name) {
			std::list<std::string> added_list = splitGroups(group_name);
			for(std::string& added_name : added_list) {
				TestSet::current().current_group_name.push_back(added_name);
			}
		}
	};

	struct group_name_clear {
		group_name_clear(size_t count) {
			if (count == 0) {
				TestSet::current().current_group_name.clear();
			} else {
				for(size_t i = 0; i < count; ++i) {
					TestSet::current().current_group_name.erase(--TestSet::current().current_group_name.end());
				}
			}
		}
	};

#define ccat_impl(x, y) x##y

#define ccat(x, y) ccat_impl(x,y)

#define open_group(grp_nm) namespace { group_name_setter ccat(set_group_name,__LINE__) {ccat(u8,#grp_nm)}; }

#define close_groups() namespace { group_name_clear ccat(clear_group_name,__LINE__) {0}; }

#define close_group(grpcc) namespace { group_name_clear ccat(clear_group_name,__LINE__) {grpcc}; }

    inline static auto &inf = (*my::tests::TestSet::stream_out);

    inline static auto &err = (*my::tests::TestSet::stream_err);

#define newTest(nm)    void ccat(testFunction, __LINE__)(); \
TestBase ccat(testInstance, __LINE__) { TestSet::get_current_group_name(), ccat(u8, #nm), ccat(testFunction, __LINE__) }; \
void ccat(testFunction, __LINE__)()


}	// my::tests


#endif /* TESTS_INC_MYTESTS_H_ */








