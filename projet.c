/**
 * @file projet.c
 * @author Eric Cortial et Dogmael Dartigue Peyrou
 * @brief Documentation du projet d'informatique
 * - indentation réalisée grâce à l'indentation automatique de l'extension CC++ de Microsoft dans Visual Studio Code.
 * - documentation réalisée grâce à l'extension Doxygen de Visual Studio Code.
 * - L'objectif de ce programme est de pouvoir résoudre tout type de sudoku (n^2 x n^2).
 * @version 1.4
 * @date 2019-04-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief La variable len correpond a la taille du tableau modélisant la grille de départ à résoudre.
 * 
 */
int len;

/**
 * @brief On définit la grille de sudoku par son paramètre n : telle que la taille de la grille soit égale à n^2 × n^2.
 * 
 */
int n;

/**
 * @brief La variable cotegrille correspond a la taille du cote de la grille, soit encore au nombre d'éléments dans une ligne, colonne, bloc.
 * 
 */
int cotegrille;

/**
 * @brief La variable cotebloc correspond a la taille du cote d'un bloc (aussi égale à n).
 * 
 */
int cotebloc;

/**
 * @brief La variable compteur_tentatives est une variable de trace. Elle va nous permettre de savoir combien de tentatives le programme a fait dans la résolution du sudoku.  
 * 
 */
int compteur_tentatives = 0;

/**
 * @brief Va nous permettre de retourner le nombre de tentatives depuis des fonctions.
 * 
 */
int *pointeur_sur_compteur_tentative = &compteur_tentatives;

/**
 * @brief Variable utilisée dans le programme nombre_valeurs_non_nulles.
 * 
 */
int valeur_non_nulle;

/**
 * @brief Chaine de caractères contenant le nom du fichier de départ à traiter.
 * Le résultat sera stocké dans un fichier portant le même nom avec une extension différente.
 * 
 */
char nom_du_fichier[255] = "";

//Fonctions demandées dans l'énoncé

/**
 * @brief Fonction affichant une grille de sudoku grace a un tableau a une dimension.
 * 
 * Cette fonction a été essentielle dans notre conception du code, en affichant différents résultats intermédiaires.
 * 
 * @param t : tableau de valeurs correspondant à la grille qu'on souhaite afficher.
 */
void print_grid(int t[]);

/**
 * @brief Fonction permettant de lire une grille de sudoku depuis un fichier et de la stocker en mémoire.
 * 
 * Il est possible de lire n'importe quel fichier compris dans le même dossier que notre programme ou un sous-dossier (ex: 9x9\sO1a.txt).
 * Le fichier ne doit pas être vide, et doit pouvoir remplir le tableau à une dimension.
 *  
 * @param t : tableau qui va recevoir le contenu du fichier.
 * @return int : Le programme retournera 1 si on a reussi à stocker le tableau dans le fichier .
 */
int read_grid(int t[]);

/**
 * @brief Fonction permettant d'écrire une grille de sudoku dans un fichier.
 * 
 * On rajoutera à notre fichier de départ une extension différente resultat.txt (ex: 9x9\sO1a.txt devient 9x9\sO1a.resultat.txt).
 * 
 * @param t : tableau de valeurs qu'on va ecrire dans un fichier. Un retour à la ligne est fait à chaque fois que l'on a écrit "coteligne" valeurs.
 */
void write_grid(int t[]);

/**
 * @brief Fonction qui va nous permettre de résoudre le sudoku.
 * 
 * Dans un premier temps : on vérifie que le sudoku peut etre résolu grâce à la fonction check_grid (voir plus loin).
 * 
 * Dans un second temps : on tente de remplir la grille par des "méthodes humaines" : on cherche à remplir des cases n'admettant qu'une seule solution.
 * On va parcourir notre tableau et s'arrêter sur chaque 0 (case vide), si une seule valeur est possible alors on l'écrit.
 * On ne parcourt plus notre tableau quand il ne reste plus de possibilitées.
 * 
 * Enfin : si les méthodes humaines n'ont pas permis de résoudre le sudoku entièrement, on resout par backtracking (voir énoncé).
 *  
 * @param t : tableau de valeurs correspondant au sudoku qu'on va résoudre.
 * @param valeurs_possibles : valeurs que le tableau a 2 dim (valeur_possible) a pu permettre de remplir (tableau de travail)
 * @return int : On retourne 0 a la fin de notre programme pour stopper l'éxécution : soit le sudoku est remplit grâce aux méthodes humaines et/ou au backtracking, soit la grille n'est pas résolvable et un message d'erreur s'affiche.
 */
