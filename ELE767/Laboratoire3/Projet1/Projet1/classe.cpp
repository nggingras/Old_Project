#include "classe.h"
#include "main.h"

void classe::Build(void)
{
	weight WeightTemplate(mTaille);

	for (int weight_index = 0; weight_index < mNbWeight; weight_index++)
	{	
		vWeight_list.push_back(WeightTemplate);
	}
}

