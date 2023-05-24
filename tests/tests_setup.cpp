/***************************************************************************************
    File: setup.cpp
    Author: Navid Dezashibi
    Initial Creation Date: 2023-05-23
    Contact: navid@dezashibi.com
    Website: https://dezashibi.com
    License:
    Please refer to the LICENSE file, repository or website for more information about
    the licensing of this work. If you have any questions or concerns,
    please feel free to contact me at the email address provided above.
 ***************************************************************************************   
    Setup, teardown, ...
 **************************************************************************************/

#include "../test.h"

#include <string_view>

std::string create_test_table()
{
	// If this was real code, it might open a
	// connection to a database, create a temp
	// table with a random name, and return the
	// table name.
	return "test_data_01";
}

void drop_test_table(std::string_view /*name*/)
{
	// Real code would use the name to drop
	// the table.
}

int create_test_entry()
{
	// If this was real code, it might open a
	// connection to a database, insert a row
	// of data, and return the row identifier.
	return 100;
}

void update_test_entry_name(int /*id*/, std::string_view name)
{
	if (name.empty())
	{
		throw 1;
	}
	// Real code would proceed to update the
	// data with the new name.
}

void delete_test_entry(int /*id*/)
{
	// Real code would use the id to delete
	// the temporary row of data.
}

class TempTable
{
public:
	void setup()
	{
		m_name = create_test_table();
	}

	void teardown() const
	{
		drop_test_table(m_name);
	}

	std::string table_name() const
	{
		return m_name;
	}

private:
	std::string m_name;
};

class TempEntry
{
public:
	void setup()
	{
		m_id = create_test_entry();
	}

	void teardown()
	{
		delete_test_entry(m_id);
	}

	int id() const
	{
		return m_id;
	}

private:
	int m_id;
};


TEST_EX("Test will run setup and teardown code functional", int)
{
	int id = create_test_entry();

	// If this was a project test, it might be called
	// "Updating empty name throws". and the type thrown
	// would not be an int.
	update_test_entry_name(id, "");

	delete_test_entry(id);
}

TEST_EX("Test will run setup and teardown code object", int)
{
	MereTDD::SetupAndTeardown<TempEntry> entry;

	// If this was a project test, it might be called
	// "Updating empty name throws". And the type thrown
	// would not be an int.
	update_test_entry_name(entry.id(), "");
}

TEST("Test will run multiple setup and teardown code")
{
	MereTDD::SetupAndTeardown<TempEntry> entry1;
	MereTDD::SetupAndTeardown<TempEntry> entry2;

	// If this was a project test, it might need
	// more than one temporary entry. The TempEntry
	// policy could either create multiple data records
	// or it is easier to just have multiple instances
	// that each create a single data entry.

	update_test_entry_name(entry1.id(), "abc");
	update_test_entry_name(entry2.id(), "def");
}

MereTDD::TestSuiteSetupAndTeardown<TempTable> g_table1("Test suite setup/teardown 1", "Suite 1");

MereTDD::TestSuiteSetupAndTeardown<TempTable> g_table2("Test suite setup/teardown 2", "Suite 1");

TEST_SUITE("Test part 1 of suite", "Suite 1")
{
	// If this was a project test, it could use
	// the table names from g_table1 and g_table2
	CONFIRM("test_data_01", g_table1.table_name());
	CONFIRM("test_data_01", g_table2.table_name());
}

TEST_SUITE_EX("Test part 2 of suite", "Suite 1", int)
{
	// If this was a project test, it could use
	// the table names from g_table1 and g_table2
	throw 1;
}