int solve(int t[], int valeurs_possibles[len][cotegrille]); //

//Fonctions complémentaires

/**
 * @brief Fonction permettant de vérifier si un élément est present au moins 2 fois dans un même tableau.
 * 
 * Cette fonction nous est essentielle dans la conception de nos checks pour ligne, colonne et bloc.
 * 
 * @param tableau : tableau de valeurs qu'on veut verifier. Il pourra représenter une ligne, une colonne ou encore un bloc.
 * @param longueur_tableau : longueur de notre tableau : "len" comme cité précédemment. 
 * @return int : Si aucun élément n'est présent au moins 2 fois dans le tableau alors la fonction retourne 0, sinon elle retourne 1.
 */
int check_element(int tableau[], int longueur_tableau);

/**
 * @brief Fonction permettant de vérifier qu'une ligne est compatible ou non grâce à la fonction check_element.
 * 
 * On appelle cette fonction dans check_grid : pour vérifier qu'une grille est compatible il faut au moins vérifier que toute les lignes le sont.
 * 
 * @param t : tableau de valeurs symbolisant le sudoku. 
 * @param numero_ligne : Variable permettant de repérer la ligne où on se situe.
 * @return int : Retourne 1 si la ligne est compatible, 0 sinon.
 */
int check_ligne(int t[], int numero_ligne);

/**
 * @brief Fonction permettant de vérifier qu'une colonne est compatible ou non grâce à la fonction check_element.
 * 
 * On appelle cette fonction dans check_grid : pour vérifier qu'une grille est compatible il faut au moins vérifier que toute les colonnes le sont.
 * 
 * @param t : tableau de valeurs symbolisant le sudoku. 
 * @param numero_colonne : Variable permettant de reperer la colonne où on se situe.
 * @return int : Retourne 1 si la colonne est compatible, 0 sinon.
 */
int check_colonne(int t[], int numero_colonne);

/**
 * @brief Fonction permettant de vérifier qu'un bloc est compatible ou non grace à la fonction check_element.
 * 
 * On appelle cette fonction dans check_grid : pour vérifier qu'une grille est compatible il faut au moins vérifier que toute les blocs le sont.
 * 
 * @param t : tableau de valeurs symbolisant le sudoku.
 * @param numero_bloc : Variable permettant de repérer le bloc où on se situe.
 * @return int : Retourne 1 si le bloc est compatible, 0 sinon.
 */
int check_bloc(int t[], int numero_bloc);

/**
 * @brief Programme permettant de vérifier que la grille totale est compatible avec les règles du sudoku.
 * Cette vérification se fera grâce aux programmes permettant de vérifier nos lignes, colonnes, et blocs précédents.
 * 
 * @param t : tableau de valeurs symbolisant le sudoku.
 * @return int : Retourne 1 si la grille est compatible, 0 sinon.
 */
int check_grid(int t[]);

/**
 * @brief Fonction permettant de vérifier qu'une case d'un bloc est compatible ou non avec le reste de la grille. 
 * 
 * On appele check_element pour un bloc, une ligne et une colonne donnée : la combinaison de ces 3 conditions 
 * permet de certifier que la case en question est bonne, ou non.
 * 
 * On utilise cette fonction pour remplir la grille grâce au backtracking.
 * 
 * @param t : tableau de valeurs symbolisant le sudoku.
 * @param numero_element : Variable permettant de repérer l'indice de l'élément donné.
 * @return int : Retourne 1 si la case est compatible, 0 sinon.
 */
int check_case(int t[], int numero_element);

