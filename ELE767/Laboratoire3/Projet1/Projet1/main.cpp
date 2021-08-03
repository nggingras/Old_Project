#include "main.h"
#include"classe.h"
#include"weight.h"


void LVQ_algo(std::vector<double> vInput, std::vector<classe>& classe1);
int find_min(std::vector<double> resultat_distance);
int find_min_classe(std::vector<classe>& classe1);
std::vector<double> Soustraction_vecteur(std::vector<double> vInput, std::vector<double> vWeight);
std::vector<double> Multiplication_vecteur(const float scalar, std::vector<double> vWeight);
std::vector<double> addition_vecteur(std::vector<double> vInput, std::vector<double> vWeight);
std::vector<double> Soustraction_vecteur_poids(std::vector<double> vWeight1, std::vector<double> vWeight2);
std::vector<double> addition_vecteur_poids(std::vector<double> vWeight1, std::vector<double> vWeight2);
std::string supprime(std::string texte, std::string  x);
void read_file(std::vector<double> *vClassedData_tab, int file_to_read);
void assignation_poids(classe* classe, std::vector<double>* vClassedData_tab, int NB_PROTOTYPE, int TAILLE_PROTOTYPE, bool RandomData);
void LVQ_algo_perf(std::vector<double> vInput, std::vector<classe>& classe1, int donne_en_test);
void init_param();
void write_weight(std::vector<classe>& vClasse);
void read_weights(std::vector<classe>& vClasse);

//float valeur_ETHA = 0;
float valeur_init[NB_LIGNE_FICHIER_INIT];

const int NB_PROTOTYPE = valeur_init[0];
const int TAILLE_PROTOTYPE = valeur_init[1];
const int NB_VARIABLE_ENTREE = ((valeur_init[2] * 12) + 1);
int NB_EPOC = valeur_init[3];
float ETA;
const int Learn = valeur_init[5]; // Choix si on apprend ou on load des poids
float pente_eta = valeur_init[6];

//Struct pour le calcul des performances
struct perfo 
{
	int compteur_train;
	int compteur_VC;
	int compteur_test;

	double perfo_train;
	double perfo_VC;
	double perfo_test;

};

struct perfo performance;

