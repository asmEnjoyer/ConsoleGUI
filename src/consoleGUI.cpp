#include "console.hpp"
#include "progressBar.h"
#include "variableList.h"

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
	int i = 0;
	int prime = 0;
	ProgressBar *bar = new ProgressBar(progress);
	VariableList *var = new VariableList();
	var->add("number", i);
	var->add("Is Odd", prime);
	Console._drawable.push_back(bar);
	Console._drawable.push_back(var);
	// Console.displayProgress(progress);
	for (i = 0; i <= 100000000; i++)
	{
		prime = i % 3;
		isprime(i);
		progress = i / 1000000;
	}
	// Console.shouldClose();
	while (1)
	{
	}

	return 0;
}