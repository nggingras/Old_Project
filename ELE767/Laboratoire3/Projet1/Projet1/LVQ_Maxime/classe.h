#ifndef CLASSE_H
#define CLASSE_H

#include "weight.h"
#include "main.h"

class classe{
 public:
	 classe(int aNbWeight, int aTaille, int aClasse_nb)
	 {
		mNbWeight  = aNbWeight;
		mTaille	   = aTaille;
		mClasse_nb = aClasse_nb;

		Build();
	 }

	 int mNbWeight;
	 int mTaille;
	 int mClasse_nb;
	 int mMinValue;
	 int mMinVal_pos;


	 std::vector<weight> vWeight_list;

	 void Build(void);

};

#endif