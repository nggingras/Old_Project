/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Lit les fichiers de données, roule l'algorithme d'apprentissage et calcul les performances.
 *****************************************************************************************************/
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
#include "perceptron.h"
#include "string.h"

/**************************************************** Define variables *************************************************************/
#define NB_LIGNE_FICHIER_INIT		8
#define WEIGHT_MIN				   -0.1
#define WEIGHT_MAX				    0.1
#define THETA_MIN					-1
#define THETA_MAX					 1
#define NB_DONNEE				 1560
#define NB_LIGNE_FICHIER	     1340
#define NB_LIGNE_FICHIER_VC	      120
#define NB_LIGNE_FICHIER_TEST     780

/**************************************************** Functions **************************************************************/
using namespace std;
std::string  supprime(std::string  texte, std::string  x);

float calcul_fonct_activation(float Ival, int factivation);
float generate_random_float(float LO, float HI);

void reset_perceptron(perceptron* Perceptron1);
void Algo_phase1(perceptron* Perceptron1, const int f_activation);
void Algo_phase2(perceptron* Perceptron1, const int NB_COUCHE_CACHEE, const int NB_VARIABLE_ENTREE, const int NB_UNITE_COUCHE_CACHEE, const int NB_UNITE_COUCHE_SORTIE, const int NB_EPOC, const int f_activation, const float ETA);
void Algo_phase3(perceptron* Perceptron1, const float ETA);
void Algo_phase4(perceptron* Perceptron1);
void init_param();
void set_input(perceptron* Perceptron1, const int fichier_utilisee, const int nb_variable_entree, const int NB_COUCHE_CACHEE, int* temp);
void random_init(perceptron* Perceptron1, const int NB_COUCHE_CACHEE, const int NB_UNITE_COUCHE_CACHEE, const int NB_VARIABLE_ENTREE, const int NB_UNITE_COUCHE_SORTIE);
void write_weight(perceptron* Perceptron1);
void read_weights(perceptron* Perceptron1);

int max(perceptron* Perceptron1);


#endif
/*End of file*/