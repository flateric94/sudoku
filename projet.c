#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len = 256;   			// taille du tableau modelisant la grille
int n = 4;					// taille du cote d'un bloc
int cotegrille = 16;		// longueur d'un cote de la grille
int cotebloc = 4;   		// longeur d'un bloc	
int compteur_tentatives = 0;
int *pointeur_sur_compteur_tentative = &compteur_tentatives;
int valeur_non_nulle;		


//Fonctions demandées dans l'énoncé
void print_grid(int t[]);
int read_grid(int t[]);//Attention à ne pas mettre de retour à la ligne dans le fichier
void write_grid(int t[]);
int solve (int t[], int valeurs_possibles[len][cotegrille]); //doit prendre le tableau de départ ainsi que les valeurs que le tableau a 2 dim (valeur_possible)a pu permettre de remplir.

//Fonctions complémentaires
int check_element(int tableau[], int longueur_tableau); // Elle renvoit 1 si un même élement est présent deux fois dans le tableau, 0 sinon.
int check_ligne(int t[], int numero_ligne); //Elle renvoit 1 si la ligne est compatible, O sinon.
int check_colonne(int t[], int numero_colonne); //Elle renvoit 1 si la colonne est compatible, O sinon.
int check_bloc(int t[], int numero_bloc); //Elle renvoit 1 si le bloc est compatible, O sinon.
int check_grid (int t[]); // Elle renvoit 1 si la grille est compatible, 0 sinon.
int check_case(int t[], int numero_element); // Elle renvoit 1 si la case est compatible, 0 sinon.
int previous_modifiable(int t_save[], int numero_case);
int next_modifiable(int t_save[], int numero_case);
void possibilite_case(int t[], int valeurs_possibles[len][cotegrille], int numero_element); //Prend en argument un numero d'élement et crée un tableau avec les élements possibles dans la case.
int get_case (int t[], int numero_element, int ligne[], int colonne[], int bloc[]); //Pour un élement donné il crée un tableau colonne[], case[] et bloc[] dans lequel il met les élements qui correspondent à la case.

//Fonctions de travail
void print_tableau  (int t[], int len); //affiche tableaux de 1dim
void print_tableau_deux_dim (int valeurs_possibles[len][cotegrille]); //affiche tableaux 2dim
int nombre_valeurs_non_nulles (int valeurs_possibles[len][cotegrille], int numero_element); //Renvoit le nombre de valeurs non nulles de valeur_possible[numero_element][k] pour un numero d'élément donné

int main() {
	int t[len];
	int valeurs_possibles [len][cotegrille]; //On crée le tableau servant à sauvegarder les possibilitées de chaque case.
	for(int i = 0; i < len; i++) {	//On initialise valeurs_possibles 
		int cpt = 1;
		for(int j = 0; j < cotegrille; j++) {
			valeurs_possibles[i][j] = cpt;
			cpt ++;
		}
	}
	read_grid(t);
	print_grid(t);
	solve(t, valeurs_possibles);
	print_tableau_deux_dim (valeurs_possibles);
	print_grid(t);
	write_grid(t);
	return 0;
}


void print_grid(int t[]) { //Attention a bien mettre les crochets après le t.
	for (int k = 0; k < len; k++){
		printf("%3d", t[k]);
		if((k+1)%cotegrille == 0) {
			printf("\n");
		}
	}
}


int read_grid(int t[]) { 
    int tailleligne = 10*cotegrille; //Il faut bien mettre ca (3*coter grille ne suffit pas)
    int position = 0; //Repere pour l'injection dans le tableau en parametre
    int i = 0;

    FILE *fichier = fopen("test.txt", "r");

    if (fichier != NULL) {
        while (i < cotegrille) {

            char ligne[tailleligne];
            fgets(ligne, tailleligne, fichier); 
            char *token;
            int j = 0;
            token = strtok(ligne, "  ");

            while (j < cotegrille) {
                t[position] = atoi(token);
                position++;
                token = strtok(NULL, " ");
                j++;
            }
            i++;
        }
        fclose(fichier);
    }
    return 1; // Tout s'est bien passe
}


