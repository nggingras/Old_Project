/**************************** Laboratoire 3 ****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : 
 *****************************************************************************************************/
#include "main.h"

#ifndef WEIGHT_H
#define WEIGHT_H

class weight{
public:
	weight(int aTaille)
	{
		mTaille = aTaille;


		Build();
	}

	int mTaille = 0;



	std::vector<double>  vWeight;

	void Build(void);

};



#endif