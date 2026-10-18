#include "TestUtil.h"
#include <cstring>
#include <cstdint>
#include <vector>
#include "doctest.h"
#include "WavParser.h"

//WAVファイルの中身（バイト列）を組み立てて、WavParser::Parseが読み取る場所と、失敗したときの原因を確かめる
//失敗の原因の文字列は日本語（Shift-JIS）。テストのソースも同じ文字コードで読むので、そのまま比べられる
namespace
{
	using Bytes = std::vector<uint8_t>;

	void AddText(Bytes& bytes, const char* text)
	{
		bytes.insert(bytes.end(), text, text + 4);
	}

	void AddU16(Bytes& bytes, uint16_t value)
	{
		bytes.push_back((uint8_t)(value & 0xFF));
		bytes.push_back((uint8_t)(value >> 8));
	}

	void AddU32(Bytes& bytes, uint32_t value)
	{
		for (int i = 0; i < 4; i++)	bytes.push_back((uint8_t)((value >> (8 * i)) & 0xFF));
	}

	//RIFFヘッダ（サイズは読み取りに使われないので0でよい）
	Bytes Header()
	{
		Bytes bytes;
		AddText(bytes, "RIFF");
		AddU32(bytes, 0);
		AddText(bytes, "WAVE");
		return bytes;
	}

	//チャンクを1つ足す。sizeを省略すると、中身の大きさになる（奇数なら、詰め物を1バイト付ける）
	void AddChunk(Bytes& bytes, const char* id, const Bytes& body, int64_t declaredSize = -1, bool isPadded = true)
	{
		AddText(bytes, id);
		AddU32(bytes, declaredSize >= 0 ? (uint32_t)declaredSize : (uint32_t)body.size());
		bytes.insert(bytes.end(), body.begin(), body.end());
		if (isPadded && (body.size() & 1))	bytes.push_back(0);
	}

	//PCMの標準的なfmtチャンクの中身（16バイト）
	Bytes PcmFormat(uint16_t channels = 2, uint32_t rate = 44100, uint16_t bits = 16)
	{
		Bytes body;
		AddU16(body, 1);									//WAVE_FORMAT_PCM
		AddU16(body, channels);
		AddU32(body, rate);
		AddU32(body, rate * channels * bits / 8);			//1秒あたりのバイト数
		AddU16(body, (uint16_t)(channels * bits / 8));		//ブロックサイズ
		AddU16(body, bits);
		return body;
	}

	Bytes Samples(size_t count)
	{
		Bytes body;
		for (size_t i = 0; i < count; i++)	body.push_back((uint8_t)(i * 7 + 1));
		return body;
	}

	bool Parse(const Bytes& bytes, WavParser::WavInfo* pInfo, std::string* pReason)
	{
		return WavParser::Parse(bytes.empty() ? nullptr : bytes.data(), bytes.size(), pInfo, pReason);
	}
}

TEST_CASE("WavParser: finds the format and the position of the wave data in a plain PCM file")
{
	Bytes file = Header();
	AddChunk(file, "fmt ", PcmFormat(2, 44100, 16));
	size_t expectedOffset = file.size() + 8;
	AddChunk(file, "data", Samples(100));

	WavParser::WavInfo info = {};
	std::string reason;
	REQUIRE(Parse(file, &info, &reason));
	CHECK(reason.empty());
	CHECK(info.format.Format.wFormatTag == 1);
	CHECK(info.format.Format.nChannels == 2);
	CHECK(info.format.Format.nSamplesPerSec == 44100);
	CHECK(info.format.Format.wBitsPerSample == 16);
	CHECK(info.dataOffset == expectedOffset);
	CHECK(info.dataSize == 100);

	//そこから読むと、組み立てたデータと同じ
	Bytes samples = Samples(100);
	CHECK(memcmp(file.data() + info.dataOffset, samples.data(), 100) == 0);
}