void write_grid(int t[]){

	FILE* fichier = NULL;
 
    fichier = fopen("vide.txt", "w");
 
    if (fichier != NULL) { //On verifie que vide.txt ait bien été chargé dans la variable fichier
    	for (int k = 1; k <= len; k++) {
			fprintf(fichier, "%3d", t[k-1]);
			if(k%(cotegrille) == 0) {
				fprintf(fichier, "\n");
			}
    	}
        fclose(fichier);
    } else {
         printf("Le fichier n'a pas pu être chargé.\n");
    }
}


int solve (int t[], int valeurs_possibles[len][cotegrille]){
// 1- On commence par vérifier si la sudoku peut être résolue.
	if (check_grid(t) == 0){
		printf("La grille n'est pas compatible.\n");
		return 1;
	}

// 2- On tente les méthodes humaines
	while(0 == 0) { //Pour avoir une boucle qui ne s'arret que quand elle atteint break.
		//On appel valeurs_possible pour tout les élement du tableau qui ne sont pas déjà remplis
			for(int k = 0; k < len; k++) {
				if (t[k] != 0) {
					for (int i = 0; i < cotegrille; i++) {
						valeurs_possibles[k][i] = -1;
					}
				} else {
					possibilite_case(t, valeurs_possibles, k);
				}
			}
		//On regarde si pour certain element nb_valeur_non_nulles de valeurs possible est égale à 1
			//Si on en trouve on attribue les valeues et on recommence while
			//Sinon on arrete le while
			int cpt = 0;
			for(int k = 0; k < len; k++) {
				if( nombre_valeurs_non_nulles(valeurs_possibles, k) == 1) {
					t[k] = valeur_non_nulle;
					cpt++;
				}
			}
			if (cpt == 0) {
				break;
			}
	}
//On finit par un back_traking ameilloré

	//On commence par dubliqué le tableau donné en entré.
	int t_save[len];
	for(int k = 0; k <= len; k++){
		t_save[k] = t[k];
	}

	// On prend le min entre 0 et next_modifiable(t_save, 0)
	int k; 
	if(t[0] == 0) {
		k = 0;
	} else {
		k = next_modifiable(t_save, 0);
	}

	//On resout par backtracking
	while(k < len){
		//On tente de  trouver une valeur qui fonctionne pour t[k]
		for(int i = t[k] + 1; i <= cotegrille; i++) { //On tente tout les valeurs pour t[k], si aucune n'est valide on remonte et k = previous_modifiable.
			t[k] = i; //On associe à notre terme la valeur de i.
			compteur_tentatives++;
			if(check_case(t,k) == 1) { 
				k = next_modifiable(t_save, k); //Si grille reste compatible on passe a la case suivante
				if(k>(len-1) || k < 0) { //On arrête la boucle while si on est en dehors du tableau.
					return 0;
				}
				break;
			} else if (check_case(t, k) == 0 && i == cotegrille) { //Si on a essayé tout les valeurs pour t[k] jusqu'à 9 et que ça ne fonctionne toujours pas on modifie la case précédente.
				while (t[k] == cotegrille) { //On va remonter juqu'à trouver une case qui n'est pas déja à 9 et on remet à zero toute celles qu'on rencontre en remontant.
					t[k] = 0; //On remet k à 0 avant de remonter dans le tableau
					k = previous_modifiable(t_save, k);
				}
			}
			if(compteur_tentatives % 10000000 == 0) {
				printf("compteur_tentatives = %d\n", compteur_tentatives);
				print_grid(t);
				printf("\n");
			}		
		}
	}
	return 0;
}


int check_element(int tableau[], int longueur_tableau){
	int masque[cotegrille];
	for(int k = 0; k < cotegrille; k++) {
		masque[k] = 1;
	}
	for(int k = 0; k < longueur_tableau; k++) {
		if(tableau[k] == 0) {
			continue;
		} else if(masque[tableau[k]-1] == 0) { //On prend k-1 car dans notre masque la première case est numéroté 0, ce qui décale tout la numérotation.
			return 1;
		} else {
			masque[tableau[k]-1] = 0;
		}
	}
	return 0;
}


