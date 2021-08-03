/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Création des couches pour la classe Perceptron
 *****************************************************************************************************/
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