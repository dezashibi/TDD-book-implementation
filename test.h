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

#include <ostream>
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

	inline int run_tests(std::ostream& output)
	{
		output << "Running "
				  << get_tests().size()
				  << " tests\n";

		int num_passed = 0;
		int num_failed = 0;

		for (auto* test: get_tests())
		{
			output << "----------------\n"
					  << test->name()
					  << std::endl;
			try
			{
				test->run();
			}
			catch (...)
			{
				test->set_failed("Unexpected exception thrown.");
			}

			if (test->passed())
			{
				++num_passed;
				output << "Passed" << std::endl;
			}
			else
			{
				++num_failed;
				output << "Failed\n"
						  << test->reason()
						  << std::endl;
			}
		}

		output << "-----------------\n";

		if (num_failed == 0)
		{
			output << "All tests passed." << std::endl;
		}
		else
		{
			output << "Tests passed: " << num_passed
					  << "\nTests failed: " << num_failed
					  << std::endl;
		}

		return num_failed;
	}
}

#define MERETDD_CLASS_FINAL(line) Test ## line
#define MERETDD_CLASS_RELAY(line) MERETDD_CLASS_FINAL(line)
#define MERETDD_CLASS MERETDD_CLASS_RELAY(__LINE__)

#define MERETDD_INSTANCE_FINAL(line) test ## line
#define MERETDD_INSTANCE_RELAY(line) MERETDD_INSTANCE_FINAL(line)
#define MERETDD_INSTANCE MERETDD_INSTANCE_RELAY(__LINE__)


#define TEST(test_name) \
class MERETDD_CLASS : public MereTDD::TestBase \
{            \
public:      \
    MERETDD_CLASS (std::string_view name)              \
        : TestBase(name)                      \
        {          \
            MereTDD::get_tests().push_back(this); \
        }          \
\
        void run() override;                     \
};           \
MERETDD_CLASS MERETDD_INSTANCE(test_name);\
void MERETDD_CLASS::run()
