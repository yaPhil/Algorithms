#include"test_suffprefLCS.h"

std::string getStr(const size_t MAX_SIZE, const size_t DIFF)
{
	std::string ans;
	auto size = rand() % MAX_SIZE + 1;
	for (auto i = 0; i < size; ++i)
		ans += ('a' + rand() % DIFF);
	return ans;
}

void test(const size_t NUM_OF_TESTS, const size_t MAX_SIZE, const size_t DIFF)
{
	for (size_t t = 0; t < NUM_OF_TESTS; ++t)
	{
		std::cout << "Test #" << t << ": " << std::endl;
		std::cout << "	Checking..." << std::endl;
		std::string str1 = getStr(MAX_SIZE, DIFF);
		std::string str2 = getStr(MAX_SIZE, DIFF);
		auto table = calculateSuffixPrefixLCS(str1, str2);
		auto expected = slowSuffPrefLCS(str1, str2);
		for (size_t i = 0; i < table.size(); ++i)
		{
			for (size_t j = 0; j < table[i].size(); ++j)
			{
				if (table[i][j] != expected[i][j])
				{

					std::cout << "	String1 = " << str1 << std::endl << "	String2 = " << str2 << std::endl;
					std::cout << "	Error: LCS[" << i << "][" << j << "] is incorrect (" << table[i][j] << " instead of " << expected[i][j] << ")" << std::endl;
					std::cout << "	Substrings are : " << str1.substr(i) << std::endl << "	                 " << str2.substr(0, j + 1) << std::endl;
					return;
				}
			}
		}
		std::cout << "	Alright!!!" << std::endl;
	}
}