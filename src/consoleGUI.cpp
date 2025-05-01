#include "console.hpp"

bool isprime(int a)
{
	for (int i = 2; i * i < a; i++)
	{
		if (a % i == 0)
			return false;
	}
	return true;
}

int main()
{
	int progress = 0;
	int a = 2;
	Console.displayProgress(progress);
	for (int i = 0; i <= 100000000; i++)
	{
		isprime(i);
		progress = i / 1000000;
	}
	// Console.shouldClose();
	while (1)
	{
	}

	return 0;
}