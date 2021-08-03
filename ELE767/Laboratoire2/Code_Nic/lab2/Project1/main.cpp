/**************************** Laboratoire 2****************************************************************
 * Author : Nicolas Gauvin-Gingras, Maxime Vallee-Chene et Andy Beaudoin
 * Date   : 19-03-2020
 * Description : Lit les fichiers de données, roule l'algorithme d'apprentissage et calcul les performances.
 *****************************************************************************************************/
#include "main.h"



int valeur_init[NB_LIGNE_FICHIER_INIT];
float valeur_ETHA = 0;

static	double FileRead[NB_LIGNE_FICHIER][NB_DONNEE + 1];
static	vector<double> vUnclassedData_tab[NB_LIGNE_FICHIER];
static	vector<double> vClassedData_tab[NB_LIGNE_FICHIER];
static	vector<double> vUnclassedData;
static	vector<double> vClassedData;
static	vector<double> vClassedData_VC;
static	vector<double> vClassedData_TEST;

static	double FileRead_test[NB_LIGNE_FICHIER_TEST][NB_DONNEE + 1] = { 0 };
static	vector<double> vUnclassedData_tab_test[NB_LIGNE_FICHIER_TEST];
static	vector<double> vClassedData_tab_test[NB_LIGNE_FICHIER_TEST];
static	vector<double> vUnclassedData_test;
static	vector<double> vClassedData_test;
std::string sRead_test;

static	double FileRead_vc[NB_LIGNE_FICHIER_VC][NB_DONNEE + 1] = { 0 };
static	vector<double> vUnclassedData_tab_vc[NB_LIGNE_FICHIER_VC];
static	vector<double> vClassedData_tab_vc[NB_LIGNE_FICHIER_VC];
static	vector<double> vUnclassedData_vc;
static	vector<double> vClassedData_vc;

std::string sDeuxPoints = ":";
std::string sRead;
std::string::size_type sz;


