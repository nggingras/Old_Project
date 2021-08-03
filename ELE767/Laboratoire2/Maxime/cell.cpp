#include "main.h"
#include "cell.h"
void cell::Build(void)
{
	for (int i = 0; i < NbWeights; i++)
	{
		vWeights.push_back(0);
	}		

	for (int i = 0; i < NbWeights; i++)
	{
		vDeltaWeights.push_back(0);
	}
}