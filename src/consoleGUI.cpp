#include "console.hpp"
#include "progressBar.hpp"
#include "variableList.hpp"
#include <vector>

std::vector<int> primes;

bool isprime(int a)
{
	for (auto p : primes)
	{
		if (p * p > a)
			break;
		if (a % p == 0)
			return false;
	}
	primes.push_back(a);
	return true;
}

int main()
{
	int progress = 0;
	int primes = 0;
	ProgressBar *bar = new ProgressBar(progress);
	VariableList *var = new VariableList();
	var->add("primes", primes);
	Console._drawable.push_back(bar);
	Console._drawable.push_back(var);
	int i = 0;
	var->add("number", i);
	for (i = 2; i <= 100000000; i++)
	{
		if (isprime(i))
			primes++;
		progress = i / 1000000;
	}
	i--;

	return 0;
}