void main(void)
{
	init_param();

	const int NB_COUCHE_CACHEE       = valeur_init[0];
	const int NB_VARIABLE_ENTREE     = ((valeur_init[1] * 12) + 1);
	const int NB_UNITE_COUCHE_CACHEE = valeur_init[2];
	const int NB_UNITE_COUCHE_SORTIE = valeur_init[3];
	      int NB_EPOC                = valeur_init[4];
	const int f_activation           = valeur_init[5]; // Choix fonction d'activation ---> 0 = sigmoid et 1 = tan
	const int Learn = valeur_init[7]; // Choix si on apprend ou on load des poids
	float ETA = valeur_ETHA;
	int compteur_bonne_valeur = 0;
	int temp = 0;


	/****************************************************************************************************************************************************************/
	/***************************************************** Lecture des fichiers *************************************************************************************/
	/****************************************************************************************************************************************************************/

	/***************************************************** Lecture du fichier TRAIN *************************************************************************************/
	{
		ifstream monFlux("data_train.txt");

		/* Vérifie l'ouverture du fichier */
		if (monFlux)
		{
			/* Lecture d'une string dans le fichier, pour toute le fichier */
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

		/* Boucle qui met à 0 toutes les valeurs dynamiques */
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
		/* Boucle qui remplie le vecteur des donnees statiques restantes */
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


		/* Boucle qui rearrange les donnees statiques par energie */
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
				/* Boucle qui trouve la donnée d'énergie la plus petite en ne lisant pas la première donnée qui est le chiffre lu*/
				for (int j = 1; j < vUnclassedData_tab[nb_ligne].size(); j++)
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
					vClassedData.push_back(vUnclassedData_tab[nb_ligne][0]); /* On garde le chiffre dit */

				/* la valeur d energie obtenue est la plus haute*/
				sauvegarde_index -= 12; /* On recule l'index de 12 */
				for (int k = 0; k < 12; k++)
				{
					vClassedData.push_back(vUnclassedData_tab[nb_ligne][(sauvegarde_index + k)]);
					vUnclassedData_tab[nb_ligne][sauvegarde_index + k] = 100;

				}
				/* Met à 100 l'énergie qu'on ne sauvegarde pas */
				vUnclassedData_tab[nb_ligne][sauvegarde_index + 12] = 100;

			}
			vClassedData_tab[nb_ligne] = vClassedData;
		}
	}



	/***************************************************** Lecture du fichier VC ****************************************************************************/


	{
		ifstream monFlux("data_vc.txt");
		/* Vérifie l'ouverture du fichier */
		if (monFlux)
		{
			/* Lecture d'une string dans le fichier, pour toute le fichier */
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
			{
				for (int donnee = 0; donnee <= (NB_DONNEE); donnee++)
				{
					monFlux >> sRead;
					if (sRead.find(sDeuxPoints) != string::npos)
					{
						sRead = supprime(sRead, sDeuxPoints);
					}
					FileRead_vc[nb_ligne][donnee] = std::stod(sRead, &sz);
				}
			}
		}

		/* Boucle qui met à 0 toutes les valeurs dynamiques */
		int compteur = 0;
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
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
					FileRead_vc[nb_ligne][donnee] = 100;
				compteur %= 26;
				compteur++;
			}
		}
		/* Boucle qui remplie le vecteur des donnees statiques restantes */
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
		{
			vUnclassedData_vc.clear();
			for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
			{
				if (FileRead_vc[nb_ligne][donnee] != 100)
					vUnclassedData_vc.push_back(FileRead_vc[nb_ligne][donnee]);
			}
			vUnclassedData_tab_vc[nb_ligne] = (vUnclassedData_vc);
		}


		/* Boucle qui rearrange les donnees statiques par energie */
		int trame_place = 0;
		int compteur2 = 0;
		int sauvegarde_index = 0;
		int done = 0;
		double valeur_energie = -100;
		int test_multiple13 = 0;

		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
		{
			vClassedData_vc.clear();
			for (trame_place = 0; trame_place < 60; trame_place++)
			{
				done = 0;
				valeur_energie = -100;
				/* Boucle qui trouve la donnée d'énergie la plus petite en ne lisant pas la première donnée qui est le chiffre lu*/
				for (int j = 1; j < vUnclassedData_tab_vc[nb_ligne].size(); j++)
				{
					if ((j % 13) == 0)
					{
						if (valeur_energie < vUnclassedData_tab_vc[nb_ligne][j] && vUnclassedData_tab_vc[nb_ligne][j] != double(100))
						{
							valeur_energie = vUnclassedData_tab_vc[nb_ligne][j];
							sauvegarde_index = j;
						}
					}
				}

				if (trame_place == 0)
					vClassedData_vc.push_back(vUnclassedData_tab_vc[nb_ligne][0]); /* On garde le chiffre dit */

				/* la valeur d energie obtenue est la plus haute*/
				sauvegarde_index -= 12; /* On recule l'index de 12 */
				for (int k = 0; k < 12; k++)
				{
					vClassedData_vc.push_back(vUnclassedData_tab_vc[nb_ligne][(sauvegarde_index + k)]);
					vUnclassedData_tab_vc[nb_ligne][sauvegarde_index + k] = 100;

				}
				/* Met à 100 l'énergie qu'on ne sauvegarde pas */
				vUnclassedData_tab_vc[nb_ligne][sauvegarde_index + 12] = 100;

			}
			vClassedData_tab_vc[nb_ligne] = vClassedData_vc;
		}
	}

	/***************************************************** Lecture du fichier TEST ****************************************************************************/

	{
		ifstream monFlux("data_test.txt");
		/* Vérifie l'ouverture du fichier */
		if (monFlux)
		{
			/* Lecture d'une string dans le fichier, pour toute le fichier */
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
			{
				for (int donnee = 0; donnee <= (NB_DONNEE); donnee++)
				{
					monFlux >> sRead_test;
					if (sRead_test.find(sDeuxPoints) != string::npos)
					{
						sRead_test = supprime(sRead_test, sDeuxPoints);
					}
					FileRead_test[nb_ligne][donnee] = std::stod(sRead_test, &sz);
				}
			}
		}

		/* Boucle qui met à 0 toutes les valeurs dynamiques */
		int compteur = 0;
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
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
					FileRead_test[nb_ligne][donnee] = 100;
				compteur %= 26;
				compteur++;
			}
		}
		/* Boucle qui remplie le vecteur des donnees statiques restantes */
		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
		{
			vUnclassedData_test.clear();
			for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
			{
				if (FileRead_test[nb_ligne][donnee] != 100)
					vUnclassedData_test.push_back(FileRead_test[nb_ligne][donnee]);
			}
			vUnclassedData_tab_test[nb_ligne] = (vUnclassedData_test);
		}


		/* Boucle qui rearrange les donnees statiques par energie */
		int trame_place = 0;
		int compteur2 = 0;
		int sauvegarde_index = 0;
		int done = 0;
		double valeur_energie = -100;
		int test_multiple13 = 0;

		for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
		{
			vClassedData_test.clear();
			for (trame_place = 0; trame_place < 60; trame_place++)
			{
				done = 0;
				valeur_energie = -100;
				/* Boucle qui trouve la donnée d'énergie la plus petite en ne lisant pas la première donnée qui est le chiffre lu*/
				for (int j = 1; j < vUnclassedData_tab_test[nb_ligne].size(); j++)
				{
					if ((j % 13) == 0)
					{
						if (valeur_energie < vUnclassedData_tab_test[nb_ligne][j] && vUnclassedData_tab_test[nb_ligne][j] != double(100))
						{
							valeur_energie = vUnclassedData_tab_test[nb_ligne][j];
							sauvegarde_index = j;
						}
					}
				}


				if (trame_place == 0)
					vClassedData_test.push_back(vUnclassedData_tab_test[nb_ligne][0]);/* On garde le chiffre dit */

			/* la valeur d energie obtenue est la plus haute*/
				sauvegarde_index -= 12; /* On recule l'index de 12 */
				for (int k = 0; k < 12; k++)
				{
					vClassedData_test.push_back(vUnclassedData_tab_test[nb_ligne][(sauvegarde_index + k)]);
					vUnclassedData_tab_test[nb_ligne][sauvegarde_index + k] = 100;

				}
				/* Met à 100 l'énergie qu'on ne sauvegarde pas */
				vUnclassedData_tab_test[nb_ligne][sauvegarde_index + 12] = 100;

			}
			vClassedData_tab_test[nb_ligne] = vClassedData_test;
		}
	}


	/***************************************************** Phase 0 (build perceptron) ****************************************************************************/

	perceptron Perceptron1(NB_COUCHE_CACHEE, NB_UNITE_COUCHE_CACHEE, (NB_VARIABLE_ENTREE - 1), NB_UNITE_COUCHE_SORTIE);
	Perceptron1.Build();

	/***************************************************** Initialisation random des entrées, poids et thetas ****************************************************************************/
	if(Learn) // Si on veut seulement loader des poids
	{
		random_init(&Perceptron1, NB_COUCHE_CACHEE, NB_UNITE_COUCHE_CACHEE, NB_VARIABLE_ENTREE, NB_UNITE_COUCHE_SORTIE);
	}
	else
	{
		read_weights(&Perceptron1);
		NB_EPOC = 1;
    }
	/********************************************************** Algorithme d'apprentissage ********************************************************************************/

	while (1)
	{ 
		/* Boucle qui repète l'algorithme selon le nombre d'époque choisit dans le fichier Init */
		printf("Affichage de l'epoque en cours d'apprentissage\n");
		for (int i = 0; i < NB_EPOC; i++)
		{
			if (NB_EPOC >= 10)
				ETA -= 0.005;

			printf("epoque = %d\n", i);

			/* Boucle qui parcours toutes les données du fichier aléatoirement */
			for (int index_value = 0; index_value < NB_LIGNE_FICHIER; index_value++)
			{
				set_input(&Perceptron1, 1, NB_VARIABLE_ENTREE, NB_COUCHE_CACHEE, &temp);
				reset_perceptron(&Perceptron1);

				/***************************************************** Phase 1 activation ****************************************************************************/

				Algo_phase1(&Perceptron1, f_activation);
			
				
					/* Condition qui vérifie si la valeur max du perceptron est celle de la valeur désirée*/
					/* Si oui, on note pour la performance et on passe l'apprentissage */
					if ((max(&Perceptron1) == (temp - 1)) || max(&Perceptron1) == (temp - 9))
					{
						if (i == NB_EPOC - 1)
						{
							compteur_bonne_valeur += 1;
						}
					}
					/* Si non, on apprend */
					else if(Learn)
					{
						/***************************************************** Phase 2 Signal d'erreur ****************************************************************************/

						Algo_phase2(&Perceptron1, NB_COUCHE_CACHEE, NB_VARIABLE_ENTREE, NB_UNITE_COUCHE_CACHEE, NB_UNITE_COUCHE_SORTIE, NB_EPOC, f_activation, ETA);

						/***************************************************** Phase 3 Facteur de correction ****************************************************************************/

						Algo_phase3(&Perceptron1, ETA);

						/***************************************************** Phase 4 Mise à jour des poids ****************************************************************************/

						Algo_phase4(&Perceptron1);

					}
				
			}//Fin lecture une donnee

		}//fin for(EPOC)

		write_weight(&Perceptron1);
		/***************************************************** Vérification sur les données du fichier VC ****************************************************************************/

		/* On recommence la pige aléatoire, mais on prend les donnees du vecteurs validation croisee et on calcul la performance avec les poids obtenues durant 
		l'apprentissage */ 
		int compteur_performance = 0;
		for (int index_value = 0; index_value < NB_LIGNE_FICHIER_VC; index_value++)
		{

			set_input(&Perceptron1, 2, NB_VARIABLE_ENTREE, NB_COUCHE_CACHEE, &temp);

			/* On recalcul l'activation avec les données d'entrée du fichier VC */
			reset_perceptron(&Perceptron1);
			Algo_phase1(&Perceptron1, f_activation);

			/* Condition qui vérifie si la valeur max du perceptron est celle de la valeur désirée*/
			/* Si oui, on note pour la performance */
			int test = max(&Perceptron1);
			if ((max(&Perceptron1) == (temp - 1)) || max(&Perceptron1) == (temp - 9))
			{
				compteur_performance += 1;
			}
		}

		/***************************************************** Vérification sur les données du fichier TEST ****************************************************************************/

		/* On recommence la pige aléatoire, mais on prend les donnees du vecteurs validation croisee et on calcul la performance avec les poids obtenues durant
		l'apprentissage */
		int compteur_performance_test = 0;
		for (int index_value = 0; index_value < NB_LIGNE_FICHIER_TEST; index_value++)
		{
			set_input(&Perceptron1, 3, NB_VARIABLE_ENTREE, NB_COUCHE_CACHEE, &temp);

			//Puis on recalcule la valeur d'activation pour obtenir la performance des donnees de validation croisee
			reset_perceptron(&Perceptron1);
			Algo_phase1(&Perceptron1, f_activation);
			
			/* Condition qui vérifie si la valeur max du perceptron est celle de la valeur désirée*/
			/* Si oui, on note pour la performance */
			int test = max(&Perceptron1);
			if ((max(&Perceptron1) == (temp - 1)) || max(&Perceptron1) == (temp - 9))
			{
				compteur_performance_test += 1;
			}

		}

		/* Calcul des performances */
		double performance_VC = double(compteur_performance) / NB_LIGNE_FICHIER_VC;
		double performance_train = double(compteur_bonne_valeur) / NB_LIGNE_FICHIER;
		double performance_test = double(compteur_performance_test) / NB_LIGNE_FICHIER_TEST;

		/***************************************************** Écriture des poids du perceptron dans un fichier ****************************************************************************/
		
		printf(" la performane de train est de %f\n", performance_train);
		printf(" la performane de la VC est de %f\n", performance_VC);
		printf(" la performane des donnees de test est de %f\n", performance_test);
		system("pause");
		
	}/* fin main */
}
/******************************************************************************************************************************************************************************/
/************************************************************** Déclaration de fonctions **************************************************************************************/
/******************************************************************************************************************************************************************************/

