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
	class TestBase
	{
	public:
		explicit TestBase(std::string_view name) : m_name(name)
		{
		}

		virtual ~TestBase() = default;

		virtual void run() = 0;

		[[nodiscard]] std::string_view name() const
		{
			return m_name;
		}

		[[nodiscard]] bool passed() const
		{
			return m_passed;
		}

		[[nodiscard]] std::string_view reason() const
		{
			return m_reason;
		}

		void set_failed(std::string_view reason)
		{
			m_passed = false;
			m_reason = reason;
		}

	private:
		std::string m_name;
		bool m_passed{ true };
		std::string m_reason;
	};

	inline std::vector<TestBase*>& get_tests()
	{
		static std::vector<TestBase*> tests;

		return tests;
	}

	inline void run_tests()
	{
		for (auto* test: get_tests())
		{
			try
			{
				test->run();
			}
			catch (...)
			{
				test->set_failed("Unexpected exception thrown.");
			}
		}
	}
}


#define TEST \
class Test : public MereTDD::TestBase \
{            \
public:      \
    Test (std::string_view name)              \
        : TestBase(name)                      \
        {          \
            MereTDD::get_tests().push_back(this); \
        }          \
\
        void run() override;                     \
};           \
Test test("testCanBeCreated");             \
void Test::run()
