#include "TestUtil.h"
#include "doctest.h"
#include "Csv/CsvReader.h"

//GetStringが返す文字列は、末尾に'\0'を含んでいる（従来の挙動）ので、比べるときはc_str()を通す
namespace
{
	std::string Text(CsvReader& csv, DWORD x, DWORD y)
	{
		return std::string(csv.GetString(x, y).c_str());
	}
}

TEST_CASE("CsvReader: reads CRLF separated numbers")
{
	TempDir dir;
	dir.Write("numbers.csv", "1,2,3\r\n4,5,6\r\n");

	CsvReader csv;
	REQUIRE(csv.Load(dir.Path("numbers.csv")));
	CHECK(csv.GetHeight() == 2);
	CHECK(csv.GetWidth() == 3);
	CHECK(csv.GetValue(0, 0) == 1);
	CHECK(csv.GetValue(2, 0) == 3);
	CHECK(csv.GetValue(1, 1) == 5);
	CHECK(csv.GetValue(2, 1) == 6);
}

TEST_CASE("CsvReader: reads text fields, and text is 0 as a number")
{
	TempDir dir;
	dir.Write("text.csv", "abc,def\r\nghi,jkl\r\n");

	CsvReader csv;
	REQUIRE(csv.Load(dir.Path("text.csv")));
	CHECK(Text(csv, 0, 0) == "abc");
	CHECK(Text(csv, 1, 0) == "def");
	CHECK(Text(csv, 0, 1) == "ghi");
	CHECK(Text(csv, 1, 1) == "jkl");
	CHECK(csv.GetValue(0, 0) == 0);
}

TEST_CASE("CsvReader: reading outside the table returns an empty string / 0")
{
	TempDir dir;
	dir.Write("numbers.csv", "1,2,3\r\n4,5,6\r\n");

	CsvReader csv;
	REQUIRE(csv.Load(dir.Path("numbers.csv")));
	CHECK(csv.GetString(3, 0) == "");
	CHECK(csv.GetString(0, 2) == "");
	CHECK(csv.GetValue(9, 9) == 0);
}

TEST_CASE("CsvReader: a missing file fails, and the accessors stay safe")
{
	TempDir dir;

	CsvReader csv;
	CHECK_FALSE(csv.Load(dir.Path("missing.csv")));
	CHECK(csv.GetWidth() == 0);
	CHECK(csv.GetHeight() == 0);
	CHECK(csv.GetString(0, 0) == "");
	CHECK(csv.GetValue(0, 0) == 0);
}

TEST_CASE("CsvReader: a folder cannot be loaded as a file")
{
	TempDir dir;

	CsvReader csv;
	CHECK_FALSE(csv.Load(dir.Path()));
}

TEST_CASE("CsvReader: an empty file loads but has no content")
{
	TempDir dir;
	dir.Write("empty.csv", "");

	CsvReader csv;
	CHECK(csv.Load(dir.Path("empty.csv")));
	CHECK(csv.GetWidth() == 0);
	CHECK(csv.GetHeight() == 0);
	CHECK(csv.GetString(0, 0) == "");
}

TEST_CASE("CsvReader: a file without a trailing newline does not read out of range")
{
	TempDir dir;
	dir.Write("no-newline.csv", "1,2,3");

	CsvReader csv;
	CHECK(csv.Load(dir.Path("no-newline.csv")));
	CHECK(csv.GetString(5, 5) == "");
}
