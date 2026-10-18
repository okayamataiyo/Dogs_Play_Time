#include "TestUtil.h"
#include "doctest.h"
#include "Json/JsonReader.h"

TEST_CASE("JsonReader: loads a valid file and returns its sections")
{
	TempDir dir;
	dir.Write("valid.json", "{\"a\":{\"b\":1}}");

	REQUIRE(JsonReader::Load(dir.Path("valid.json")));
	CHECK(JsonReader::GetSection("a")["b"].get<int>() == 1);
}

TEST_CASE("JsonReader: a failed Load returns false, throws nothing and keeps the loaded data")
{
	TempDir dir;
	dir.Write("valid.json", "{\"a\":{\"b\":1}}");
	dir.Write("invalid.json", "{\"a\":");
	dir.Write("empty.json", "{}");

	REQUIRE(JsonReader::Load(dir.Path("valid.json")));
	CHECK_FALSE(JsonReader::Load(dir.Path("missing.json")));
	CHECK_FALSE(JsonReader::Load(dir.Path("invalid.json")));
	CHECK_FALSE(JsonReader::Load(dir.Path("empty.json")));		//íÜêgÇÃÇ»Ç¢JSONÇÕé∏îsÇ∆ÇµÇƒàµÇ§
	CHECK(JsonReader::GetSection("a")["b"].get<int>() == 1);
}

TEST_CASE("JsonReader: Save writes a file that Load reads back")
{
	TempDir dir;
	nlohmann::json json;
	json["x"] = 7;

	CHECK_FALSE(JsonReader::Save(dir.Path("no-such-dir\\out.json"), json));
	REQUIRE(JsonReader::Save(dir.Path("saved.json"), json));
	REQUIRE(JsonReader::Load(dir.Path("saved.json")));
	CHECK(JsonReader::GetSection("x").get<int>() == 7);
}