int check_ligne(int t[], int numero_ligne) { //généralisé à priori
	int ligne[cotegrille];
	int i = 0; //numérote les case du tableau ligne.

	//Dans une premier temps on recupères les élements d'une ligne dans un tableau.
	for(int k = (numero_ligne - 1)*cotegrille; k <= numero_ligne*cotegrille - 1; k++) { //On parcourt tout les termes de la ligne
		ligne[i]=t[k];
		i++;
	}
	if(check_element(ligne, cotegrille) == 1) {
		return 0;
	} else {
		return 1;
	}
}


int check_colonne(int t[], int numero_colonne) { //généralisé à priori
		int colonne[cotegrille];
		int i = 0;
		for (int k = (numero_colonne - 1); k <= numero_colonne + (cotegrille * (cotegrille - 1)); k = k + cotegrille) {
			colonne[i]=t[k];
			i++;
		}
		if(check_element(colonne, cotegrille) == 1) {
			return 0;
		} else {
			return 1;
		}
}


int check_bloc(int t[], int element_debut_bloc) { //le tableau bloc géneré n'est pas bon
	int bloc[cotegrille];
	int element_fin_bloc = element_debut_bloc + (cotebloc-1)*cotegrille + cotebloc -1; //bien généralisé
	int curseur = 0; //Il sert à récuperer ou sommes nous dans le tableau bloc[]
	for(int k = element_debut_bloc; k < element_fin_bloc; k = k + cotegrille) {
		for (int i = 0; i < cotebloc; i++) {
			bloc[curseur] = t[k+i];
			curseur++;
		}
	}
	if(check_element(bloc, cotegrille) == 1) {
		return 0;
	} else {
		return 1;
	}
}


int check_grid (int t[]) {

//On verifie que les lignes sont compatibles.
	for(int numero_ligne = 1; numero_ligne <= cotegrille; numero_ligne++) { //On fait une boucle pour verifier toutes les lignes, elles sont numérotées de 1 à coter grille.
		if(check_ligne(t, numero_ligne) == 0) {
			printf("erreur de ligne");
			return 0;
		} else {
			continue;
		}
	}

// On verifie que les colonnes sont compatibles. 
	for(int numero_colonne = 1; numero_colonne <= cotegrille; numero_colonne++) { //On sait que les éléments de la nieme colonne sont congrus a (n - 1) mod 9
		if(check_colonne(t, numero_colonne) == 0) {
			printf("erreur de colonne");
			return 0;
		} else {
			continue;
		}
	}

// On verifie que les blocs sont compatibles.
	for(int element_debut_bloc = 0; element_debut_bloc < len; element_debut_bloc = element_debut_bloc + cotebloc*cotegrille) {
		for (int curseur_bloc = element_debut_bloc; curseur_bloc < element_debut_bloc + cotegrille; curseur_bloc = curseur_bloc + cotebloc) {
			// printf("\ncurseur bloc = %d\n", curseur_bloc);
			if(check_bloc(t, curseur_bloc) == 0) {
				printf("erreur de bloc, pour curseur = %d\n", curseur_bloc);
				return 0;
			} else {
				continue;
			}
		}
				
	}
return 1;
}


int check_case (int t[], int numero_element) {

	int ligne[cotegrille];
	int colonne[cotegrille];
	int bloc[cotegrille];
	get_case(t, numero_element, ligne, colonne, bloc);

	if(check_element(bloc, cotegrille) == 1 || check_element(ligne,cotegrille) == 1 || check_element(colonne, cotegrille) == 1) {
		return 0;
	}
	return 1;
}


int previous_modifiable(int t_save[], int numero_case){
	int k = numero_case - 1;
	while(k >= 0) {
		if (t_save[k] != 0) {
			k--;
		} else {
			return k;
		}
	}
	if(k = -1) {
		return -1; //Il n'y a pas de case modifiable avant celle entré
	}
}


