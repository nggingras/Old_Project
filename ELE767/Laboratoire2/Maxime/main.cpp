#include "main.h"
#include "perceptron.h"
#include "string.h"

float calcul_fonct_activation(float Ival, int factivation);
void reset_perceptron(perceptron* Perceptron1);
std::string  supprime(std::string  texte, std::string  x);
void init_param();
float generate_random_float(float LO, float HI);
int generate_random_int(int LO, int HI);

/*					Déclaration de variable 								*/ 
#define NB_LIGNE_FICHIER_INIT 7
#define WEIGHT_MIN -0.1
#define WEIGHT_MAX 0.1
#define THETA_MIN 0
#define THETA_MAX 0
#define NB_DONNEE				 1560
#define NB_LIGNE_FICHIER	     1340

int valeur_init[NB_LIGNE_FICHIER_INIT];
float valeur_ETHA = 0;

void main(void)
{
	init_param();

	const int NB_COUCHE_CACHEE		 = valeur_init[0];
	const int NB_VARIABLE_ENTREE	 = ((valeur_init[1]*12)+1);
	const int NB_UNITE_COUCHE_CACHEE = valeur_init[2];
	const int NB_UNITE_COUCHE_SORTIE = valeur_init[3];
	const int NB_EPOC				 = valeur_init[4];
	const int f_activation			 = valeur_init[5]; // Choix fonction d'activation ---> 0 = sigmoid et 1 = tan
	const float ETA					 = valeur_ETHA;
	
	using namespace std;

	static	double FileRead[NB_LIGNE_FICHIER][NB_DONNEE + 1];
	static	vector<double> vUnclassedData_tab[NB_LIGNE_FICHIER];
	static	vector<double> vClassedData_tab[NB_LIGNE_FICHIER];
	static	vector<double> vUnclassedData;
	static	vector<double> vClassedData;

	std::string sDeuxPoints = ":";
	std::string sRead;
	std::string::size_type sz;

	/* Traitement de donnees*/
	{
		ifstream monFlux("data_train.txt");
		//Verifie l'ouverture du fichier
		if (monFlux)
		{
			//Lecture d'une ligne du fichier
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER; nb_ligne++)
			{
				for (int donnee = 0; donnee <= (NB_DONNEE); donnee++)
				{
					monFlux >> sRead;
					if (sRead.find(sDeuxPoints) != string::npos)
					{
						sRead = supprime(sRead, sDeuxPoints);
					}
					FileRead[nb_ligne][donnee] = std::stod(sRead, &sz);
				}
			}
		}

		//Boucle qui met à 0 toutes les valeurs dynamiques
		int compteur = 0;
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER; nb_ligne++)
		{
			compteur = 0;
			for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
				/*
				1er donnée du vecteur : nombre dit
				2-13 : données statiques
				14 : donnée énergie statique
				15-26 : données dynamique
				27: donnée énergie dynamique

				On veut retirer les données 15-27
				*/
			{
				if (compteur > 13)
					FileRead[nb_ligne][donnee] = 100;
				compteur %= 26;
				compteur++;
			}
		}
		// Boucle qui remplie le vecteur des donnees statiques restantes
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER; nb_ligne++)
		{
			vUnclassedData.clear();
			for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
			{
				if (FileRead[nb_ligne][donnee] != 100)
					vUnclassedData.push_back(FileRead[nb_ligne][donnee]);
			}
			vUnclassedData_tab[nb_ligne] = (vUnclassedData);
		}

		//Boucle qui rearrange les donnees statiques par energie
		int trame_place = 0;
		int compteur2 = 0;
		int sauvegarde_index = 0;
		int done = 0;
		double valeur_energie = -100;
		int test_multiple13 = 0;

		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER; nb_ligne++)
		{
			vClassedData.clear();
			for (trame_place = 0; trame_place < 60; trame_place++)
			{
				done = 0;
				valeur_energie = -100;
				//je cherche une donnee d energie plus basse
				for (int j = 1; j < vUnclassedData_tab[nb_ligne].size(); j++) //Skip la premiere donnee qui est le chiffre lu
				{
					if ((j % 13) == 0)
					{
						if (valeur_energie < vUnclassedData_tab[nb_ligne][j] && vUnclassedData_tab[nb_ligne][j] != double(100))
						{
							valeur_energie = vUnclassedData_tab[nb_ligne][j];
							sauvegarde_index = j;
						}
					}
				}

				if (trame_place == 0)
					vClassedData.push_back(vUnclassedData_tab[nb_ligne][0]); //On garde le chiffre dit

				//la valeur d energie obtenue est la plus haute
				sauvegarde_index -= 12; //On recule l'index de 12
				for (int k = 0; k < 12; k++)
				{
					vClassedData.push_back(vUnclassedData_tab[nb_ligne][(sauvegarde_index + k)]);
					vUnclassedData_tab[nb_ligne][sauvegarde_index + k] = 100;

				}
				//printf("%f\n", vUnclassedData_tab[nb_ligne][sauvegarde_index + 12]);
				vUnclassedData_tab[nb_ligne][sauvegarde_index + 12] = 100; // Met a 0 l'energie qu on ne sauvegarde pas

			}
			vClassedData_tab[nb_ligne] = vClassedData;
		}
	}// Fin traitement des données
	
	//Phase 0 (build perceptron)

	perceptron Perceptron1(NB_COUCHE_CACHEE, NB_UNITE_COUCHE_CACHEE, (NB_VARIABLE_ENTREE-1), NB_UNITE_COUCHE_SORTIE);
	Perceptron1.Build();


	//init random entrée, poids et theta
	{
		// init random de l'entrée
		//int random_num = generate_random_int(0, 1340);
		int random_num = 0;
		for (int index = 0; index < NB_VARIABLE_ENTREE - 1; index++) {
			Perceptron1.vEntres[index] = vClassedData_tab[random_num][index + 1];
			//printf("random_num:%d   Perceptron1.vEntres[%d]:%f\n", random_num, index, Perceptron1.vEntres[index]);
			if ((index == 0) || (index == NB_VARIABLE_ENTREE - 2)) { printf("random_num:%d   Perceptron1.vEntres[%d]:%f\n", random_num, index, Perceptron1.vEntres[index]); }// print premiere et derniere donnée d'entrée
		}
		
		int temp = vClassedData_tab[random_num][0]; // transforme la valeur desiré en int et la passe dans un switch pour aller coder les s_desire des cellules de sortie
		printf("\nSortie desire: %d\n", temp);
		switch (temp) {
		case 1:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 2:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 3:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 4:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 5:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 6:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 7:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 8:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 9:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 1;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 0:
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 1;
			break;

		}
		
		for (int i = 0 ; i < 10 ; i++) {
			printf("cell_sortie_%d:%d\n",i+1,Perceptron1.vCouches[NB_COUCHE_CACHEE].vCells[i].s_desire);
		}


		// init random des poids des couches caché	          
		for (int index_couche = 0; index_couche < NB_COUCHE_CACHEE; index_couche++) {
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_CACHEE; index_cell++) {
				for (int index_poids = 0; index_poids < (NB_VARIABLE_ENTREE - 1); index_poids++) {
					Perceptron1.vCouches[index_couche].vCells[index_cell].vWeights[index_poids] = generate_random_float(WEIGHT_MIN, WEIGHT_MAX);
					//printf("poids_couche_cache%d%d%d : %f\n", index_couche, index_cell, index_poids, Perceptron1.vCouches[index_couche].vCells[index_cell].vWeights[index_poids]);
				}
			}
		}

		// init random des poids de la couche de sortie	
		for (int index_couche = NB_COUCHE_CACHEE; index_couche < (NB_COUCHE_CACHEE + 1); index_couche++) {
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_SORTIE; index_cell++) {
				for (int index_poids = 0; index_poids < NB_UNITE_COUCHE_CACHEE; index_poids++) {
					Perceptron1.vCouches[index_couche].vCells[index_cell].vWeights[index_poids] = generate_random_float(WEIGHT_MIN, WEIGHT_MAX);
					//printf("poids_couche_sortie%d%d%d : %f\n", index_couche, index_cell, index_poids, Perceptron1.vCouches[index_couche].vCells[index_cell].vWeights[index_poids]);
				}
			}
		}

		// init random des theta couche caché
		for (int index_couche = 0; index_couche < NB_COUCHE_CACHEE; index_couche++) {
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_CACHEE; index_cell++) {
				Perceptron1.vCouches[index_couche].vCells[index_cell].theta = generate_random_float(THETA_MIN, THETA_MAX);
				printf("Theta%d%d : %f\n", index_couche, index_cell, Perceptron1.vCouches[index_couche].vCells[index_cell].theta);

			}
		}

		// init random des theta couche sortie
		for (int index_couche = NB_COUCHE_CACHEE; index_couche < (NB_COUCHE_CACHEE + 1); index_couche++) {
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_SORTIE; index_cell++) {
				Perceptron1.vCouches[index_couche].vCells[index_cell].theta = generate_random_float(THETA_MIN, THETA_MAX);
				printf("Theta%d%d : %f\n", index_couche, index_cell, Perceptron1.vCouches[index_couche].vCells[index_cell].theta);

			}
		}

	}//Fin init
	
	
	/***************************************************** Phase 1 activation ****************************************************************************/

	float temp_activation = 0;
	//iteration = couche
	//Boucle qui calcul l'activation pour chaque neuronne de chaque couche
	while(1) 
	{ //Iteration est la variable qui change de couche

			for (int i = 0; i < NB_EPOC; i++)
			{
				if (i != 0)
					reset_perceptron(&Perceptron1);
				//Valeur de X devient la valeur calculé en activation

						for (int couche_index = 0; couche_index < Perceptron1.vCouches.size(); couche_index++)
						{
							for (int cell_index = 0; cell_index < Perceptron1.vCouches[couche_index].vCells.size(); cell_index++)
							{
								cell* CurrentCell = &Perceptron1.vCouches[couche_index].vCells[cell_index];

								//chaque cellue contient un poids pour chaque entrée du perceptron
								if (couche_index == 0)
								{
									for (int entre_index = 0; entre_index < Perceptron1.vEntres.size(); entre_index++)
									{
										temp_activation = Perceptron1.vEntres[entre_index] * CurrentCell->vWeights[entre_index]; // x1 * w11 + x2 * w21 .....  
										Perceptron1.vCouches[0].vCells[cell_index].Ival += temp_activation;
									}
								}
								else //calcul de l'activation pour les autres couches
								{
									//chaque cellue contient un poids pour chaque cellule dans la couche precedente
									for (int prevC_cell_index = 0; prevC_cell_index < Perceptron1.vCouches[couche_index - 1].vCells.size(); prevC_cell_index++)
									{

										temp_activation = Perceptron1.vCouches[couche_index - 1].vCells[prevC_cell_index].Aval * CurrentCell->vWeights[prevC_cell_index];   // a1 * w11 + a2 * w21 .....  
										Perceptron1.vCouches[couche_index].vCells[cell_index].Ival += temp_activation;
									}
								}

								CurrentCell->Ival += CurrentCell->theta;
								CurrentCell->Aval = calcul_fonct_activation(CurrentCell->Ival, f_activation);
								//printf("Aval = %f\n", CurrentCell->Aval);
							}
						}

				/***************************************************** Phase 2 Signal d'erreur ****************************************************************************/

						

						

						float d_i[] = { 3, 2 }; // sortie désiré !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! a arrangé!!!!!!!!!!!!!!!!!!!!!!!!!!






						for (int couche = (NB_COUCHE_CACHEE + 1); couche > 0; couche--) {

							switch (f_activation) {
								case 0: //--------------- fonction SIGMOID--------------------------------------------------------------------------------//	

									// couche de sortie
									if (couche == (NB_COUCHE_CACHEE + 1)) {
										for (int i = 0; i < NB_UNITE_COUCHE_SORTIE; i++) { //i = quel neurone
											Perceptron1.vCouches[couche - 1].vCells[i].delta = ((d_i[i] - Perceptron1.vCouches[couche - 1].vCells[i].Aval) * (Perceptron1.vCouches[couche - 1].vCells[i].Aval * (1 - Perceptron1.vCouches[couche - 1].vCells[i].Aval)));
											//printf("delta %d %d = %f\n\n", i + 1, couche, Perceptron1.vCouches[couche - 1].vCells[i].delta);
										}
									}

									// couche caché
									if (couche <= NB_COUCHE_CACHEE) {

										for (int i = 0; i < NB_UNITE_COUCHE_CACHEE; i++) {
											for (int j = 0; j < NB_UNITE_COUCHE_SORTIE; j++) {
												Perceptron1.vCouches[couche - 1].vCells[i].delta += Perceptron1.vCouches[couche].vCells[j].delta * Perceptron1.vCouches[couche].vCells[j].vWeights[i];
											}

											Perceptron1.vCouches[couche - 1].vCells[i].delta = Perceptron1.vCouches[couche - 1].vCells[i].delta * (Perceptron1.vCouches[couche - 1].vCells[i].Aval * (1 - Perceptron1.vCouches[couche - 1].vCells[i].Aval));
											//printf("delta %d %d = %f\n\n", i + 1, couche, Perceptron1.vCouches[couche - 1].vCells[i].delta);
										}
									}

								break;

								case 1: //--------------- fonction TAN--------------------------------------------------------------------------------//

									// couche de sortie
									if (couche == (NB_COUCHE_CACHEE + 1)) {
										for (int i = 0; i < NB_UNITE_COUCHE_SORTIE; i++) { //i = quel neurone
											Perceptron1.vCouches[couche - 1].vCells[i].delta = ((d_i[i] - Perceptron1.vCouches[couche - 1].vCells[i].Aval) * (1 / (pow(cos(Perceptron1.vCouches[couche - 1].vCells[i].Ival), 2.0))));
											//printf("delta %d %d = %f\n\n", i + 1, couche, Perceptron1.vCouches[couche - 1].vCells[i].delta);
										}
									}

									// couche caché
									if (couche <= NB_COUCHE_CACHEE) {
										for (int i = 0; i < NB_UNITE_COUCHE_CACHEE; i++) {
											for (int j = 0; j < NB_UNITE_COUCHE_SORTIE; j++) {
												Perceptron1.vCouches[couche - 1].vCells[i].delta += Perceptron1.vCouches[couche].vCells[j].delta * Perceptron1.vCouches[couche].vCells[j].vWeights[i];
											}
											Perceptron1.vCouches[couche - 1].vCells[i].delta = Perceptron1.vCouches[couche - 1].vCells[i].delta * (1 / (pow(cos(Perceptron1.vCouches[couche - 1].vCells[i].Ival), 2.0)));
											//printf("delta %d %d = %f\n\n", i + 1, couche, Perceptron1.vCouches[couche - 1].vCells[i].delta);
										}
									}
								break;
							}
						}



					/***************************************************** Phase 3 Facteur de correction ****************************************************************************/

					for (int couche_index = 0; couche_index < Perceptron1.vCouches.size(); couche_index++){
						for (int cell_index = 0; cell_index < Perceptron1.vCouches[couche_index].vCells.size(); cell_index++){
							if (couche_index == 0){
								for (int weight_index = 0; weight_index < Perceptron1.vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++){
									Perceptron1.vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index] = ETA * Perceptron1.vEntres[weight_index] * Perceptron1.vCouches[couche_index].vCells[cell_index].delta;
								}
							}
							else{
								for (int weight_index = 0; weight_index < Perceptron1.vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++){
									Perceptron1.vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index] = ETA * Perceptron1.vCouches[couche_index - 1].vCells[weight_index].Aval * Perceptron1.vCouches[couche_index].vCells[cell_index].delta;
								}
							}
						}
					}

					/***************************************************** Phase 4 Mise à jour des poids ****************************************************************************/


					for (int couche_index = 0; couche_index < Perceptron1.vCouches.size(); couche_index++)
					{
						for (int cell_index = 0; cell_index < Perceptron1.vCouches[couche_index].vCells.size(); cell_index++)
						{
							for (int weight_index = 0; weight_index < Perceptron1.vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++)
							{
								Perceptron1.vCouches[couche_index].vCells[cell_index].vWeights[weight_index] += Perceptron1.vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index];
							}
						}
					}

			}//fin for(EPOC)
		
		system("pause");

	}//While(1)
}//fin main