/***************************************************************************************************
 * Calcul_fonct_activation
 *
 * @brief Fonction qui calcul le résultat de l'activation en fonction de la valeur d'activation et de la fonction
 *
 * @parameters : 	-La valeur d'activation
 *					-La fonction d'activation
 *
 * @return     :	Retourne le résultat de la fonction d'activation.
 **************************************************************************************************/
	float calcul_fonct_activation(float Ival, int factivation)
	{

		int   iter = 0;
		float Aval = 0;

		/* Sigmoide */
		if (factivation == 0)
		{
			Aval = 1 / (1 + exp(-Ival)); 
		}
		/* Tangeant */
		else if (factivation == 1)
		{
			Aval = tan(Ival); 	
		}
		/* Sinus */
		else if (factivation == 2)
		{
			Aval = sin(Ival);
		}
		else
		{
			printf("ERROR wrong activation fuction");
		}

		return(Aval);

	}

	/***************************************************************************************************
 * reset_perceptron
 *
 * @brief Fonction qui réinitialise les valeurs qui doivent être remises à 0 du Perceptron entre les époques
 *
 * @parameters : 	L'adresse du perceptron
 *
 * @return     :	Void
 **************************************************************************************************/
	void reset_perceptron(perceptron * Perceptron1)
	{
		const int NB_COUCHE_CACHEE       = valeur_init[0];
		const int NB_UNITE_COUCHE_CACHEE = valeur_init[2];
		const int NB_UNITE_COUCHE_SORTIE = valeur_init[3];

		/* Boucle qui partir le Perceptron et remet Aval, delta et Ival à 0 pour toutes les cellules */
		for (int couche_index = 0; couche_index < (NB_COUCHE_CACHEE + 1); couche_index++)
		{

			if (couche_index == (NB_COUCHE_CACHEE))
			{
				for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_SORTIE; cell_index++)
				{
					Perceptron1->vCouches[couche_index].vCells[cell_index].Aval  = 0;
					Perceptron1->vCouches[couche_index].vCells[cell_index].delta = 0;
					Perceptron1->vCouches[couche_index].vCells[cell_index].Ival  = 0;
				}
			}
			else
			{
				for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_CACHEE; cell_index++)
				{
					Perceptron1->vCouches[couche_index].vCells[cell_index].Aval  = 0;
					Perceptron1->vCouches[couche_index].vCells[cell_index].delta = 0;
					Perceptron1->vCouches[couche_index].vCells[cell_index].Ival  = 0;
				}
			}
		}
	}

	/***************************************************************************************************
 * supprime
 *
 * @brief Fonctione qui trouve un caractère dans un variable de type string et qui supprime ce caractère
 *		  en réécrivant la string au complet sans le caractère
 *
 * @parameters : 	-String dans laquelle le caractère doit être trouvé
 *					-Caractère à trouver et à supprimé
 *
 * @return     :	Retourne la nouvelle string sans le caractère
 **************************************************************************************************/
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


	/***************************************************************************************************
 * init_param
 *
 * @brief Fonction qui ouvre le fichier texte d'initialisation et qui va mettre les valeurs lues dans un tableau
 *
 * @parameters : 	Void
 *
 * @return     :	Void
 **************************************************************************************************/
	void init_param() {
		std::string ligne;
		std::string s_valeur;
		std::ifstream file_init("Init.txt"); // ouvre fichier

		if (file_init) {				// vérifie l'ouverture du fichier
			printf("Affichage des donnees du fichier Init \n");
			for (int i = 0; i < NB_LIGNE_FICHIER_INIT; i++) {
				if (i != 6) {
					getline(file_init, ligne);	                  // lire une ligne
					std::cout << ligne << std::endl;              // print la ligne
					s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
					valeur_init[i] = std::stoi(s_valeur);         // convertis string en integer
					//std::cout << valeur_init[i] << std::endl;   // print la valeur
				}
				else if (i == 6) 
				{
					getline(file_init, ligne);	                  // lire une ligne
					std::cout << ligne << std::endl;              // print la ligne
					s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
					valeur_ETHA = std::stof(s_valeur);            // convertis string en float
					//std::cout << valeur_ETHA << std::endl;      // print la valeur
				}
			}
			file_init.close();
		}

	}

