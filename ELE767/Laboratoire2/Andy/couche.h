/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Création des couches pour la classe Perceptron
 *****************************************************************************************************/
#include "main.h"
#include "cell.h"
#ifndef COUCHE_H
#define COUCHE_H


class couche 
{
public:

	void Build(void);
	int NbCells;
	int PrevNbCells;
	std::vector<cell> vCells;

};

#endif