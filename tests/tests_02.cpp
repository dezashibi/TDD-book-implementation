/***************************************************************************************
    File: tests_02.cpp
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

// This test should fail because it throws an
// unexpected exception.
TEST("Test that throws unexpectedly can be created")
{
	set_expected_failure_reason("Unexpected exception thrown");

	throw "Unexpected";
}

// This test should fail because it does not throw
// an exception that it is expecting to be thrown
TEST_EX("Test that never throws can be created", int)
{
	set_expected_failure_reason("Expected exception type int was not thrown.");
}

// This test should fail because it throws an
// exception that does not match the expected type.
TEST_EX("Test that throws wrong type can be created", int)
{
	set_expected_failure_reason("Unexpected exception thrown.");

	throw "Wrong type";
}

// This test should throw an unexpected exception,
// but it doesn't. We need to somehow let the user
// know what happened. This will result in a missed failure.
TEST("Test that should throw unexpectedly can be created")
{
	set_expected_failure_reason("Unexpected exception thrown.");
}
