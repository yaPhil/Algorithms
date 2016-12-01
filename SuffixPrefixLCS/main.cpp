#include"test_suffprefLCS.h"

int main()
{
	/*auto r = calculateSuffixPrefixLCS("abcdef", "cgabdfe");
	for(auto i = 0; i < r.size(); ++i)
	{
		for(auto j = 0; j < r[i].size(); ++j)
		{
			std::cout << r[i][j] << ' ';
		}
		std::cout << std::endl;
	}*/
	test(1000, 200, 20);
	return 0;
}