TEST_CASE("WavParser: chunks other than fmt and data are skipped, and an odd size is followed by one padding byte")
{
	Bytes file = Header();
	AddChunk(file, "LIST", Samples(7));		//奇数の大きさ（詰め物が1バイト付く）
	AddChunk(file, "fmt ", PcmFormat(1, 22050, 8));
	AddChunk(file, "fact", Samples(4));
	size_t expectedOffset = file.size() + 8;
	AddChunk(file, "data", Samples(33));

	WavParser::WavInfo info = {};
	REQUIRE(Parse(file, &info, nullptr));
	CHECK(info.format.Format.nChannels == 1);
	CHECK(info.format.Format.nSamplesPerSec == 22050);
	CHECK(info.dataOffset == expectedOffset);
	CHECK(info.dataSize == 33);
}

TEST_CASE("WavParser: a longer fmt chunk is read up to the size of WAVEFORMATEXTENSIBLE, and the rest is skipped")
{
	//WAVE_FORMAT_EXTENSIBLE（fmtが40バイト）
	Bytes extensible = PcmFormat(2, 48000, 16);
	extensible[0] = 0xFE;
	extensible[1] = 0xFF;							//WAVE_FORMAT_EXTENSIBLE
	AddU16(extensible, 22);							//cbSize
	AddU16(extensible, 16);							//有効なビット数
	AddU32(extensible, 3);							//チャンネルの配置
	for (int i = 0; i < 16; i++)	extensible.push_back((uint8_t)(0xA0 + i));	//SubFormat
	REQUIRE(extensible.size() == 40);

	Bytes file = Header();
	AddChunk(file, "fmt ", extensible);
	size_t expectedOffset = file.size() + 8;
	AddChunk(file, "data", Samples(8));

	WavParser::WavInfo info = {};
	REQUIRE(Parse(file, &info, nullptr));
	CHECK(info.format.Format.wFormatTag == 0xFFFE);
	CHECK(info.format.Format.nSamplesPerSec == 48000);
	CHECK(info.format.Format.cbSize == 22);
	CHECK(info.format.dwChannelMask == 3);
	CHECK(info.format.SubFormat.Data1 == 0xA3A2A1A0);
	CHECK(info.dataOffset == expectedOffset);

	//fmtが40バイトより長い場合も、先頭の40バイトだけを使い、残りは読み飛ばす
	Bytes longer = extensible;
	for (int i = 0; i < 10; i++)	longer.push_back(0xEE);
	Bytes file2 = Header();
	AddChunk(file2, "fmt ", longer);
	size_t expectedOffset2 = file2.size() + 8;
	AddChunk(file2, "data", Samples(8));

	WavParser::WavInfo info2 = {};
	REQUIRE(Parse(file2, &info2, nullptr));
	CHECK(info2.format.dwChannelMask == 3);
	CHECK(info2.dataOffset == expectedOffset2);
}

TEST_CASE("WavParser: an 18 byte fmt chunk leaves the rest of the format zero")
{
	Bytes fmt18 = PcmFormat();
	AddU16(fmt18, 0);		//cbSize

	Bytes file = Header();
	AddChunk(file, "fmt ", fmt18);
	AddChunk(file, "data", Samples(4));

	WavParser::WavInfo info = {};
	REQUIRE(Parse(file, &info, nullptr));
	CHECK(info.format.Format.cbSize == 0);
	CHECK(info.format.dwChannelMask == 0);
}

TEST_CASE("WavParser: the data chunk may be empty")
{
	Bytes file = Header();
	AddChunk(file, "fmt ", PcmFormat());
	AddChunk(file, "data", Bytes());

	WavParser::WavInfo info = {};
	REQUIRE(Parse(file, &info, nullptr));
	CHECK(info.dataSize == 0);
	CHECK(info.dataOffset == file.size());
}

TEST_CASE("WavParser: chunks after the data chunk are ignored")
{
	Bytes file = Header();
	AddChunk(file, "fmt ", PcmFormat());
	AddChunk(file, "data", Samples(10));
	AddChunk(file, "LIST", Samples(5));
	file.push_back(0xFF);		//半端なごみが付いていても読める

	WavParser::WavInfo info = {};
	REQUIRE(Parse(file, &info, nullptr));
	CHECK(info.dataSize == 10);
}