/***************************************************** Déclaration de fonctions ****************************************************************************/

//Calcul le résultat de l'activation a partir de la I value
float calcul_fonct_activation(float Ival, int factivation)
{
	//fonction sigmoide
	int   iter = 0;
	float Aval = 0;

	if (factivation == 0)
	{
		Aval = 1 / (1 + exp(-Ival)); // sigmoide
	}
	else if (factivation == 1)
	{
		Aval = tan(Ival); // tangeant	
	}
	else if (factivation == 2)
	{
		Aval = sin(Ival); //sinus
	}
	else
	{
		printf("ERROR wrong activation fuction");
	}

	return(Aval);

}

// Fonction qui reset les variables du perceptron entre les époques
void reset_perceptron(perceptron* Perceptron1)
{
	const int NB_COUCHE_CACHEE = valeur_init[0];
	const int NB_UNITE_COUCHE_CACHEE = valeur_init[2];
	const int NB_UNITE_COUCHE_SORTIE = valeur_init[3];

	for(int couche_index = 0; couche_index < (NB_COUCHE_CACHEE+1); couche_index++)
	{
	
		if (couche_index == (NB_COUCHE_CACHEE)) 
		{
			for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_SORTIE; cell_index++)
			{
				Perceptron1->vCouches[couche_index].vCells[cell_index].Aval = 0;
				Perceptron1->vCouches[couche_index].vCells[cell_index].delta = 0;
				Perceptron1->vCouches[couche_index].vCells[cell_index].Ival = 0;
			}
		}
		else
		{
			for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_CACHEE; cell_index++)
			{
				Perceptron1->vCouches[couche_index].vCells[cell_index].Aval = 0;
				Perceptron1->vCouches[couche_index].vCells[cell_index].delta = 0;
				Perceptron1->vCouches[couche_index].vCells[cell_index].Ival = 0;
			}
		}
	}



	

}

