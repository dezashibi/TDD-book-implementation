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

#include <map>
#include <ostream>
#include <string_view>
#include <vector>
#include <string>
#include <source_location>

namespace MereTDD
{
	class Test;

	class TestSuite;

	class MissingException
	{
	public:
		explicit MissingException(std::string_view ex_type) : m_ex_type(ex_type)
		{
		}

		std::string_view ex_type() const
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

		std::string_view reason() const
		{
			return m_reason;
		}

		int line() const
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

	template<typename T>
	class SetupAndTeardown : public T
	{
	public:
		SetupAndTeardown()
		{
			T::setup();
		}

		~SetupAndTeardown()
		{
			T::teardown();
		}
	};

	inline std::map<std::string, std::vector<Test*>>& get_tests()
	{
		static std::map<std::string, std::vector<Test*>> tests;

		return tests;
	}

	inline std::map<std::string, std::vector<TestSuite*>>& get_test_suites()
	{
		static std::map<std::string, std::vector<TestSuite*>> suites;

		return suites;
	}

	inline void add_test(std::string_view suite_name, Test* test)
	{
		std::string name(suite_name);
		if (!get_tests().contains(name))
		{
			get_tests().try_emplace(name, std::vector<Test*>());
		}

		get_tests()[name].push_back(test);
	}

	inline void add_test_suite(std::string_view suite_name, TestSuite* suite)
	{
		std::string name(suite_name);
		if (!get_test_suites().contains(name))
		{
			get_test_suites().try_emplace(name, std::vector<TestSuite*>());
		}
		get_test_suites()[name].push_back(suite);
	}

	class TestBase
	{
	public:
		explicit TestBase(std::string_view name, std::string_view suite_name) :
				m_name(name), m_suite_name(suite_name)
		{
		}

		virtual ~TestBase() = default;

		virtual void run_ex()
		{
			run();
		}

		virtual void run() = 0;

		std::string_view name() const
		{
			return m_name;
		}

		std::string_view suite_name() const
		{
			return m_suite_name;
		}

		bool passed() const
		{
			return m_passed;
		}

		std::string_view reason() const
		{
			return m_reason;
		}

		std::string_view expected_reason() const
		{
			return m_expected_reason;
		}

		int confirm_location() const
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
		std::string m_suite_name;
		bool m_passed{ true };
		std::string m_reason;
		std::string m_expected_reason;
		int m_confirm_location{ -1 };
	};

	class Test : public TestBase
	{
	public:
		Test(std::string_view name, std::string_view suite_name) : TestBase(name, suite_name)
		{
			add_test(suite_name, this);
		}

		virtual void run_ex()
		{
			run();
		}

		virtual void run() = 0;

		std::string_view expected_reason() const
		{
			return m_expected_reason;
		}

		void set_expected_failure_reason(std::string_view reason)
		{
			m_expected_reason = reason;
		}

	private:
		std::string m_expected_reason;
	};

	template<typename ExceptionT>
	class TestEx : public Test
	{
	public:
		TestEx(std::string_view name, std::string_view suite_name, std::string_view exception_name) :
				Test(name, suite_name), m_exception_name(exception_name)
		{
		}

		void run_ex() override
		{
			try
			{
				run();
			}
			catch (ExceptionT const&)
			{
				return;
			}

			throw MissingException(m_exception_name);
		}

	private:
		std::string m_exception_name;
	};

	class TestSuite : public TestBase
	{
	public:
		TestSuite(std::string_view name, std::string_view suite_name) : TestBase(name, suite_name)
		{
			add_test_suite(suite_name, this);
		}

		virtual void suite_setup() = 0;

		virtual void suite_teardown() = 0;
	};

	template<typename T>
	class TestSuiteSetupAndTeardown : public T, public TestSuite
	{
	public:
		TestSuiteSetupAndTeardown(std::string_view name, std::string_view suite) : TestSuite(name, suite)
		{
		}

		void run() override
		{}

		void suite_setup() override
		{
			T::setup();
		}

		void suite_teardown() override
		{
			T::teardown();
		}
	};

	inline void run_test(std::ostream& output, Test* test, int& num_passed, int& num_failed, int& num_missed_failed)
	{
		output << "------- Test: "
			   << test->name()
			   << std::endl;

		try
		{
			test->run_ex();
		}
		catch (const ConfirmException& ex)
		{
			test->set_failed(ex.reason(), ex.line());
		}
		catch (const MissingException& ex)
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

			output << test->reason() << std::endl;
		}
	}