/**
 * @brief Fonction permettant de corriger les cases à remplir par la methode du backtracking. 
 * Consiste à revenir d'au moins une case en arrière pour la modifier par la suite.
 * 
 * @param t_save : Pour faciliter les modifications : on crée un "tableau de sauvegarde" sauvegardant le tableau qui memorise le sudoku avant les modifications par backtracking.
 * @param numero_case : Variable permettant de repérer l'indice de la case où on se situe.
 * @return int : Retourne le kieme élément a modifier, qui sera réutiliser dans la méthode du backtracking.
 */
int previous_modifiable(int t_save[], int numero_case);

/**
 * @brief Fonction permettant de repérer le prochain élément à modifier pour le backtracking.
 * 
 * @param t_save : Pour faciliter les modifications : on crée un "tableau de sauvegarde" sauvegardant le tableau qui mémorise le sudoku avant les modifications par backtracking.
 * @param numero_case : Variable permettant de repérer l'indice de la case où on se situe.
 * @return int : Retourne le kieme élément à modifier, qui sera réutilisé dans la méthode du backtracking.
 */
int next_modifiable(int t_save[], int numero_case);

/**
 * @brief Fonction permettant de renvoyer pour un élément donné un tableau de valeurs correspondant aux possibilitées de cette case.
 * On va repérer dans quelle ligne, colonne, et bloc l'élément se situe, puis on parcourt les valeurs trouvées dans chacun d'eux.
 * Si une valeur est déjà présente dans la ligne/colonne/bloc : on l'annule, sinon, elle sera affichée dans le résultat. 
 *   
 * @param t : tableau de départ.
 * @param valeurs_possibles : tableau servant à sauvegarder les possibilitées de chaque case vides de notre sudoku.
 * @param numero_element : Variable permettant de repérer l'indice de la case à traiter.
 */
void possibilite_case(int t[], int valeurs_possibles[len][cotegrille], int numero_element);

/**
 * @brief Fonction qui pour un élément donné crée un tableau ligne, colonne et bloc dans lequel il met les éléments qui correspondent à la case donnée.
 * 
 * @param t : tableau de départ.
 * @param numero_element : Variable permettant de repérer l'indice de l'élément.
 * @param ligne : tableau dans lequel on récupere les éléments de la ligne.
 * @param colonne : tableau dans lequel on récupere les éléments de la colonne.
 * @param bloc : tableau dans lequel on récupere les éléments du bloc.
 */
void get_case(int t[], int numero_element, int ligne[], int colonne[], int bloc[]);

/**
 * @brief Fonction permettant de renvoyer le nombre de valeurs non nulles de valeur_possible[numero_element][k] pour un k élément donné.
 * 
 * @param valeurs_possibles : tableau dans lequel on stocke les valeurs possibles pour chaque élément.
 * @param numero_element : Variable permettant de repérer l'indice de l'élément.
 * @return int : Retourne une variable "compteur" qui nous donnera le nombre de valeurs non nulles.
 */
int nombre_valeurs_non_nulles(int valeurs_possibles[len][cotegrille], int numero_element);

// Fonctions de travail (pour debugger)

/**
 * @brief Fonction annexe permettant d'afficher un tableau à une dimension.
 * 
 * @param t : tableau dans lequel on va ranger les valeurs.
 * @param len : taille du tableau
 */
void print_tableau(int t[], int len);

/**
 * @brief Fonction annexe permettant d'afficher un tableau a 2 dimensions.
 * 
 * @param valeurs_possibles : On incrémente dans notre main le tableau servant à sauvegarder les possibilitées de chaque case vides de notre sudoku.
 */
void print_tableau_deux_dim(int valeurs_possibles[len][cotegrille]);

/**
 * @brief LE MAIN INTERACTIF :
 * 
 * Tout d'abord : on demande à l'utilisateur d'entrer la taille de la grille souhaitée ce qui va permettre d'en déduire les autres variables : len, cotegrille, cotebloc.
 * 
 * Puis : La saisie du fichier souhaité, il est essentiel de vérifier que ce fichier est accessible.
 * 
 * On poursuit par la résolution : 
 * - début par une "méthode humaine" 
 * - et si ça ne suffit pas une autre par "backtracking" (les 2 sont affichées sauf si on avait remplit grâce à la méthode humaine).
 * 
 * Enfin : on laisse le choix à l'utilisateur de stocker le résultat du solve dans un fichier [nom_du_fichier]resultat.txt (grâce à la fonction strcat).
 * 
 * @return int : 0 quand l'éxécution s'achève.
 */