// Fonction qui supprime x dans la chaine
std::string supprime(std::string texte, std::string  x)
{
	int p, i;
	for (i = 0; texte[i] != '\0'; i++)
	{
		if (texte[i] == x[0])  // on a trouve
		{
			for (p = i; texte[p] != 0; p++)
				texte[p] = texte[p + 1];
		}
	}
	return texte;
}

void init_param() {
	std::string ligne;
	std::string s_valeur;
	std::ifstream file_init("Init.txt"); // ouvre fichier

	if (file_init) {				// vérifie l'ouverture du fichier
		for (int i = 0; i < NB_LIGNE_FICHIER_INIT; i++) {
			if (i != 7) {
				getline(file_init, ligne);	                  // lire une ligne
				std::cout << ligne << std::endl; // print la ligne
				s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
				valeur_init[i] = std::stoi(s_valeur);         // convertis string en integer
				std::cout << valeur_init[i] << std::endl; // print la valeur
			}
			if (i == 8){
				getline(file_init, ligne);	                  // lire une ligne
				std::cout << ligne << std::endl; // print la ligne
				s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
				valeur_ETHA = std::stof(s_valeur);         // convertis string en float
				std::cout << valeur_ETHA << std::endl; // print la valeur
			}
		}
		file_init.close();
	}

}

float generate_random_float(float LO, float HI) {
	float random1 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return random1;
}

int generate_random_int(int LO, int HI) {
	int random2 = LO + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (HI - LO)));
	return random2;
}