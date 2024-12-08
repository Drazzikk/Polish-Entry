#include "iostream"
#include "string"
#include "../include/polish_entry.h"

int main()
{
	std::string a;
	std::cin >> a;
	Translator T(a);
	std::cout << T.solution() << std::endl;

	return 0;
}