int main()
{
	printf("Entrez la taille de la grille (un sudoku classique est de taille 3) : \n");
	scanf("%d", &n);
	if (n < 2)
	{
		printf("Erreur : la taille de la grille est trop petite.\n");
		return 0;
	}
	else
	{
		len = pow(n, 4);
		cotegrille = pow(n, 2);
		cotebloc = n;
	}

	int t[len];
	int valeurs_possibles[len][cotegrille];
	for (int i = 0; i < len; i++)
	{ //On initialise valeurs_possibles
		int cpt = 1;
		for (int j = 0; j < cotegrille; j++)
		{
			valeurs_possibles[i][j] = cpt;
			cpt++;
		}
	}

	printf("Entrez le nom du fichier (avec son extension) :\n");
	scanf("%254s", &nom_du_fichier);

	if (read_grid(t) == 0)
	{
		return 0;
	}
	printf("La grille lue est : \n");
	print_grid(t);
	if (solve(t, valeurs_possibles) == 1)
	{
		return 0;
	}
	else
	{
		printf("La grille resolue est : \n");
		print_grid(t);
	}
	int ans;
	printf("Souhaitez-vous enregistrer le resultat dans un fichier texte ? (Oui : 1/ Non : 0) : \n");
	scanf("%d", &ans);
	if (ans == 0)
	{
		return 0;
	}
	else
	{
		char *token;
		token = strtok(nom_du_fichier, ".");
		*nom_du_fichier = *strcat(token, ".resultat.txt");
		printf("Le fichier a ete enregistre sous le nom : %s\n", nom_du_fichier);
		write_grid(t);
	}

	return 0;
}

void print_grid(int t[])
{
	for (int k = 0; k < len; k++)
	{
		printf("%3d", t[k]);
		if ((k + 1) % cotegrille == 0)
		{
			printf("\n");
		}
	}
}

int read_grid(int t[]) //Gerer la fichiers contenant trop ou pas asser de chiffres
{
	int tailleligne = 10 * cotegrille; //Il faut bien mettre ca (3*coter grille ne suffit pas)
	int position = 0;				   //Repere pour l'injection dans le tableau en parametre
	int i = 0;

	FILE *fichier = fopen(nom_du_fichier, "r");
	if (fichier == NULL)
	{
		printf("Impossible d'ouvrir le fichier");
		return 0;
	}
	else
	{
		while (i < cotegrille)
		{

			char ligne[tailleligne];
			fgets(ligne, tailleligne, fichier);
			char *token;
			int j = 0;
			token = strtok(ligne, "  ");
			while (j < cotegrille)
			{
				if (*token != 48 && atoi(token) == 0)
				{ //On verifie que atoi(token) ne soit pas superieur à coter grille.
					printf("Caractere incorrect.\n");
					return 0;
				}
				if (atoi(token) < 0 || atoi(token) > cotegrille) //On verifie qu'il n'y ait pas une valeur de case superieur à coter grille.
				{
					printf("Valeur d'une case incorrecte\n");
					return 0;
				}
				t[position] = atoi(token);
				position++;
				token = strtok(NULL, " ");
				j++;
			}
			i++;
		}
		fclose(fichier);
	}

	//On verfie que la grille ne soit pas pleine ou vide
	int marqueur_vide = 0;
	int marqueur_plein = 0;
	for (int k = 0; k < len; k++)
	{
		if (t[k] == 0)
		{ //la grille n'est pas déjà pleine
			marqueur_plein = 1;
		}
		if (t[k] != 0)
		{ //la grille n'est pas vide
			marqueur_vide = 1;
		}
		if (marqueur_vide == 1 && marqueur_plein == 1)
		{
			break;
		}
		if (k == len - 1 && marqueur_vide == 0)
		{
			printf("La grille est vide.");
			return 0;
		}
		if (k == len - 1 && marqueur_plein == 0)
		{
			printf("La grille est deja pleine.");
			return 0;
		}
	}
	return 1; // Tout s'est bien passe
}