/***************************************************************************************************
 * generate_random_float
 *
 * @brief Fonction qui retourne une valeur aléatoire de type float, situé entre les deux bornes en paramètre
 *
 * @parameters : 	La borne inférieure et la borne supérieure
 *
 * @return     :	la valeur aléatoire générée
 **************************************************************************************************/
	float generate_random_float(float LO, float HI) {
		float random1 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
		return random1;
	}

/***************************************************************************************************
 * max
 *
 * @brief La fonction max trouve la position de la valeur maximum
 *
 * @parameters : 	L'adresse de la classe Perceptron
 *
 * @return     :	Int contenant la valeur de la position de la valeur max
 **************************************************************************************************/
	int max(perceptron* Perceptron1)
	{
		double valeur_maximum = 0;
		int nb_couche = Perceptron1->NbCouchesC;
		int index_valeur_max = 0;

		/* Boucle qui trouve la position de la valeur maximum dans le Perceptron */
		for (int index_activation = 0; index_activation < Perceptron1->NbSorties; index_activation++)
		{
			if (valeur_maximum < Perceptron1->vCouches[nb_couche].vCells[index_activation].Aval)
			{
				valeur_maximum = Perceptron1->vCouches[nb_couche].vCells[index_activation].Aval;
				index_valeur_max = index_activation;
			}
		}
		return index_valeur_max;
	}
	
