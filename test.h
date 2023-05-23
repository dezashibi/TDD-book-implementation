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
#include <string>
#include <source_location>

namespace MereTDD
{
	class MissingException
	{
	public:
		explicit MissingException(std::string_view ex_type) : m_ex_type(ex_type)
		{
		}

		[[nodiscard]] std::string_view ex_type() const
		{
			return m_ex_type;
		}

	private:
		std::string m_ex_type;
	};

	class ConfirmException
	{
	public:
		ConfirmException(int line) : m_line(line)
		{
		}

		virtual ~ConfirmException() = default;

		[[nodiscard]] std::string_view reason() const
		{
			return m_reason;
		}

		[[nodiscard]] int line() const
		{
			return m_line;
		}

	protected:
		std::string m_reason;
		int m_line;
	};

	class BoolConfirmException : public ConfirmException
	{
	public:
		BoolConfirmException(bool expected, int line) : ConfirmException(line)
		{
			m_reason += "    Expected: ";
			m_reason += expected ? "true" : "false";
		}
	};

	class ActualConfirmException : public ConfirmException
	{
	public:
		ActualConfirmException(std::string_view expected, std::string_view actual, int line) :
				ConfirmException(line),
				m_expected(expected),
				m_actual(actual)
		{
			format_reason();
		}

	private:
		void format_reason()
		{
			m_reason += "    Expected: " + m_expected + '\n';
			m_reason += "    Actual  : " + m_actual;
		}

		std::string m_expected;
		std::string m_actual;
		int m_line{};
	};

	class TestBase
	{
	public:
		explicit TestBase(std::string_view name) : m_name(name)
		{
		}

		virtual ~TestBase() = default;

		virtual void run_ex()
		{
			run();
		}

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

		[[nodiscard]] std::string_view expected_reason() const
		{
			return m_expected_reason;
		}

		[[nodiscard]] int confirm_location() const
		{
			return m_confirm_location;
		}

		void set_failed(std::string_view reason, int confirm_location = -1)
		{
			m_passed = false;
			m_reason = reason;
			m_confirm_location = confirm_location;
		}

		void set_expected_failure_reason(std::string_view reason)
		{
			m_expected_reason = reason;
		}

	private:
		std::string m_name;
		bool m_passed{ true };
		std::string m_reason;
		std::string m_expected_reason;
		int m_confirm_location{ -1 };
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
		int num_missed_failed = 0;
		int num_failed = 0;

		for (auto* test: get_tests())
		{
			output << "----------------\n"
				   << test->name()
				   << std::endl;
			try
			{
				test->run_ex();
			}
			catch (ConfirmException const& ex)
			{
				test->set_failed(ex.reason(), ex.line());
			}
			catch (MissingException const& ex)
			{
				std::string message = "Expected exception type ";
				message += ex.ex_type();
				message += " was not thrown.";
				test->set_failed(message);
			}
			catch (...)
			{
				test->set_failed("Unexpected exception thrown.");
			}

			if (test->passed())
			{
				if (!test->expected_reason().empty())
				{
					// This test passed, but it was supposed
					// to have failed.
					++num_missed_failed;
					output << "Missed expected failure\n"
						   << "Test passed but was expected to fail."
						   << std::endl;
				}
				else
				{
					++num_passed;
					output << "Passed" << std::endl;
				}
			}
			else if (!test->expected_reason().empty() && test->expected_reason() == test->reason())
			{
				++num_passed;
				output << "Expected failure\n"
					   << test->reason()
					   << std::endl;
			}
			else
			{
				++num_failed;

				if (test->confirm_location() != -1)
				{
					output << "Failed confirm on line "
						   << test->confirm_location() << '\n';
				}
				else
				{
					output << "Failed\n";
				}

				output << test->reason()
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

		if (num_missed_failed != 0)
		{
			output << "Tests failures missed: " << num_missed_failed;
		}

		output << std::endl;

		return num_failed;
	}

	inline void
	confirm(bool expected, bool actual, const std::source_location location = std::source_location::current())
	{
		if (actual != expected)
		{
			throw BoolConfirmException(expected, location.line());
		}
	}

	template<typename T>
	void
	confirm(const T& expected, const T& actual, const std::source_location location = std::source_location::current())
	{
		if (actual != expected)
		{
			throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
		}
	}

	inline void confirm(std::string_view expected, std::string_view actual,
			const std::source_location location = std::source_location::current())
	{
		if (actual != expected)
		{
			throw ActualConfirmException(expected, actual, location.line());
		}
	}

	inline void confirm(const std::string& expected, const std::string& actual,
			const std::source_location location = std::source_location::current())
	{
		confirm(std::string_view(expected), std::string_view(actual));
	}

	inline void
	confirm(double expected, double actual, const std::source_location location = std::source_location::current())
	{
		if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
		{
			throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
		}
	}

	inline void confirm(long double expected, long double actual,
			const std::source_location location = std::source_location::current())
	{
		if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
		{
			throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
		}
	}

	inline void
	confirm(float expected, float actual, const std::source_location location = std::source_location::current())
	{
		if (actual < (expected - 0.0001f) || actual > (expected + 0.0001f))
		{
			throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
		}
	}
}

#define MERETDD_CLASS_FINAL(line) Test ## line
#define MERETDD_CLASS_RELAY(line) MERETDD_CLASS_FINAL(line)
#define MERETDD_CLASS MERETDD_CLASS_RELAY(__LINE__)

#define MERETDD_INSTANCE_FINAL(line) test ## line
#define MERETDD_INSTANCE_RELAY(line) MERETDD_INSTANCE_FINAL(line)
#define MERETDD_INSTANCE MERETDD_INSTANCE_RELAY(__LINE__)


#define TEST(test_name) \
namespace {\
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
};                      \
}\
MERETDD_CLASS MERETDD_INSTANCE(test_name);\
void MERETDD_CLASS::run()


#define TEST_EX(test_name, exception_type) \
namespace {\
class MERETDD_CLASS : public MereTDD::TestBase \
{                                          \
public:                                    \
    MERETDD_CLASS (std::string_view name)     \
        : TestBase(name)                         \
        {                                        \
            MereTDD::get_tests().push_back(this);\
        }                                        \
        void run_ex() override                   \
        {                                        \
            try\
            {                                       \
                run();                                 \
            }                                       \
            catch(exception_type const&)            \
            {                              \
                return;                                \
            }                              \
            throw MereTDD::MissingException(#exception_type);\
        }                                        \
        void run() override;                    \
};                                         \
}\
MERETDD_CLASS MERETDD_INSTANCE(test_name); \
void MERETDD_CLASS::run()

#define CONFIRM_FALSE(actual) \
    MereTDD::confirm(false, actual)

#define CONFIRM_TRUE(actual)  \
    MereTDD::confirm(true, actual)

#define CONFIRM(expected, actual) \
    MereTDD::confirm(expected, actual)




