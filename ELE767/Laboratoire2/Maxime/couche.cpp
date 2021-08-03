#include "main.h"
#include "couche.h"
void couche::Build(void) 
{
	cell cellTemplate;
	for (int i = 0; i < NbCells; i++) //creation des cellules
	{
		cellTemplate.NbWeights = PrevNbCells;
		//cellTemplate.Theta = ;
		
		vCells.push_back(cellTemplate);
		vCells[i].Build();

	}

}