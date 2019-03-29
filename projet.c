#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 1000

int len = 256;        // taille du tableau modelisant la grille
int n = 4;          // taille du cote d'un bloc
int cotegrille = 16; // longueur d'un cote de la grille
int cotebloc = 4;   // longeur d'un bloc	
int compteur_tentatives = 0;
int *pointeur_sur_compteur_tentative = &compteur_tentatives;

void print_grid(int t[]);
int read_grid(int t[]);//return(-1) si rencontre un problème dans le type de fichier (caractère ou taille incorrect par exemple), renvoit 0 sinon
void write_grid(int t[]);
int check_grid (int t[]); // Il renvoit 1 si la grille est compatible, 0 sinon.
int check_element(int tableau[], int longueur_tableau); //Il renvoit 1 si un même élement est présent deux fois dans le tableau, 0 sinon.
int solve (int t[]);
int next_modifiable(int t_save[], int numero_case);
int previous_modifiable(int t_save[], int numero_case);
int check_ligne(int t[], int numero_ligne); //Elle renvoit 1 si la ligne est compatible, O sinon.
int check_colonne(int t[], int numero_colonne); //Elle renvoit 1 si la colonne est compatible, O sinon.
int check_bloc(int t[], int numero_bloc); //Elle renvoit 1 si le bloc est compatible, O sinon.
int check_case(int t[], int numero_element);

void print_tableau (int t[], int len) {
	printf("[");
	for(int k = 0; k < len - 1; k++) {
		printf("%2d,", t[k]);
	}
	printf("%2d", t[len - 1]);
	printf("]");
}

int main() {
	int t[len];
	read_grid(t);
	print_grid(t);
	printf("%d\n",check_grid(t));
	return 0;
}

int read_grid(int t[]) { 
	FILE* fichier = NULL;
    
    char chaine[TAILLE_MAX] = "";
 	int position = 0;
 	int i = 0;
 	int tailleligne = 4*cotegrille;
 	char ligne[tailleligne];


    fichier = fopen("test.txt", "r");
 	
 	if (fichier == NULL) {
 		printf("Le fichier est introuvable.\n");
 		return -1;
 	} else {
        while (i < cotegrille) {
        	int j = 0;
            fgets(ligne, tailleligne, fichier);
            printf("Ligne lue : '%s'", ligne);
            char *token;
            token = strtok(ligne, " ");

            //Afficher le résultat du découpage
            while (j < cotegrille) {
                t[position] = atoi(token);
                position++;
                token = strtok(NULL, " ");
                j++;
            }
            printf("\n");
            i++;
            //printf("valeur de i : %d, valeur de j : %d\n", i,j);
        }
        fclose(fichier);
	}
	return 0;
}
  

int check_bloc(int t[], int element_debut_bloc) { //le tableau bloc géneré n'est pas bon
	int bloc[cotegrille];
	int element_fin_bloc = element_debut_bloc + (cotebloc-1)*cotegrille + cotebloc -1; //bien généralisé
	int curseur = 0; //Il sert à récuperer ou sommes nous dans le tableau bloc[]
	for(int k = element_debut_bloc; k < element_fin_bloc; k = k + cotegrille) {
		for (int i = 0; i < cotebloc; i++) {
			// printf("k = %d, i= %d\n", k,i); //k+i est bon
			// printf("t[k+i]=%d\n",t[k + i]);
			printf("curseur = %d\n", curseur);
			bloc[curseur] = t[k+i];
			curseur++;
			printf("bloc[curseur] = %d\n", bloc[curseur]);
		}
		print_tableau(bloc, cotegrille);
	}
	if(check_element(bloc, cotegrille) == 1) {
		return 0;
	} else {
		return 1;
	}
}

int check_case (int t[], int numero_element) {
	int numero_colonne = numero_element % cotegrille + 1;
	if(check_colonne(t, numero_colonne) == 0) {
		return 0;
	}

	int numero_ligne = (numero_element - numero_colonne)/cotegrille +2;
	if (check_ligne(t,numero_ligne) == 0) {
		return 0;
	}

	int colonne_debut_bloc = (numero_colonne-1)/cotebloc * cotebloc + 1;
	int ligne_debut_bloc = (numero_ligne-1)/cotebloc * cotebloc + 1;
	int element_debut_bloc = ((ligne_debut_bloc - 1)*cotegrille) - 1 + colonne_debut_bloc;
	if(check_bloc(t, element_debut_bloc) == 0){
		return 0;
	}
	return 1;
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
		for (int curseur_bloc = element_debut_bloc; curseur_bloc < cotebloc; curseur_bloc = curseur_bloc + cotebloc) {
			if(check_bloc(t, element_debut_bloc) == 0) {
				printf("erreur de bloc, pour element_debut_bloc = %d", element_debut_bloc);
				return 0;
			} else {
				continue;
			}
		}
				
	}
return 1;
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

int solve (int t[]){
	printf("On est dans le solve\n");
	//On commence par vérifier si la sudoku peut être résolue.
	if (check_grid(t) == 0){
		printf("La grille n'est pas compatible.\n");
		return 1;
	}
	printf("On est sorie de check_grid du solves\n");
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
		}
	}
	return 0;
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

void print_grid(int t[]) { //Attention a bien mettre les crochets après le t.
	for (int k = 0; k < len; k++){
		printf("%2d", t[k]);
		if((k+1)%cotegrille == 0) {
			printf("\n");
		}
	}
}

void write_grid(int t[]){

	FILE* fichier = NULL;
 
    fichier = fopen("vide.txt", "w");
 
    if (fichier != NULL) { //On verifie que vide.txt ait bien été chargé dans la variable fichier
    	for (int k = 1; k <= len; k++) {
			fprintf(fichier, "%2d", t[k-1]);
			if(k%(cotegrille) == 0) {
				fprintf(fichier, "\n");
			}
    	}
        fclose(fichier);
    } else {
         printf("Le fichier n'a pas pu être chargé.\n");
    }
}