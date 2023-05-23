/***************************************************************************************
    File: test.h
    Author: Navid Dezashibi
    Initial Creation Date: 2023-05-23
    Contact: navid@dezashibi.com
    Website: https://dezashibi.com
    License:
    Please refer to the LICENSE file, repository or website for more information about
    the licensing of this work. If you have any questions or concerns,
    please feel free to contact me at the email address provided above.
 ***************************************************************************************   
    MereTDD implementations and Macros
 **************************************************************************************/
#pragma once

#include <iostream>
#include <string_view>
#include <vector>

namespace MereTDD
{
	class TestInterface
	{
	public:
		virtual ~TestInterface() = default;

		virtual void run() = 0;
	};

	inline std::vector<TestInterface*>& get_tests()
	{
		static std::vector<TestInterface*> tests;

		return tests;
	}

	inline void run_tests()
	{
		for (auto* test: get_tests()) test->run();
	}
}


#define TEST \
class Test : public MereTDD::TestInterface \
{            \
public:      \
    Test (std::string_view name)              \
        : m_name(name)                           \
        {          \
            MereTDD::get_tests().push_back(this); \
        }          \
             \
        void run() override;                     \
private:     \
    std::string m_name;                       \
    bool m_result{true};                       \
};           \
Test test("testCanBeCreated");             \
void Test::run()
