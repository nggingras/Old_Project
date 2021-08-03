#include"perceptron.h"
#include "main.h"


void perceptron::Build(void) 
{
	int PrevLayerIndex = 0;
	couche CoucheTemplate;
	vCouches.clear();

	if ((NbCouchesC + NbCellsPCouches + NbEntres + NbSorties) == 0) //Perceptron missing core data to build
	{
		printf("ERROR perceptron is missing core data to build");
	}
	else
	{
		
		for (int i = 0; i < (NbCouchesC + 1); i++) //creation des couches
		{
			if (i == 0)// creation de la premiere couche
			{
				CoucheTemplate.PrevNbCells = NbEntres;
			}
			else
			{
				PrevLayerIndex = (i - 1);
				CoucheTemplate.PrevNbCells = vCouches[PrevLayerIndex].NbCells;
			}

			if (i == NbCouchesC) // creation de la couche de sortie ?
			{CoucheTemplate.NbCells = NbSorties;}
			else
			{CoucheTemplate.NbCells = NbCellsPCouches;}

			
			vCouches.push_back(CoucheTemplate);
			vCouches[i].Build();
		}
		
		for (int i = 0; i < NbEntres; i++)
		{
			vEntres.push_back(0);
		}
		
	}

}