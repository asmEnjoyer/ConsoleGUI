#include "console.hpp"
#include "progressBar.h"

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
	int progress1 = 0;
	int progress2 = 0;
	int progress3 = 0;
	int a = 2;
	Drawable *bar1 = new ProgressBar(progress1);
	Drawable *bar2 = new ProgressBar(progress2);
	Drawable *bar3 = new ProgressBar(progress3);
	Console._drawable.push_back(bar1);
	Console._drawable.push_back(bar2);
	Console._drawable.push_back(bar3);
	// Console.displayProgress(progress);
	for (int i = 0; i <= 40000000; i++)
	{
		isprime(i);
		progress1 = i / 100000;
		progress2 = i / 200000;
		progress3 = i / 400000;
	}
	// Console.shouldClose();
	while (1)
	{
	}

	return 0;
}