int next_modifiable(int t_save[], int numero_case){
	int k = numero_case + 1;
	while(k < len) {
		if (t_save[k] != 0) {
			k++;
		} else {
			return k;
		}
	}
	if(k = 81) {
		return -1; //Il n'y a pas de case modifiable après celle entré
	}
}


void possibilite_case (int t[], int valeurs_possibles[len][cotegrille], int numero_element) { //On appel pour numero_element 

	int ligne[cotegrille];
	int colonne[cotegrille];
	int bloc[cotegrille];
	get_case(t, numero_element, ligne, colonne, bloc);

	for(int k = 0; k < cotegrille; k++){ //on parcourt valeur_possibles[numero_element][]
		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans la ligne
		for (int i = 0; i < cotegrille; i++) {
			if (ligne[i] == valeurs_possibles[numero_element][k]) {
				valeurs_possibles[numero_element][k] = 0;
			} else {
				continue; //eventuellement changer le continue pour directement passer à k+1
			}
		}

		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans la colonne
		for (int i = 0; i < cotegrille; i++) {
			if (colonne[i] == valeurs_possibles[numero_element][k]) {
				valeurs_possibles[numero_element][k] = 0;
			} else {
				continue;
			}
		}

		//On regarde si la valeur de valeur_possibles[numero_element][k] est dans le bloc
		for (int i = 0; i < cotegrille; i++) {
			if (bloc[i] == valeurs_possibles[numero_element][k]) {
				valeurs_possibles[numero_element][k] = 0;
			} else {
				continue;
			}
		}
	}
}


int get_case (int t[], int numero_element, int ligne[], int colonne[], int bloc[]) {

	int curseur = 0; //Il sert à récuperer où sommes nous dans les tableaux
	int numero_colonne = (numero_element % cotegrille) + 1;
	int numero_ligne = (numero_element - numero_colonne + 1)/cotegrille +1;
	int colonne_debut_bloc = (numero_colonne-1)/cotebloc * cotebloc + 1;
	int ligne_debut_bloc = (numero_ligne-1)/cotebloc * cotebloc + 1;
	int element_debut_bloc = ((ligne_debut_bloc - 1)*cotegrille) - 1 + colonne_debut_bloc;
	int element_fin_bloc = element_debut_bloc + (cotebloc-1)*cotegrille + cotebloc -1;
	
	//On récupère les élements de la ligne
	for(int k = (numero_ligne - 1)*cotegrille; k <= numero_ligne*cotegrille - 1; k++) {
		ligne[curseur] = t[k];
		curseur++;
	}
	curseur = 0;

	//On récupère les élements de la colonne.
	for (int k = (numero_colonne - 1); k <= numero_colonne + (cotegrille * (cotegrille - 1)); k = k + cotegrille) {
		colonne[curseur]=t[k];
		curseur++;
	}
	curseur = 0;

	//On récupère les élements du bloc
	for(int k = element_debut_bloc; k < element_fin_bloc; k = k + cotegrille) {
		for (int i = 0; i < cotebloc; i++) {
			bloc[curseur] = t[k+i];
			curseur++;
		}
	}

}


void print_tableau (int t[], int len) {
	printf("[");
	for(int k = 0; k < len - 1; k++) {
		printf("%2d,", t[k]);
	}
	printf("%2d", t[len - 1]);
	printf("]");
}


void print_tableau_deux_dim (int valeurs_possibles[len][cotegrille]) {
	for(int i = 0; i < len; i++) {
		printf("[");
		for(int j = 0; j < cotegrille; j++) {
			printf("%3d", valeurs_possibles[i][j]);
		}
		printf("]\n");
	}
}


int nombre_valeurs_non_nulles (int valeurs_possibles[len][cotegrille], int numero_element){
	int cpt = 0;
	for(int k = 0; k < cotegrille; k++) {
		if(valeurs_possibles[numero_element][k] != 0) {
			valeur_non_nulle = valeurs_possibles[numero_element][k];
			cpt++;
		} else {
			continue;
		}
	}
	return cpt;
}