/***************************************************************************************************
* Algo_phase3
*
* @brief Fonction qui exécute la phase 3 de l'algorithme d'apprentissage
*
* @parameters : 	L'adresse de la classe Perceptron
*
* @return     :		Void
**************************************************************************************************/
	void Algo_phase3(perceptron* Perceptron1, const float ETA)
	{
		/* Deux boucles for qui parcours les cellules sur tous les couches du Perceptron */
		for (int couche_index = 0; couche_index < Perceptron1->vCouches.size(); couche_index++) 
		{
			for (int cell_index = 0; cell_index < Perceptron1->vCouches[couche_index].vCells.size(); cell_index++) 
			{
				if (couche_index == 0) 
				{
					for (int weight_index = 0; weight_index < Perceptron1->vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++) {
						/* Delta w = ETA * Entrée * delta */
						Perceptron1->vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index] = ETA * Perceptron1->vEntres[weight_index] * Perceptron1->vCouches[couche_index].vCells[cell_index].delta;
					}
				}
				else {
					for (int weight_index = 0; weight_index < Perceptron1->vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++) 
					{
						/* Delta w = ETA * activation * delta */
						Perceptron1->vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index] = ETA * Perceptron1->vCouches[couche_index - 1].vCells[weight_index].Aval * Perceptron1->vCouches[couche_index].vCells[cell_index].delta;
					}
				}
			}
		}
	}

/***************************************************************************************************
* Algo_phase4
*
* @brief Fonction qui exécute la phase 4 de l'algorithme d'apprentissage
*
* @parameters : 	L'adresse de la classe Perceptron
*
* @return     :		Void
	**************************************************************************************************/
	void Algo_phase4(perceptron* Perceptron1)
	{
		/* Deux boucles for qui parcours les cellules sur tous les couches du Perceptron */
		for (int couche_index = 0; couche_index < Perceptron1->vCouches.size(); couche_index++)
		{
			for (int cell_index = 0; cell_index < Perceptron1->vCouches[couche_index].vCells.size(); cell_index++)
			{
				for (int weight_index = 0; weight_index < Perceptron1->vCouches[couche_index].vCells[cell_index].NbWeights; weight_index++)
				{
					/* w (new) = w (old) + Delta w*/
					Perceptron1->vCouches[couche_index].vCells[cell_index].vWeights[weight_index] += Perceptron1->vCouches[couche_index].vCells[cell_index].vDeltaWeights[weight_index];
				}
			}
		}
	}