void write_grid(int t[])
{

	FILE *fichier = NULL;

	fichier = fopen(nom_du_fichier, "w");

	if (fichier != NULL)
	{ //On verifie que vide.txt ait bien été chargé dans la variable fichier
		for (int k = 1; k <= len; k++)
		{
			fprintf(fichier, "%3d", t[k - 1]);
			if (k % (cotegrille) == 0)
			{
				fprintf(fichier, "\n");
			}
		}
		fclose(fichier);
	}
	else
	{
		printf("Le fichier n'a pas pu être sauvegardé.\n");
	}
}

int solve(int t[], int valeurs_possibles[len][cotegrille])
{
	// 1- On commence par vérifier si la sudoku peut être résolue.
	if (check_grid(t) == 0)
	{
		printf("La grille n'est pas compatible.\n");
		return 1;
	}

	// 2- On tente les méthodes humaines
	while (0 == 0)
	{
		for (int k = 0; k < len; k++)
		{
			if (t[k] != 0)
			{
				for (int i = 0; i < cotegrille; i++)
				{
					valeurs_possibles[k][i] = -1;
				}
			}
			else
			{
				possibilite_case(t, valeurs_possibles, k);
			}
		}
		int cpt = 0;
		for (int k = 0; k < len; k++)
		{
			if (nombre_valeurs_non_nulles(valeurs_possibles, k) == 1)
			{
				t[k] = valeur_non_nulle;
				cpt++;
			}
		}
		if (cpt == 0)
		{
			break;
		}
	}
	printf("resultat par methodes humaines :\n");
	print_grid(t);
	for (int i = 0; i < len; i++) //si grille resolue par methodes humaines on sort de solve.
	{
		if (t[i] == 0)
		{
			break;
		}
		if (i == len - 1)
		{
			return 0;
		}
	}
	printf("\nNous continuons par backtracking.\n");
	//On finit par un back_traking amelioré.

	//On commence par dubliqué le tableau donné en entré.
	int t_save[len];
	for (int k = 0; k <= len; k++)
	{
		t_save[k] = t[k];
	}

	// On prend le min entre 0 et next_modifiable(t_save, 0)
	int k;
	if (t[0] == 0)
	{
		k = 0;
	}
	else
	{
		k = next_modifiable(t_save, 0);
	}

	//On resout par backtracking
	while (k < len)
	{
		//On tente de  trouver une valeur qui fonctionne pour t[k]
		for (int i = t[k] + 1; i <= cotegrille; i++)
		{			  //On tente tout les valeurs pour t[k], si aucune n'est valide on remonte et k = previous_modifiable.
			t[k] = i; //On associe à notre terme la valeur de i.
			compteur_tentatives++;
			if (check_case(t, k) == 1)
			{
				k = next_modifiable(t_save, k); //Si grille reste compatible on passe a la case suivante
				if (k > (len - 1) || k < 0)
				{ //On arrête la boucle while si on est en dehors du tableau.
					return 0;
				}
				break;
			}
			else if (check_case(t, k) == 0 && i == cotegrille)
			{ //Si on a essayé tout les valeurs pour t[k] jusqu'à 9 et que ça ne fonctionne toujours pas on modifie la case précédente.
				while (t[k] == cotegrille)
				{			  //On va remonter juqu'à trouver une case qui n'est pas déja à 9 et on remet à zero toute celles qu'on rencontre en remontant.
					t[k] = 0; //On remet k à 0 avant de remonter dans le tableau
					k = previous_modifiable(t_save, k);
				}
			}
			if (compteur_tentatives % 10000000 == 0)
			{
				printf("compteur_tentatives = %d\n", compteur_tentatives);
				print_grid(t);
				printf("\n");
			}
		}
	}
	return 0;
}

int check_element(int tableau[], int longueur_tableau)
{
	int masque[cotegrille];
	for (int k = 0; k < cotegrille; k++)
	{
		masque[k] = 1;
	}
	for (int k = 0; k < longueur_tableau; k++)
	{
		if (tableau[k] == 0)
		{
			continue;
		}
		else if (masque[tableau[k] - 1] == 0)
		{ //On prend k-1 car dans notre masque la première case est numéroté 0, ce qui décale tout la numérotation.
			return 1;
		}
		else
		{
			masque[tableau[k] - 1] = 0;
		}
	}
	return 0;
}

