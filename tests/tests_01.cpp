/***************************************************************************************
    File: tests_01.cpp
    Author: Navid Dezashibi
    Initial Creation Date: 2023-05-23
    Contact: navid@dezashibi.com
    Website: https://dezashibi.com
    License:
    Please refer to the LICENSE file, repository or website for more information about
    the licensing of this work. If you have any questions or concerns,
    please feel free to contact me at the email address provided above.
 ***************************************************************************************   
    Basic Tests
 **************************************************************************************/

#include "../test.h"

TEST("Test can be created")
{
}

TEST("Test that fails work")
{
	throw 1;
}

TEST_EX("Test with throw can be created", int)
{
	throw 1;
}

TEST_EX("Test that never throws can be created", int)
{
}