/***************************************************************************************************
* Algo_phase2
*
* @brief Fonction qui exécute la phase 2 de l'algorithme d'apprentissage
*
* @parameters : 	L'adresse de la classe Perceptron
*
* @return     :		Void
	**************************************************************************************************/
	void Algo_phase2(perceptron* Perceptron1, const int NB_COUCHE_CACHEE, const int NB_VARIABLE_ENTREE, const int NB_UNITE_COUCHE_CACHEE, const int NB_UNITE_COUCHE_SORTIE, const int NB_EPOC, const int f_activation, const float ETA)
	{
		/* Boucle qui parcours toutes les couches, en partant de la fin vers le début */
		for (int couche = (NB_COUCHE_CACHEE + 1); couche > 0; couche--) {

			/* Switch case pour nos deux fonctions d'activation, déterminé selon le fichier Init */
			switch (f_activation) {
			case 0: //--------------- fonction SIGMOID--------------------------------------------------------------------------------//	

				/* Boucle pour la couche de sortie */
				if (couche == (NB_COUCHE_CACHEE + 1)) {
					/* Boucle qui parcours toutes les cellules dans la couche */
					for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_SORTIE; cell_index++) {
						/* dela = (sortie désirée - f(a)) * f(a)(1-f(a))  */
						Perceptron1->vCouches[couche - 1].vCells[cell_index].delta = ((Perceptron1->vCouches[couche - 1].vCells[cell_index].s_desire - Perceptron1->vCouches[couche - 1].vCells[cell_index].Aval) * (Perceptron1->vCouches[couche - 1].vCells[cell_index].Aval * (1 - Perceptron1->vCouches[couche - 1].vCells[cell_index].Aval)));
					}
				}

				/* Couches cachées */
				if (couche <= NB_COUCHE_CACHEE)
				{
					/* Boucle qui parcours toutes les cellules*/
					for (int hidden_cell_index = 0; hidden_cell_index < NB_UNITE_COUCHE_CACHEE; hidden_cell_index++)
					{
						/* Calcul pour la dernière couche cachées */
						if (couche == NB_COUCHE_CACHEE)
						{
							for (int out_cell_index = 0; out_cell_index < NB_UNITE_COUCHE_SORTIE; out_cell_index++)
							{
								/* delta += delta * w */
								Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta += Perceptron1->vCouches[couche].vCells[out_cell_index].delta * Perceptron1->vCouches[couche].vCells[out_cell_index].vWeights[hidden_cell_index];
							}
							/* delta = delta * f(a)*(1-f(a)) */
							Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta = Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta * (Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Aval * (1 - Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Aval));
						}
						/* Calcul pour les couches cachées, sauf la dernière qui ne possède pas le même nombre de poids */
						else
						{
							for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_CACHEE; cell_index++)
							{
								/* delta += delta * w */
								Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta += Perceptron1->vCouches[couche].vCells[cell_index].delta * Perceptron1->vCouches[couche].vCells[cell_index].vWeights[hidden_cell_index];
							}
							/* delta = delta * f(a)*(1-f(a)) */
							Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta = Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta * (Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Aval * (1 - Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Aval));
						}
					}
				}

				break;

			case 1: //--------------- fonction TAN--------------------------------------------------------------------------------//

				/* Couches cachées */
				if (couche == (NB_COUCHE_CACHEE + 1))
				{
					for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_SORTIE; cell_index++)
					{
						/* delta = (sortie désirée- f(a))* f'(a) */
						Perceptron1->vCouches[couche - 1].vCells[cell_index].delta = ((Perceptron1->vCouches[couche - 1].vCells[cell_index].s_desire - Perceptron1->vCouches[couche - 1].vCells[cell_index].Aval) * (1 / (pow(cos(Perceptron1->vCouches[couche - 1].vCells[cell_index].Ival), 2.0))));
					}
				}

				/* Couches cachées */
				if (couche <= NB_COUCHE_CACHEE)
				{
					/* Boucle qui parcours toutes les cellules*/
					for (int hidden_cell_index = 0; hidden_cell_index < NB_UNITE_COUCHE_CACHEE; hidden_cell_index++)
					{
						/* Calcul pour la dernière couche cachées */
						if (couche == NB_COUCHE_CACHEE)
						{
							for (int out_cell_index = 0; out_cell_index < NB_UNITE_COUCHE_SORTIE; out_cell_index++)
							{
								/* delta += delta * w */
								Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta += Perceptron1->vCouches[couche].vCells[out_cell_index].delta * Perceptron1->vCouches[couche].vCells[out_cell_index].vWeights[hidden_cell_index];
							}
							/* delta = delta = delta * f'(a)  */
							Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta = Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta * (1 / (pow(cos(Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Ival), 2.0)));
						}
						/* Calcul pour les couches cachées, sauf la dernière qui ne possède pas le même nombre de poids */
						else
						{
							for (int cell_index = 0; cell_index < NB_UNITE_COUCHE_CACHEE; cell_index++)
							{
								/* delta += delta * w */
								Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta += Perceptron1->vCouches[couche].vCells[cell_index].delta * Perceptron1->vCouches[couche].vCells[cell_index].vWeights[hidden_cell_index];
							}
							/* delta = delta * f'(a) */
							Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta = Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].delta * (1 / (pow(cos(Perceptron1->vCouches[couche - 1].vCells[hidden_cell_index].Ival), 2.0)));
						}
					}
				}
				break;

			}
		}
	}