int check_ligne(int t[], int numero_ligne)
{
	int ligne[cotegrille];
	int i = 0; //numérote les case du tableau ligne.

	//Dans une premier temps on recupères les élements d'une ligne dans un tableau.
	for (int k = (numero_ligne - 1) * cotegrille; k <= numero_ligne * cotegrille - 1; k++)
	{ //On parcourt tout les termes de la ligne
		ligne[i] = t[k];
		i++;
	}
	if (check_element(ligne, cotegrille) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int check_colonne(int t[], int numero_colonne)
{
	int colonne[cotegrille];
	int i = 0;
	for (int k = (numero_colonne - 1); k <= numero_colonne + (cotegrille * (cotegrille - 1)); k = k + cotegrille)
	{
		colonne[i] = t[k];
		i++;
	}
	if (check_element(colonne, cotegrille) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int check_bloc(int t[], int element_debut_bloc)
{ //le tableau bloc géneré n'est pas bon
	int bloc[cotegrille];
	int element_fin_bloc = element_debut_bloc + (cotebloc - 1) * cotegrille + cotebloc - 1; //bien généralisé
	int curseur = 0;																		//Il sert à récuperer ou sommes nous dans le tableau bloc[]
	for (int k = element_debut_bloc; k < element_fin_bloc; k = k + cotegrille)
	{
		for (int i = 0; i < cotebloc; i++)
		{
			bloc[curseur] = t[k + i];
			curseur++;
		}
	}
	if (check_element(bloc, cotegrille) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int check_grid(int t[])
{

	//On verifie que les lignes sont compatibles.
	for (int numero_ligne = 1; numero_ligne <= cotegrille; numero_ligne++)
	{ //On fait une boucle pour verifier toutes les lignes, elles sont numérotées de 1 à coter grille.
		if (check_ligne(t, numero_ligne) == 0)
		{
			printf("Erreur de ligne\n"); //A supprimer pour la version final car pas fiable.
			return 0;
		}
		else
		{
			continue;
		}
	}

	// On verifie que les colonnes sont compatibles.
	for (int numero_colonne = 1; numero_colonne <= cotegrille; numero_colonne++)
	{ //On sait que les éléments de la nieme colonne sont congrus a (n - 1) mod 9
		if (check_colonne(t, numero_colonne) == 0)
		{
			printf("Erreur de colonne\n"); //A supprimer pour la version final car pas fiable.
			return 0;
		}
		else
		{
			continue;
		}
	}

	// On verifie que les blocs sont compatibles.
	for (int element_debut_bloc = 0; element_debut_bloc < len; element_debut_bloc = element_debut_bloc + cotebloc * cotegrille)
	{
		for (int curseur_bloc = element_debut_bloc; curseur_bloc < element_debut_bloc + cotegrille; curseur_bloc = curseur_bloc + cotebloc)
		{
			// printf("\ncurseur bloc = %d\n", curseur_bloc);
			if (check_bloc(t, curseur_bloc) == 0)
			{
				printf("Erreur de bloc\n"); //A supprimer pour la version final car pas fiable.
				return 0;
			}
			else
			{
				continue;
			}
		}
	}
	return 1;
}

int check_case(int t[], int numero_element)
{

	int ligne[cotegrille];
	int colonne[cotegrille];
	int bloc[cotegrille];
	get_case(t, numero_element, ligne, colonne, bloc);

	if (check_element(bloc, cotegrille) == 1 || check_element(ligne, cotegrille) == 1 || check_element(colonne, cotegrille) == 1)
	{
		return 0;
	}
	return 1;
}

int previous_modifiable(int t_save[], int numero_case)
{
	int k = numero_case - 1;
	while (k >= 0)
	{
		if (t_save[k] != 0)
		{
			k--;
		}
		else
		{
			return k;
		}
	}
	if (k == -1)
	{
		return -1; //Il n'y a pas de case modifiable avant celle entré
	}
}

int next_modifiable(int t_save[], int numero_case)
{
	int k = numero_case + 1;
	while (k < len)
	{
		if (t_save[k] != 0)
		{
			k++;
		}
		else
		{
			return k;
		}
	}
	if (k == 81)
	{
		return -1; //Il n'y a pas de case modifiable après celle entrée
	}
}

void possibilite_case(int t[], int valeurs_possibles[len][cotegrille], int numero_element)
{ //On appel pour numero_element

	int ligne[cotegrille];
	int colonne[cotegrille];
	int bloc[cotegrille];
	get_case(t, numero_element, ligne, colonne, bloc);

	for (int k = 0; k < cotegrille; k++)
	{ //on parcourt valeur_possibles[numero_element][]
		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans la ligne
		for (int i = 0; i < cotegrille; i++)
		{
			if (ligne[i] == valeurs_possibles[numero_element][k])
			{
				valeurs_possibles[numero_element][k] = 0;
			}
			else
			{
				continue; //eventuellement changer le continue pour directement passer à k+1
			}
		}

		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans la colonne
		for (int i = 0; i < cotegrille; i++)
		{
			if (colonne[i] == valeurs_possibles[numero_element][k])
			{
				valeurs_possibles[numero_element][k] = 0;
			}
			else
			{
				continue;
			}
		}

		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans le bloc
		for (int i = 0; i < cotegrille; i++)
		{
			if (bloc[i] == valeurs_possibles[numero_element][k])
			{
				valeurs_possibles[numero_element][k] = 0;
			}
			else
			{
				continue;
			}
		}
	}
}

void get_case(int t[], int numero_element, int ligne[], int colonne[], int bloc[])
{

	int curseur = 0; //Il sert à récuperer où sommes nous dans les tableaux
	int numero_colonne = (numero_element % cotegrille) + 1;
	int numero_ligne = (numero_element - numero_colonne + 1) / cotegrille + 1;
	int colonne_debut_bloc = (numero_colonne - 1) / cotebloc * cotebloc + 1;
	int ligne_debut_bloc = (numero_ligne - 1) / cotebloc * cotebloc + 1;
	int element_debut_bloc = ((ligne_debut_bloc - 1) * cotegrille) - 1 + colonne_debut_bloc;
	int element_fin_bloc = element_debut_bloc + (cotebloc - 1) * cotegrille + cotebloc - 1;

	//On récupère les élements de la ligne
	for (int k = (numero_ligne - 1) * cotegrille; k <= numero_ligne * cotegrille - 1; k++)
	{
		ligne[curseur] = t[k];
		curseur++;
	}
	curseur = 0;

	//On récupère les élements de la colonne.
	for (int k = (numero_colonne - 1); k <= numero_colonne + (cotegrille * (cotegrille - 1)); k = k + cotegrille)
	{
		colonne[curseur] = t[k];
		curseur++;
	}
	curseur = 0;

	//On récupère les élements du bloc
	for (int k = element_debut_bloc; k < element_fin_bloc; k = k + cotegrille)
	{
		for (int i = 0; i < cotebloc; i++)
		{
			bloc[curseur] = t[k + i];
			curseur++;
		}
	}
}

void print_tableau(int t[], int len)
{
	printf("[");
	for (int k = 0; k < len - 1; k++)
	{
		printf("%2d,", t[k]);
	}
	printf("%2d", t[len - 1]);
	printf("]");
}

void print_tableau_deux_dim(int valeurs_possibles[len][cotegrille])
{
	for (int i = 0; i < len; i++)
	{
		printf("[");
		for (int j = 0; j < cotegrille; j++)
		{
			printf("%3d", valeurs_possibles[i][j]);
		}
		printf("]\n");
	}
}

int nombre_valeurs_non_nulles(int valeurs_possibles[len][cotegrille], int numero_element)
{
	int cpt = 0;
	for (int k = 0; k < cotegrille; k++)
	{
		if (valeurs_possibles[numero_element][k] != 0)
		{
			valeur_non_nulle = valeurs_possibles[numero_element][k];
			cpt++;
		}
		else
		{
			continue;
		}
	}
	return cpt;
}
