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

TEST("Test passing grades")
{
	bool result = is_passing_grade(0);
	CONFIRM_FALSE(result);

	result = is_passing_grade(100);
	CONFIRM_TRUE(result);
}