/***************************************************************************************************
* Algo_phase1
*
* @brief Fonction qui exécute la phase 1 de l'algorithme d'apprentissage
*
* @parameters : 	L'adresse de la classe Perceptron
*
* @return     :		Void
	**************************************************************************************************/
	void Algo_phase1(perceptron* Perceptron1, const int f_activation)
	{
		float temp_activation = 0;

		/* Boucle qui parcours toutes les couches */
		for (int couche_index = 0; couche_index < Perceptron1->vCouches.size(); couche_index++)
		{
			/* Boucle qui parcours toutes les cellules de la couche */
			for (int cell_index = 0; cell_index < Perceptron1->vCouches[couche_index].vCells.size(); cell_index++)
			{
				cell* CurrentCell = &Perceptron1->vCouches[couche_index].vCells[cell_index];

				/* Chaque cellule contient un poids pour chaque entrée du perceptron */
				/* Calcul de l'activation pour la première couche */
				if (couche_index == 0)
				{
					for (int entre_index = 0; entre_index < Perceptron1->vEntres.size(); entre_index++)
					{
						/* x1*w11 + x2*w21 ... */
						temp_activation = Perceptron1->vEntres[entre_index] * CurrentCell->vWeights[entre_index];
						Perceptron1->vCouches[0].vCells[cell_index].Ival += temp_activation;
					}
				}
				/* Calcul de l'activation pour les autres couches */
				else 
				{
					/* Chaque cellules contient un poids pour chaque cellules dans la couche précédente */
					for (int prevC_cell_index = 0; prevC_cell_index < Perceptron1->vCouches[couche_index - 1].vCells.size(); prevC_cell_index++)
					{
						/* a1*w11 + a2*w21 ... */
						temp_activation = Perceptron1->vCouches[couche_index - 1].vCells[prevC_cell_index].Aval * CurrentCell->vWeights[prevC_cell_index]; 
						Perceptron1->vCouches[couche_index].vCells[cell_index].Ival += temp_activation;
					}
				}

				CurrentCell->Ival += CurrentCell->theta;
				CurrentCell->Aval = calcul_fonct_activation(CurrentCell->Ival, f_activation);
			}
		}
	}


/**************************************************************************************************
* set_input
*
* @brief Fonction qui pige l'entrée aléatoirement et configure les sorties désirées du Perceptron
*
* @parameters : 	-L'adresse de la classe Perceptron
*					-Le fichier lu
*					-La taille du vecteur d'entrée utilisé
*					-Le nombre de couches cachées
*					-l'adresse d'une variable temporaire pour la pige
*					-Le tableau ou les données sont enregistrés
* 
* @return     :		Void
**************************************************************************************************/
void set_input(perceptron* Perceptron1, const int fichier_utilisee, const int nb_variable_entree, const int NB_COUCHE_CACHEE, int* temp)
	{
		int random_num = 0;
		double test = 0;
		/* Switch case qui varie la génération aléatoire selon le fichier d'entrée */
		switch (fichier_utilisee)
		{
			/* Fichier train */
		case 1:

			random_num = (rand() % 1339) + 1;
			for (int index = 0; index < nb_variable_entree - 1; index++)
			{
				test = vClassedData_tab[random_num][index + 1];
				Perceptron1->vEntres[index] =test;
			}
			*temp = vClassedData_tab[random_num][0];// transforme la valeur desiré en int et la passe dans un switch pour aller coder les s_desire des cellules de sortie
			break;
			/* Fichier VC */
		case 2:

			random_num = (rand() % 119) + 1;
			for (int index = 0; index < nb_variable_entree - 1; index++)
			{
				Perceptron1->vEntres[index] = vClassedData_tab_vc[random_num][index + 1];
			}
			*temp = vClassedData_tab_vc[random_num][0];// transforme la valeur desiré en int et la passe dans un switch pour aller coder les s_desire des cellules de sortie
			break;
			/* Fichier test */
		case 3:

			random_num = (rand() % 779) + 1;
			for (int index = 0; index < nb_variable_entree - 1; index++)
			{
				Perceptron1->vEntres[index] = vClassedData_tab_test[random_num][index + 1];
			}
			*temp = vClassedData_tab_test[random_num][0];// transforme la valeur desiré en int et la passe dans un switch pour aller coder les s_desire des cellules de sortie
			break;


		}

		// transforme la valeur desiré en int et la passe dans un switch pour aller coder les s_desire des cellules de sortie

		switch (*temp) {
		case 1:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 2:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 3:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 4:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 5:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 6:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 7:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 8:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;
		case 9:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 1;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 0;
			break;

		case 0:
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[0].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[1].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[2].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[3].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[4].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[5].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[6].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[7].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[8].s_desire = 0;
			Perceptron1->vCouches[NB_COUCHE_CACHEE].vCells[9].s_desire = 1;
			break;

		}
	}

