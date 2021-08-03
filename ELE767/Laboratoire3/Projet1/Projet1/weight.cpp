/**************************** Laboratoire 3 ****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : 
 *****************************************************************************************************/
#include "weight.h"
#include "main.h"

void weight::Build(void)
{
	for (int i = 0; i < mTaille; i++)
	{
		vWeight.push_back(0);
	}
}
