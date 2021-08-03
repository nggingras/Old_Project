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