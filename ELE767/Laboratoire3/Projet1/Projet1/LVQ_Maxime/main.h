#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>
#include "string.h"

#include "classe.h"

/**************************************************** Define variables *************************************************************/
//#define NB_CLASSE 10
//#define NB_POIDS 10 
#define NB_LIGNE_FICHIER_INIT		7
#define NB_DONNEE				 1560
#define NB_LIGNE_FICHIER	     1340
#define NB_LIGNE_FICHIER_VC	      120
#define NB_LIGNE_FICHIER_TEST     780

/**************************************************** Functions **************************************************************/

double calcul_distance(std::vector<double> input, std::vector<double> weight);





#endif