/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Création des cellules pour la classe Perceptron
 *****************************************************************************************************/
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