/**************************************************************************************************
* random_init
*
* @brief Fonction qui initialise les poids et les thetas de facon aléatoire
*
* @parameters : 	-L'adresse de la classe Perceptron	
*					-Le nombre de couches cachées
*					-Le nombre de cellules dans les couches cachées
*					-La taille du vecteur d'entrée
*					-Le nombre d'unité dans la couche de sortie
* 
* @return     :		Void
**************************************************************************************************/
void random_init(perceptron *Perceptron1, const int NB_COUCHE_CACHEE, const int NB_UNITE_COUCHE_CACHEE, const int NB_VARIABLE_ENTREE, const int NB_UNITE_COUCHE_SORTIE)
{
	/* Boucle qui parcours toutes les couches */
	for (int index_couche = 0; index_couche < NB_COUCHE_CACHEE; index_couche++)
	{
		/* La première couche possède autant de poids que d'entrée */
		if (index_couche == 0)
		{
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_CACHEE; index_cell++)
			{
				for (int index_poids = 0; index_poids < (NB_VARIABLE_ENTREE - 1); index_poids++)
				{
					Perceptron1->vCouches[index_couche].vCells[index_cell].vWeights[index_poids] = generate_random_float(WEIGHT_MIN, WEIGHT_MAX);
				}
			}
		}

		/* Le couches suivantes ont le même nombre de poids qu'il y a de cellules dans les couches cachées */
		else
		{
			for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_CACHEE; index_cell++)
			{
				for (int index_poids = 0; index_poids < (NB_UNITE_COUCHE_CACHEE - 1); index_poids++)
				{
					Perceptron1->vCouches[index_couche].vCells[index_cell].vWeights[index_poids] = generate_random_float(WEIGHT_MIN, WEIGHT_MAX);
				}
			}
		}
	}

	/* Génération aléatoires des poids pour la couche de sortie */
	for (int index_couche = NB_COUCHE_CACHEE; index_couche < (NB_COUCHE_CACHEE + 1); index_couche++) {
		for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_SORTIE; index_cell++) {
			for (int index_poids = 0; index_poids < NB_UNITE_COUCHE_CACHEE; index_poids++) {
				Perceptron1->vCouches[index_couche].vCells[index_cell].vWeights[index_poids] = generate_random_float(WEIGHT_MIN, WEIGHT_MAX);
			}
		}
	}

	/* Initialisation aléatoire des thetas des couches cachées */
	for (int index_couche = 0; index_couche < NB_COUCHE_CACHEE; index_couche++) {
		for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_CACHEE; index_cell++) {
			Perceptron1->vCouches[index_couche].vCells[index_cell].theta = generate_random_float(THETA_MIN, THETA_MAX);

		}
	}

	/* Initialisation aléatoire des thetas de la couche de sortie */
	for (int index_couche = NB_COUCHE_CACHEE; index_couche < (NB_COUCHE_CACHEE + 1); index_couche++) {
		for (int index_cell = 0; index_cell < NB_UNITE_COUCHE_SORTIE; index_cell++) {
			Perceptron1->vCouches[index_couche].vCells[index_cell].theta = generate_random_float(THETA_MIN, THETA_MAX);


		}
	}
}
/**************************************************************************************************
* write_weight
*
* @brief Fonction qui écrit dans un fichier la valeur de tous les poids des couches
*
* @parameters : 	-L'adresse du Perceptron
*
* @return     :		Void
**************************************************************************************************/
void write_weight(perceptron *Perceptron1)
{

	ofstream monFlux("SavedData.txt");

	if (monFlux)
	{
		/* Boucle qui parcours toutes les couches */
		for (int couche_index = 0; couche_index < Perceptron1->vCouches.size(); couche_index++)
		{
			/* Boucle qui parcours toutes les cellules de la couche */
			for (int cell_index = 0; cell_index < Perceptron1->vCouches[couche_index].vCells.size(); cell_index++)
			{
				cell* CurrentCell = &Perceptron1->vCouches[couche_index].vCells[cell_index];

				monFlux << CurrentCell->theta << " ";

				for (int weights_index = 0; weights_index < CurrentCell->vWeights.size(); weights_index++) //Met les poids d'une meme cellu sur une ligne
				{
					monFlux << CurrentCell->vWeights[weights_index] << " ";
				}		
			}
		}
		monFlux << endl;
	}
}
/**************************************************************************************************
* read_weight
*
* @brief Fonction qui lis dans un fichier la valeur de tous les poids des couches
*
* @parameters : 	-L'adresse du Perceptron
*
* @return     :		Void
**************************************************************************************************/
void read_weights(perceptron *Perceptron1)
{
	std::string ligne;
	std::string s_valeur;
	std::ifstream file_weights("SavedData.txt"); // ouvre fichier
	long charactercount = 0;
	int WatchDogLine = 0;

	
	if (file_weights.is_open()) 
	{				// vérifie l'ouverture du fichier
	
		while(file_weights.peek() != EOF || charactercount >= 1000000)
		{
			charactercount++;

			/* Boucle qui parcours toutes les couches */
			for (int couche_index = 0; couche_index < Perceptron1->vCouches.size(); couche_index++)
			{
				/* Boucle qui parcours toutes les cellules de la couche */
				for (int cell_index = 0; cell_index < Perceptron1->vCouches[couche_index].vCells.size(); cell_index++)
				{
					cell* CurrentCell = &Perceptron1->vCouches[couche_index].vCells[cell_index];

					int max_weight = CurrentCell->vWeights.size();

					file_weights >> s_valeur;
					if (s_valeur != "")
						CurrentCell->theta = std::stof(s_valeur, &sz);
					s_valeur.clear();


					for (int weights_index = 0; weights_index < CurrentCell->vWeights.size(); weights_index++) //Met les poids d'une ligne dans une cellule
					{
						file_weights >> s_valeur;
						if(s_valeur != "")
						CurrentCell->vWeights[weights_index] = std::stof(s_valeur, &sz);
						s_valeur.clear();
					}
				}
			}
		}

	  file_weights.close();
	}
}