void main(void)
{
	init_param();
	
	//Lecture des donn�es du fichier Init
	const int NB_PROTOTYPE = valeur_init[0];
	const int TAILLE_PROTOTYPE = valeur_init[1];
	const int NB_VARIABLE_ENTREE = ((valeur_init[2] * 12) + 1);
	int NB_EPOC = valeur_init[3];
	float ETHA = valeur_init[4];
	ETA = ETHA; // Met le ETHA dans la variable global ETA
	const int Learn = valeur_init[5];
	float pente_eta = valeur_init[6];
	


	//Cr�ation des 10 classes pour les 10 chiffres des donn�es
	//nb poids par classe, nb de poids, classe
	classe classe0(NB_PROTOTYPE, TAILLE_PROTOTYPE, 0);
	classe classe1(NB_PROTOTYPE, TAILLE_PROTOTYPE, 1);
	classe classe2(NB_PROTOTYPE, TAILLE_PROTOTYPE, 2);
	classe classe3(NB_PROTOTYPE, TAILLE_PROTOTYPE, 3);
	classe classe4(NB_PROTOTYPE, TAILLE_PROTOTYPE, 4);
	classe classe5(NB_PROTOTYPE, TAILLE_PROTOTYPE, 5);
	classe classe6(NB_PROTOTYPE, TAILLE_PROTOTYPE, 6);
	classe classe7(NB_PROTOTYPE, TAILLE_PROTOTYPE, 7);
	classe classe8(NB_PROTOTYPE, TAILLE_PROTOTYPE, 8);
	classe classe9(NB_PROTOTYPE, TAILLE_PROTOTYPE, 9);

	// Sert � regrouper les 10 classes dans un vecteur classe
	std::vector<classe> vClasse;

	//Tableaux de vecteurs pour la lecture des fichiers de donn�es
	static	std::vector<double> vClassedData_tab[NB_LIGNE_FICHIER];
	static	std::vector<double> vClassedData_tab_vc[NB_LIGNE_FICHIER_VC];
	static	std::vector<double> vClassedData_tab_test[NB_LIGNE_FICHIER_TEST];
	
	bool RandomData = false;

	while (1)
	{
		// Lecture des fichiers de donn�es
		read_file(vClassedData_tab, 1);      // Read train 
		read_file(vClassedData_tab_test, 2); // Read VC
		read_file(vClassedData_tab_test, 3); // Read Test


		if (Learn) // Si on veut seulement loader des poids
		{

				//Assignation des poids al�atoirement pour les classes
			assignation_poids(&classe0, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe1, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe2, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe3, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe4, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe5, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe6, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe7, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe8, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			assignation_poids(&classe9, vClassedData_tab, NB_PROTOTYPE, TAILLE_PROTOTYPE, RandomData);
			
			//Insertion des classes dans le vecteur de classe (param�tre de la fonction Algo
			vClasse.push_back(classe0);
			vClasse.push_back(classe1);
			vClasse.push_back(classe2);
			vClasse.push_back(classe3);
			vClasse.push_back(classe4);
			vClasse.push_back(classe5);
			vClasse.push_back(classe6);
			vClasse.push_back(classe7);
			vClasse.push_back(classe8);
			vClasse.push_back(classe9);
		}
		else
		{
			//Insertion des classes dans le vecteur de classe (param�tre de la fonction Algo
			vClasse.push_back(classe0);
			vClasse.push_back(classe1);
			vClasse.push_back(classe2);
			vClasse.push_back(classe3);
			vClasse.push_back(classe4);
			vClasse.push_back(classe5);
			vClasse.push_back(classe6);
			vClasse.push_back(classe7);
			vClasse.push_back(classe8);
			vClasse.push_back(classe9);

			read_weights(vClasse);
			NB_EPOC = 1;
		}
		/***************************************************** Apprentissage ****************************************************************************/
		for (int epoch_index = 0; epoch_index < NB_EPOC; epoch_index++)
		{
			//R�duit le ETA si on est � la deuxi�me it�ration
			if (epoch_index > 0)
				ETA -= pente_eta;
			
			printf("%f\n", ETA);
			//Arr�te l'apprentissage si on est rendu � ETA == 0
			if (ETA <= 0)
			{
				epoch_index = (NB_EPOC - 1);
			}
			else
			{
				performance.compteur_train = 0;
				for (int index_input = 0; index_input < NB_LIGNE_FICHIER; index_input++)
				{
					LVQ_algo(vClassedData_tab[index_input], vClasse);
				}
			}
		}
		write_weight(vClasse);
		/***************************************************** Test sur les donn�es VC ****************************************************************************/
		for (int index_input = 0; index_input < NB_LIGNE_FICHIER_VC; index_input++)
		{
			LVQ_algo_perf(vClassedData_tab[index_input], vClasse, 1);
		}

		/***************************************************** Test sur les donn�es test ****************************************************************************/
		for (int index_input = 0; index_input < NB_LIGNE_FICHIER_TEST; index_input++)
		{
			LVQ_algo_perf(vClassedData_tab[index_input], vClasse, 2);
		}


		performance.perfo_train = ( (double)performance.compteur_train / NB_LIGNE_FICHIER)*100;
		performance.perfo_VC    = ((double)performance.compteur_VC / NB_LIGNE_FICHIER_VC)*100;
		performance.perfo_test  = ((double)performance.compteur_test / NB_LIGNE_FICHIER_TEST)*100;

		printf(" la performance de train est de %f\n", performance.perfo_train);
		printf(" la performance de la VC est de %f\n", performance.perfo_VC);
		printf(" la performance des donnees de test est de %f\n", performance.perfo_test);
		system("pause");

	}
}


/***************************************************************************************************
 * assignation_poids
 *
 * @brief Fonction qui assigne des valeurs au poids. 
 *		  On copie les entr�es pig�es al�atoirement dans les 1340 entr�es
 * @parameters : 	classe classe : une classe
 * 					std::vector<double>*  : vecteurs des donn�es tri�s
 *					int NB_PROTOTYPE      : indique le nombre de classe
 *					int TAILLE_PROTOTYPE  : indique le nombre de poids 
 *
 * @return :	
 **************************************************************************************************/
void assignation_poids(classe *classe, std::vector<double>* vClassedData_tab, int NB_PROTOTYPE, int TAILLE_PROTOTYPE, bool RandomData)
{
	int random_num = 1;
	
	for (int index_poids = 0; index_poids < NB_PROTOTYPE; index_poids++)
	{
		if (RandomData)
		{
			random_num = (rand() % 1339) + 1;
		}
		else
		{
			random_num = index_poids + (classe->mNbWeight * classe->mClasse_nb);
		}

		for (int index_poids_valeur = 0; index_poids_valeur < TAILLE_PROTOTYPE; index_poids_valeur++)
		{
			classe->vWeight_list[index_poids].vWeight[index_poids_valeur] = vClassedData_tab[random_num][index_poids_valeur + 1];
			//printf("\nclasse %d,\npoids numero:%d,\nposition:%d,\nvaleur:%f\n", classe->mClasse_nb, index_poids, index_poids_valeur, classe->vWeight_list[index_poids].vWeight[index_poids_valeur]);
		}
	}
}

/***************************************************************************************************
 * calcul_distance
 *
 * @brief Fonction qui calcul la distance entre deux points
 *
 * @parameters : 	std::vector<double> : Le vecteur d'entr�e
 * 					std::vector<double> : Un vecteur poids
 *
 * @return :	Retourne la distance entre les deux vecteurs.
 **************************************************************************************************/
double calcul_distance(std::vector<double> vInput, std::vector<double> vWeight)
{
	double distance = 0;

	//Commence � 1, car la premi�re valeur du vInput est la classe
	for (int vector_size = 1; vector_size < (vWeight.size()+1); vector_size++)
	{
		// Calcul de la distance
		// (vInput[0] - vWeight[0])^2 + (vInput[1] - vWeight[1])^2 + (vInput[2] - vWeight[2])^2 ... 
		distance += pow((vInput[vector_size] - vWeight[vector_size-1]), 2.0);
	}
	return (distance);
}
/****************************************************************************
* find_min
*
* @brief : Fonction qui d�termine le r�sultat le plus petit parmi le tableau d'entr�e
*
* @parameters: vecteur � parcourir pour d�terminer la valeur la plus petite
*
* @return:  la position dans le tableau/vecteur de la valeur minimum trouv�
****************************************************************************/
int find_min(std::vector<double> resultat_distance)
{
	double resultat = resultat_distance[0];
	int position = 0;

	for (int index = 0; index < resultat_distance.size(); index++)
	{
		resultat = std::min(resultat, resultat_distance[index]); //cherche le min entre 2 valeurs du tableau        

		if (resultat == resultat_distance[index])
		{
			position = index;
		}
	}

	return (position);
}
/****************************************************************************
* find_min_class
*
* @brief : Fonction qui d�termine le r�sultat le plus petit parmi le tableau d'entr�e
*
* @parameters: tableau de class � parcourir pour d�terminer la valeur la plus petite
*
* @return:  la position dans le tableau/vecteur de la valeur minimum trouv�
****************************************************************************/
int find_min_classe(std::vector<classe> &classe1)
{
	classe resultat(classe1[0].mNbWeight, classe1[0].mTaille, classe1[0].mClasse_nb); 
	resultat = classe1[0];

	int position = 0;

	for (int index = 0; index < classe1.size(); index++)
	{
		resultat.mMinValue = std::min(resultat.mMinValue, classe1[index].mMinValue); //cherche le min entre 2 valeurs du tableau        

		if (resultat.mMinValue == classe1[index].mMinValue)
		{
			position = index;
		}
	}

	return (position);
}
/***************************************************************************************************
 * Algo
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : 
 * 					 : 
 *
 * @return :	
 **************************************************************************************************/
void LVQ_algo(std::vector<double> vInput, std::vector<classe> &classe1)
{


	std::vector<double> vResult_distance;
	int min_value_pos = 0;

	//Parcours toutes les classes dans le tableau de classe
	for (int class_index = 0; class_index < classe1.size(); class_index++)
	{

		vResult_distance.clear();

			
			// Pour chaque poids d'une classe
			for (int Weight_list_index = 0; Weight_list_index < classe1[class_index].vWeight_list.size(); Weight_list_index++)
			{

				vResult_distance.push_back(calcul_distance(vInput, classe1[class_index].vWeight_list[Weight_list_index].vWeight));

			}

			//Trouve la valeur minimum dans une classe de poids
			classe1[class_index].mMinVal_pos = find_min(vResult_distance);
			classe1[class_index].mMinValue = vResult_distance[classe1[class_index].mMinVal_pos];
		}

		//Trouve la valeur minimum, des valeurs minimum pour toutes les classes
		min_value_pos = find_min_classe(classe1);

		//Section qui ajuste le poids selon les r�sultats 
		if (vInput[0] == classe1[min_value_pos].mClasse_nb)
		{
			// w(new) = w(old) + ETA*(x-w)
			classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight = addition_vecteur_poids(classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight, Multiplication_vecteur(ETA, Soustraction_vecteur(vInput, classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight)));

			//Compteur pour les performance
			performance.compteur_train += 1;

		}
		else
		{

			// w(new) = w(old) + ETA*(x-w)
			classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight = Soustraction_vecteur_poids(classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight, Multiplication_vecteur(ETA, Soustraction_vecteur(vInput, classe1[min_value_pos].vWeight_list[classe1[min_value_pos].mMinVal_pos].vWeight)));

		}

	
}
/***************************************************************************************************
 * Algo_perf
 *
 * @brief Fonction qui calcul les performances seulement
 *
 * @parameters : 	 :
 * 					 :
 *
 * @return :
 **************************************************************************************************/
void LVQ_algo_perf(std::vector<double> vInput, std::vector<classe>& classe1, int donne_en_test)
{


	std::vector<double> vResult_distance;
	int min_value_pos = 0;

	//Parcours toutes les classes dans le tableau de classe
	for (int class_index = 0; class_index < classe1.size(); class_index++)
	{

		vResult_distance.clear();


		// Pour chaque poids d'une classe
		for (int Weight_list_index = 0; Weight_list_index < classe1[class_index].vWeight_list.size(); Weight_list_index++)
		{

			vResult_distance.push_back(calcul_distance(vInput, classe1[class_index].vWeight_list[Weight_list_index].vWeight));

		}

		//Trouve la valeur minimum dans une classe de poids
		classe1[class_index].mMinVal_pos = find_min(vResult_distance);
		classe1[class_index].mMinValue = vResult_distance[classe1[class_index].mMinVal_pos];
	}

	//Trouve la valeur minimum, des valeurs minimum pour toutes les classes
	min_value_pos = find_min_classe(classe1);

	if (donne_en_test == 1) //Test de donn�es de VC
	{
		if (vInput[0] == classe1[min_value_pos].mClasse_nb)
		{
			performance.compteur_VC += 1;
		}
	}
	else //Test de donn�es en test
	{
		if (vInput[0] == classe1[min_value_pos].mClasse_nb)
		{
			performance.compteur_test += 1;
		}
	}


}
/***************************************************************************************************
 * Soustraction_vecteur
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : 2 vecteurs de m�mes dimensions � soustraire
 * 					 :
 *
 * @return : Le vecteur r�sultant
 **************************************************************************************************/
std::vector<double> Soustraction_vecteur(std::vector<double> vInput, std::vector<double> vWeight)
{

	std::vector<double> Result;

	//D�but � 1, car vInput[0] est la classe
	for (int index = 1; index < (vWeight.size()+1); index++)
	{

		Result.push_back(vInput[index] - vWeight[index-1]);

	}

	return (Result);
}
/***************************************************************************************************
 * Soustraction_vecteur_poids
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : 2 vecteurs de m�mes dimensions � soustraire
 * 					 :
 *
 * @return : Le vecteur r�sultant
 **************************************************************************************************/
std::vector<double> Soustraction_vecteur_poids(std::vector<double> vWeight1, std::vector<double> vWeight2)
{

	std::vector<double> Result;

	//D�but � 1, car vInput[0] est la classe
	for (int index = 0; index < vWeight1.size(); index++)
	{

		Result.push_back(vWeight1[index] - vWeight2[index]);

	}

	return (Result);
}
/***************************************************************************************************
 * Multiplication_vecteur
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : vecteur � mulitplier
 * 					 : le scalaire multiplicateur
 *
 * @return : Le vecteur r�sultant
 **************************************************************************************************/
std::vector<double> Multiplication_vecteur(const float scalar, std::vector<double> vWeight)
{

	std::vector<double> Result;

	for (int index = 0; index < vWeight.size(); index++)
	{

		Result.push_back(scalar * vWeight[index]);

	}

	return (Result);
}
/***************************************************************************************************
 * Addition_vecteur
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : vecteur � additionner
 * 					 : 
 *
 * @return : Le vecteur r�sultant
 **************************************************************************************************/
std::vector<double> addition_vecteur(std::vector<double> vInput, std::vector<double> vWeight)
{

	std::vector<double> Result;

	//D�but � 1, car vInput[0] est la classe
	for (int index = 1; index < vWeight.size(); index++)
	{

		Result.push_back(vInput[index] + vWeight[index-1]);

	}

	return (Result);
}
/***************************************************************************************************
 * addition_vecteur_poids
 *
 * @brief Fonction qui effectue l'algorithme LVQ
 *
 * @parameters : 	 : 2 vecteurs de m�mes dimensions � soustraire
 * 					 :
 *
 * @return : Le vecteur r�sultant
 **************************************************************************************************/
std::vector<double> addition_vecteur_poids(std::vector<double> vWeight1, std::vector<double> vWeight2)
{

	std::vector<double> Result;

	//D�but � 1, car vInput[0] est la classe
	for (int index = 0; index < vWeight1.size(); index++)
	{

		Result.push_back(vWeight1[index] + vWeight2[index]);

	}

	return (Result);
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

	if (file_init) {				// v�rifie l'ouverture du fichier
		printf("Affichage des donnees du fichier Init \n");
		for (int i = 0; i < NB_LIGNE_FICHIER_INIT; i++) {
			if (i != 4 && i!= 6) {
				getline(file_init, ligne);	                  // lire une ligne
				std::cout << ligne << std::endl;              // print la ligne
				s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
				valeur_init[i] = std::stoi(s_valeur);         // convertis string en integer
				//std::cout << valeur_init[i] << std::endl;   // print la valeur
			}
			else if (i == 4)
			{
				getline(file_init, ligne);	                  // lire une ligne
				std::cout << ligne << std::endl;              // print la ligne
				s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
				valeur_init[i] = std::stof(s_valeur);            // convertis string en float
				//std::cout << valeur_ETHA << std::endl;      // print la valeur
			}
			else if (i == 6)
			{
				getline(file_init, ligne);	                  // lire une ligne
				std::cout << ligne << std::endl;              // print la ligne
				s_valeur = ligne.substr(ligne.find(":") + 1); // trouve ':' et sauvegarde ce qui suit
				valeur_init[i] = std::stof(s_valeur);            // convertis string en float
				//std::cout << valeur_ETHA << std::endl;      // print la valeur
			}
		}
		file_init.close();
	}

}
/***************************************************************************************************
* read_train
*
* @brief Fonction qui lit le fichier de donn�es et retourne les donn�es class�es en ordre croissant d'�nergie 
*		dans un tableau de vecteur
*
* @parameters : 	Void
*
* @return     :	 tableau de vecteur 
**************************************************************************************************/
void read_file(std::vector<double> *vClassedData_tab, int file_to_read)
	{

		if (file_to_read == 1)
		{

			using namespace std;

			static	double FileRead[NB_LIGNE_FICHIER][NB_DONNEE + 1];
			static	std::vector<double> vUnclassedData_tab[NB_LIGNE_FICHIER];
			static	std::vector<double> vUnclassedData;
			static	std::vector<double> vClassedData;

			std::string sDeuxPoints = ":";
			std::string sRead;
			std::string::size_type sz;

			ifstream monFlux("data_train.txt");

			/* V�rifie l'ouverture du fichier */
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

			/* Boucle qui met � 0 toutes les valeurs dynamiques */
			int compteur = 0;
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER; nb_ligne++)
			{
				compteur = 0;
				for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
					/*
					1er donn�e du vecteur : nombre dit
					2-13 : donn�es statiques
					14 : donn�e �nergie statique
					15-26 : donn�es dynamique
					27: donn�e �nergie dynamique
					On veut retirer les donn�es 15-27
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
					/* Boucle qui trouve la donn�e d'�nergie la plus petite en ne lisant pas la premi�re donn�e qui est le chiffre lu*/
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
					/* Met � 100 l'�nergie qu'on ne sauvegarde pas */
					vUnclassedData_tab[nb_ligne][sauvegarde_index + 12] = 100;

				}
				vClassedData_tab[nb_ligne] = vClassedData;
			}
		}
		if (file_to_read == 2)
		{

			using namespace std;

			static	double FileRead[NB_LIGNE_FICHIER_VC][NB_DONNEE + 1];
			static	std::vector<double> vUnclassedData_tab[NB_LIGNE_FICHIER_VC];
			static	std::vector<double> vUnclassedData;
			static	std::vector<double> vClassedData;

			std::string sDeuxPoints = ":";
			std::string sRead;
			std::string::size_type sz;

			ifstream monFlux("data_vc.txt");

			/* V�rifie l'ouverture du fichier */
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
						FileRead[nb_ligne][donnee] = std::stod(sRead, &sz);
					}
				}
			}

			/* Boucle qui met � 0 toutes les valeurs dynamiques */
			int compteur = 0;
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
			{
				compteur = 0;
				for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
					/*
					1er donn�e du vecteur : nombre dit
					2-13 : donn�es statiques
					14 : donn�e �nergie statique
					15-26 : donn�es dynamique
					27: donn�e �nergie dynamique
					On veut retirer les donn�es 15-27
					*/
				{
					if (compteur > 13)
						FileRead[nb_ligne][donnee] = 100;
					compteur %= 26;
					compteur++;
				}
			}
			/* Boucle qui remplie le vecteur des donnees statiques restantes */
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
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

			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_VC; nb_ligne++)
			{
				vClassedData.clear();
				for (trame_place = 0; trame_place < 60; trame_place++)
				{
					done = 0;
					valeur_energie = -100;
					/* Boucle qui trouve la donn�e d'�nergie la plus petite en ne lisant pas la premi�re donn�e qui est le chiffre lu*/
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
					/* Met � 100 l'�nergie qu'on ne sauvegarde pas */
					vUnclassedData_tab[nb_ligne][sauvegarde_index + 12] = 100;

				}
				vClassedData_tab[nb_ligne] = vClassedData;
			}
		}

		if (file_to_read == 3)
		{

			using namespace std;

			static	double FileRead[NB_LIGNE_FICHIER_TEST][NB_DONNEE + 1];
			static	std::vector<double> vUnclassedData_tab[NB_LIGNE_FICHIER_TEST];
			static	std::vector<double> vUnclassedData;
			static	std::vector<double> vClassedData;

			std::string sDeuxPoints = ":";
			std::string sRead;
			std::string::size_type sz;

			ifstream monFlux("data_test.txt");

			/* V�rifie l'ouverture du fichier */
			if (monFlux)
			{
				/* Lecture d'une string dans le fichier, pour toute le fichier */
				for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
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

			/* Boucle qui met � 0 toutes les valeurs dynamiques */
			int compteur = 0;
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
			{
				compteur = 0;
				for (int donnee = 0; donnee <= NB_DONNEE; donnee++)
					/*
					1er donn�e du vecteur : nombre dit
					2-13 : donn�es statiques
					14 : donn�e �nergie statique
					15-26 : donn�es dynamique
					27: donn�e �nergie dynamique
					On veut retirer les donn�es 15-27
					*/
				{
					if (compteur > 13)
						FileRead[nb_ligne][donnee] = 100;
					compteur %= 26;
					compteur++;
				}
			}
			/* Boucle qui remplie le vecteur des donnees statiques restantes */
			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
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

			for (int nb_ligne = 0; nb_ligne < NB_LIGNE_FICHIER_TEST; nb_ligne++)
			{
				vClassedData.clear();
				for (trame_place = 0; trame_place < 60; trame_place++)
				{
					done = 0;
					valeur_energie = -100;
					/* Boucle qui trouve la donn�e d'�nergie la plus petite en ne lisant pas la premi�re donn�e qui est le chiffre lu*/
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
					/* Met � 100 l'�nergie qu'on ne sauvegarde pas */
					vUnclassedData_tab[nb_ligne][sauvegarde_index + 12] = 100;

				}
				vClassedData_tab[nb_ligne] = vClassedData;
			}
		}

	}
