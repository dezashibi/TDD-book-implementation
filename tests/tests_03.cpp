/***************************************************************************************
    File: tests_03.cpp
    Author: Navid Dezashibi
    Initial Creation Date: 2023-05-23
    Contact: navid@dezashibi.com
    Website: https://dezashibi.com
    License:
    Please refer to the LICENSE file, repository or website for more information about
    the licensing of this work. If you have any questions or concerns,
    please feel free to contact me at the email address provided above.
 ***************************************************************************************   
    << OTHER DESCRIPTIONS >>
 **************************************************************************************/

#include "../test.h"

bool is_passing_grade(int value)
{
	if (value < 60) return false;

	return true;
}

bool is_negative(int value)
{
	return value < 0;
}

template<typename T>
T multiply_by_2(T value)
{
	return value * 2;
}

TEST("Test passing grades")
{
	bool result = is_passing_grade(0);
	CONFIRM_FALSE(result);

	result = is_passing_grade(100);
	CONFIRM_TRUE(result);
}

TEST("Test bool confirms")
{
	bool result = is_negative(0);
	CONFIRM_FALSE(result);

	result = is_negative(-1);
	CONFIRM_TRUE(result);
}

TEST("Test int confirms")
{
	int result = multiply_by_2(0);
	CONFIRM(0, result);

	result = multiply_by_2(1);
	CONFIRM(2, result);

	result = multiply_by_2(-1);
	CONFIRM(-2, result);
}

TEST("Test bool confirm failure")
{
	bool result = is_negative(0);
	CONFIRM_TRUE(result);
}

TEST("Test int confirm failure")
{
	int result = multiply_by_2(1);
	CONFIRM(0, result);
}

TEST("Test bool confirm failure")
{
	std::string reason = "    Expected: true";
	set_expected_failure_reason(reason);
	bool result = is_negative(0);
	CONFIRM_TRUE(result);
}

TEST("Test int confirm failure")
{
	std::string reason = "    Expected: 0\n";
	reason += "    Actual  : 2";
	set_expected_failure_reason(reason);

	int result = multiply_by_2(1);
	CONFIRM(0, result);
}

TEST("Test long confirms")
{
	long result = multiply_by_2(0L);
	CONFIRM(0L, result);
	result = multiply_by_2(1L);
	CONFIRM(2L, result);
	result = multiply_by_2(-1L);
	CONFIRM(-2L, result);
}

TEST("Test long confirm failure")
{
	std::string reason = "    Expected: 0\n";
	reason += "    Actual  : 2";
	set_expected_failure_reason(reason);

	long result = multiply_by_2(1L);
	CONFIRM(0L, result);
}

TEST("Test long long confirms")
{
	long long result = multiply_by_2(0LL);
	CONFIRM(0LL, result);

	result = multiply_by_2(10'000'000'000LL);
	CONFIRM(20'000'000'000LL, result);

	result = multiply_by_2(-10'000'000'000LL);
	CONFIRM(-20'000'000'000LL, result);
}

TEST("Test long long confirm failure")
{
	std::string reason = "    Expected: 10000000000\n";
	reason += "    Actual  : 20000000000";
	set_expected_failure_reason(reason);
	long long result = multiply_by_2(10'000'000'000LL);
	CONFIRM(10'000'000'000LL, result);
}

TEST("Test string confirms")
{
	std::string result = "abc";
	std::string expected = "abc";
	CONFIRM(expected, result);
}

TEST("Test string confirm failure")
{
	std::string reason = "    Expected: def\n";
	reason += "    Actual  : abc";
	set_expected_failure_reason(reason);
	std::string result = "abc";
	std::string expected = "def";
	CONFIRM(expected, result);
}

TEST("Test bool pointer confirms")
{
	bool result1 = true;
	bool result2 = false;
	bool* pResult1 = &result1;
	bool* pResult2 = &result2;
	CONFIRM_TRUE(*pResult1);
	CONFIRM_FALSE(*pResult2);
}

TEST("Test string and string literal confirms")
{
	std::string result = "abc";
	CONFIRM("abc", result);
}

TEST("Test float confirms")
{
	float f1 = 0.1f;
	float f2 = 0.2f;
	float sum = f1 + f2;
	float expected = 0.3f;
	CONFIRM(expected, sum);
}

TEST("Test double confirms")
{
	double d1 = 0.1;
	double d2 = 0.2;
	double sum = d1 + d2;
	double expected = 0.3;
	CONFIRM(expected, sum);
}

TEST("Test int confirms")
{
	CONFIRM(0, multiply_by_2(0));
	CONFIRM(2, multiply_by_2(1));
	CONFIRM(-2, multiply_by_2(-1));
}