TEST_CASE("WavParser: files that are not WAV are rejected")
{
	WavParser::WavInfo info = {};
	std::string reason;

	CHECK_FALSE(WavParser::Parse(nullptr, 0, &info, &reason));
	CHECK(reason == "WAVEファイルではありません");

	Bytes tooShort = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V' };
	CHECK_FALSE(Parse(tooShort, &info, &reason));
	CHECK(reason == "WAVEファイルではありません");

	Bytes notRiff = Header();
	notRiff[0] = 'X';
	CHECK_FALSE(Parse(notRiff, &info, &reason));
	CHECK(reason == "WAVEファイルではありません");

	Bytes notWave = Header();
	notWave[8] = 'A';		//"AAVE"
	CHECK_FALSE(Parse(notWave, &info, &reason));
	CHECK(reason == "WAVEファイルではありません");
}

TEST_CASE("WavParser: a chunk that does not fit in the rest of the file is reported as cut off")
{
	WavParser::WavInfo info = {};
	std::string reason;

	//dataチャンクの宣言が、実際より大きい
	Bytes cutData = Header();
	AddChunk(cutData, "fmt ", PcmFormat());
	AddChunk(cutData, "data", Samples(10), 100);
	CHECK_FALSE(Parse(cutData, &info, &reason));
	CHECK(reason == "ファイルが途中で切れています");

	//fmtチャンクが途中で切れている
	Bytes cutFormat = Header();
	AddChunk(cutFormat, "fmt ", PcmFormat(), 100);
	CHECK_FALSE(Parse(cutFormat, &info, &reason));
	CHECK(reason == "ファイルが途中で切れています");

	//関係のないチャンクでも、大きさが残りに収まらなければ同じ
	Bytes cutOther = Header();
	AddChunk(cutOther, "LIST", Samples(4), 0xFFFFFFFF);
	CHECK_FALSE(Parse(cutOther, &info, &reason));
	CHECK(reason == "ファイルが途中で切れています");
}

TEST_CASE("WavParser: a malformed or missing fmt / data chunk is reported with its own reason")
{
	WavParser::WavInfo info = {};
	std::string reason;

	Bytes shortFormat = Header();
	AddChunk(shortFormat, "fmt ", Samples(15));		//16バイトに満たない
	CHECK_FALSE(Parse(shortFormat, &info, &reason));
	CHECK(reason == "fmtチャンクが不正です");

	Bytes dataFirst = Header();
	AddChunk(dataFirst, "data", Samples(10));
	AddChunk(dataFirst, "fmt ", PcmFormat());
	CHECK_FALSE(Parse(dataFirst, &info, &reason));
	CHECK(reason == "fmtチャンクがdataチャンクより前にありません");

	Bytes noFormat = Header();
	AddChunk(noFormat, "LIST", Samples(4));
	CHECK_FALSE(Parse(noFormat, &info, &reason));
	CHECK(reason == "fmtチャンクが見つかりません");

	Bytes noData = Header();
	AddChunk(noData, "fmt ", PcmFormat());
	CHECK_FALSE(Parse(noData, &info, &reason));
	CHECK(reason == "dataチャンクが見つかりません");

	//チャンクの見出し（8バイト）に満たない半端なものが続く場合は、ファイルの終わりとして扱う
	Bytes tail = Header();
	AddChunk(tail, "fmt ", PcmFormat());
	tail.push_back('d');
	tail.push_back('a');
	CHECK_FALSE(Parse(tail, &info, &reason));
	CHECK(reason == "dataチャンクが見つかりません");
}

TEST_CASE("WavParser: a failure leaves the result untouched, and the reason may be left out")
{
	WavParser::WavInfo info = {};
	info.dataOffset = 123;
	info.dataSize = 456;

	Bytes notWave = Header();
	notWave[0] = 'X';
	CHECK_FALSE(WavParser::Parse(notWave.data(), notWave.size(), &info));
	CHECK(info.dataOffset == 123);
	CHECK(info.dataSize == 456);

	//格納先がnullptrでもクラッシュせず、失敗として返る
	std::string reason;
	Bytes valid = Header();
	AddChunk(valid, "fmt ", PcmFormat());
	AddChunk(valid, "data", Samples(2));
	CHECK_FALSE(WavParser::Parse(valid.data(), valid.size(), nullptr, &reason));
	CHECK_FALSE(reason.empty());
}
