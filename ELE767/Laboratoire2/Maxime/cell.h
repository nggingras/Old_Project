#include "main.h"
#ifndef CELL_H
#define CELL_H

class cell {
public:
	float Ival;
	float Aval;
	float theta = 0; //0 for now will need a random value later on
	float delta;
	int	  s_desire;
	int   NbWeights;
	
	void Build(void);

	std::vector<float> vWeights;
	
	std::vector<float> vDeltaWeights;
};

#endif