/***************************************************************************************************
* read_vc
*
* @brief Fonction qui ouvre le fichier texte d'initialisation et qui va mettre les valeurs lues dans un tableau
*
* @parameters : 	Void
*
* @return     :	Void
**************************************************************************************************/

/***************************************************************************************************
* read_test
*
* @brief Fonction qui ouvre le fichier texte d'initialisation et qui va mettre les valeurs lues dans un tableau
*
* @parameters : 	Void
*
* @return     :	Void
**************************************************************************************************/

/***************************************************************************************************
* supprime
*
* @brief Fonctione qui trouve un caract�re dans un variable de type string et qui supprime ce caract�re
*		  en r��crivant la string au complet sans le caract�re
*
* @parameters : 	-String dans laquelle le caract�re doit �tre trouv�
*					-Caract�re � trouver et � supprim�
*
* @return     :	Retourne la nouvelle string sans le caract�re
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
/**************************************************************************************************
* write_weight
*
* @brief Fonction qui �crit dans un fichier la valeur de tous les poids des couches
*
* @parameters : 	-L'adresse du Perceptron
*
* @return     :		Void
**************************************************************************************************/
void write_weight(std::vector<classe>& vClasse)
{

	std::ofstream monFlux("SavedData.txt");

	if (monFlux)
	{
		/* Boucle qui parcours toutes les classes */
		for (int classe_index = 0; classe_index < vClasse.size(); classe_index++)
		{
			/* Boucle qui parcours toutes les poids d'une classe */
			for (int weightlist_index = 0; weightlist_index < vClasse[classe_index].vWeight_list.size(); weightlist_index++)
			{
				for (int weights_index = 0; weights_index < vClasse[classe_index].vWeight_list[weightlist_index].vWeight.size(); weights_index++) //Met les poids d'une meme cellu sur une ligne
				{
						monFlux << vClasse[classe_index].vWeight_list[weightlist_index].vWeight[weights_index] << " ";
				}
			}
		}
		monFlux << std::endl;
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
void read_weights(std::vector<classe>& vClasse)
{
	std::string ligne;
	std::string s_valeur;
	std::string::size_type sz;
	std::ifstream file_weights("SavedData.txt"); // ouvre fichier
	long charactercount = 0;
	int WatchDogLine = 0;

	if (file_weights.is_open())
	{				// v�rifie l'ouverture du fichier

		while (file_weights.peek() != EOF || charactercount >= 1000000)
		{

			charactercount++;

			/* Boucle qui parcours toutes les classes */
			for (int classe_index = 0; classe_index < vClasse.size(); classe_index++)
			{
				/* Boucle qui parcours toutes les poids d'une classe */
				for (int weightlist_index = 0; weightlist_index < vClasse[classe_index].vWeight_list.size(); weightlist_index++)
				{
					for (int weights_index = 0; weights_index < vClasse[classe_index].vWeight_list[weightlist_index].vWeight.size(); weights_index++) //Met les poids d'une meme cellu sur une ligne
					{
						file_weights >> s_valeur;
						if (s_valeur != "")
							vClasse[classe_index].vWeight_list[weightlist_index].vWeight[weights_index] = std::stof(s_valeur, &sz);
						s_valeur.clear();

					}
				}
			}
		
		}

		file_weights.close();
	}
}