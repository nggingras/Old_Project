Le code se veut structuré principalement en trois partie.

1. Dans un premier temps, le code passe et effectue la lecture de tous les fichiers de données, soit Train, VC et test. Les données 
statiques de ces fichiers sont sauvegardés dans un tableau de vecteurs en ordre décroissant de valeurs d'énergie.

2. Dans un deuxième temps, le perceptron effectue les itérations d'apprentissage des données train, selon la configuration du réseau de neuronnes choisie 
dans le fichier Init.txt.

3. Finalement, la phase 1 de l'algorithme d'apprentissage est refait deux fois, soit avec les données VC et les données. Par contre, les derniers poids
calculés durant l'apprentissage sont utilisés afin de déterminer la performance sur les trois bases de données. La meilleur performance est enregistré 
dans le fichier 
