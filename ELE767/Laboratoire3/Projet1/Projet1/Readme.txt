Le code se veut structuré principalement en trois partie.

1. Dans un premier temps, le code passe et effectue la lecture de tous les fichiers de données, soit Train, VC et test. Les données 
statiques de ces fichiers sont sauvegardés dans un tableau de vecteurs en ordre décroissant de valeurs d'énergie. Cette méthode
nous permet de choisir la taille du vecteur d'entrée et de prendre seulement les données nécessaires au traitement.

2. Dans un deuxième temps, l'algorithme effectue les itérations d'apprentissage des données train, selon la configuration du réseau choisie 
dans le fichier Init.txt. 

3. Finalement, la phase 1 de l'algorithme d'apprentissage est refait deux fois, soit avec les données VC et les données. Par contre, les derniers poids
calculés durant l'apprentissage sont utilisés afin de déterminer la performance sur les trois bases de données. La meilleur performance est enregistré 
dans le fichier SavedData. Ce fichier SavedData permet de reprendre l'algorithme à partir de données obtenues dans un apprentissage précédent, à
l'aide de la variable LEARN du fichier Init.

Il y a deux exceptions importantes dans la sélection des valeurs dans le fichier init. Tout d'abord, la taille des représantants doivent toujours
être de même taille que le vecteur d'entrée. Ceci est obligatoire du au principe même de l'algorithme LVQ. Par la suite, le ETA ne doit pas devenir
négatif. Il y aura donc un arrêt de l'apprentissage si le taux de réduction du taux d'apprentissage le réduit à 0.

Pour ce qui est des critères d'arrêt, nous avons soit le taux d'apprentissage à 0, ou le nombre maximal d'époque atteint dans le fichier Init.
