/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Cr�ation de la classe Perceptron
 *****************************************************************************************************/
#include "main.h"
#include "couche.h"
#ifndef PERCEPTRON_H
#define PERCEPTRON_H

class perceptron {
public:
	perceptron(int aNbCouchesC,  int aNbCellsPCouches , int aNbEntres, int aNbSorties )
	{
	     NbCouchesC      = aNbCouchesC;
		 NbCellsPCouches = aNbCellsPCouches;
		 NbEntres        = aNbEntres;
		 NbSorties       = aNbSorties;

	}

	int NbCouchesC      = 0;
	int NbCellsPCouches = 0;
	int NbEntres        = 0;
	int NbSorties       = 0;

	std::vector<couche> vCouches;
	std::vector<float>  vEntres;

	void Build(void);

};



#endif