	inline bool run_suite(std::ostream& output, bool setup, const std::string& name, int& num_passed, int& num_failed)
	{
		for (auto& suite: get_test_suites()[name])
		{
			if (setup)
			{
				output << "------- Setup: ";
			}
			else
			{
				output << "------- Teardown: ";
			}
			output << suite->name() << std::endl;

			try
			{
				if (setup)
				{
					suite->suite_setup();
				}
				else
				{
					suite->suite_teardown();
				}
			}
			catch (const ConfirmException& ex)
			{
				suite->set_failed(ex.reason(), ex.line());
			}
			catch (...)
			{
				suite->set_failed("Unexpected exception thrown.");
			}

			if (suite->passed())
			{
				++num_passed;
				output << "Passed" << std::endl;
			}
			else
			{
				++num_failed;
				if (suite->confirm_location() != -1)
				{
					output << "Failed confirm on line "
						   << suite->confirm_location() << '\n';
				}
				else
				{
					output << "Failed\n";
				}
				output << suite->reason() << std::endl;
				return false;
			}
		}
		return true;
	}

	inline int run_tests(std::ostream& output)
	{
		output << "Running "
			   << get_tests().size()
			   << " tests\n";

		int num_passed = 0;
		int num_missed_failed = 0;
		int num_failed = 0;

		for (auto const& [key, value]: get_tests())
		{
			std::string suite_display_name = "Suite: ";
			if (key.empty())
			{
				suite_display_name += "Single Tests";
			}
			else
			{
				suite_display_name += key;
			}
			output << "---------------- " << suite_display_name << std::endl;

			if (!key.empty())
			{
				if (!get_test_suites().contains(key))
				{
					output << "Test suite is not found."
						   << " Exiting test application."
						   << std::endl;
					return ++num_failed;
				}

				if (!run_suite(output, true, key, num_passed, num_failed))
				{
					output << "Test suite setup failed."
						   << " Skipping tests in suite."
						   << std::endl;

					continue;
				}
			}

			for (auto* test: value)
			{
				run_test(output, test, num_passed, num_failed, num_missed_failed);
			}

			if (!key.empty())
			{
				if (!run_suite(output, false, key, num_passed, num_failed))
				{
					output << "Test suite teardown failed." << std::endl;
				}
			}
		}

		output << "-----------------------------------\n";

		output << "Tests passed: " << num_passed
			   << "\nTests failed: " << num_failed;

		if (num_missed_failed != 0)
		{
			output << "\nTests failures missed: " << num_missed_failed;
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
        { }          \
        void run() override;                     \
};                      \
}\
MERETDD_CLASS MERETDD_INSTANCE(test_name);\
void MERETDD_CLASS::run()


#define TEST_EX(test_name, exception_type) \
namespace {\
class MERETDD_CLASS : public MereTDD::TestExBase<exception_type> \
{                                          \
public:                                    \
    MERETDD_CLASS (std::string_view name, std::string_view exception_name)     \
        : TestExBase(name, exception_name)                         \
        { }                                        \
\
        void run() override;                    \
};                                         \
}\
MERETDD_CLASS MERETDD_INSTANCE(test_name, #exception_type); \
void MERETDD_CLASS::run()


#define TEST_SUITE(test_name, suite_name) \
namespace {                               \
class MERETDD_CLASS : public MereTDD::Test\
{                                         \
public:                                   \
    MERETDD_CLASS (std::string_view name, std::string_view suite): Test(name, suite) {} \
    void run() override;\
};\
}                                         \
MERETDD_CLASS MERETDD_INSTANCE(test_name, suite_name);                               \
void MERETDD_CLASS::run()

#define TEST_SUITE_EX(test_name, suite_name, exception_type) \
namespace {                                                  \
class MERETDD_CLASS : public MereTDD::TestEx<exception_type> \
{                                                            \
public:                                                      \
    MERETDD_CLASS (std::string_view name, std::string_view suite, std::string_view exception_name): TestEx(name, suite, exception_name) {} \
    void run() override;\
};\
}                                                            \
MERETDD_CLASS MERETDD_INSTANCE(test_name, suite_name, #exception_type);                                                              \
void MERETDD_CLASS::run()


#define CONFIRM_FALSE(actual) \
    MereTDD::confirm(false, actual)

#define CONFIRM_TRUE(actual)  \
    MereTDD::confirm(true, actual)

#define CONFIRM(expected, actual) \
    MereTDD::confirm(expected, actual)




