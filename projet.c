#include <stdio.h>
#include <stdlib.h>

int len = 81;        // taille du tableau modelisant la grille
int n = 3;          // taille du cote d'un bloc
int cotegrille = 9; // longueur d'un cote de la grille
int cotebloc = 3;   // longeur d'un bloc


void print_grid(int t[]) { //Attention a bien mettre les crochets après le t.
	for (int k = 0; k < len; k++){
		printf("%2d", t[k]);
		if((k+1)%cotegrille == 0) {
			printf("\n");
		}
	}
}

void read_grid(int t[]){ 
//On lui indique dans quel tableau il doit sauvegarder le fichier.
//le programme ne fonctionne pas si on a des valeurs à deux chiffres.
 	FILE* fichier = NULL;
    fichier = fopen("test.txt", "r");

	if (fichier == NULL) {
		printf("Impossible d'ouvrir le fichier test.txt");
    }
    else {
		int caractereActuel = 0;
		int cpt = 0;
		while(caractereActuel != EOF) {
					caractereActuel = fgetc(fichier); // On lit le caractere
					if( caractereActuel < 48 || caractereActuel > 57 ) {
						continue; //on ne s'interesse pas à tout ce que n'est pas un chiffre entre 0 et 9, codé entre 49 et 57 en ascii.
					} else {
						// convetir les caractères en entier.
						// rentrer le caractère dans le tableau.
						int caractere_converti;
						caractere_converti = caractereActuel - 48; //On converti les caractères en entiers.
						t[cpt] = caractere_converti;
						cpt ++;
					}
    	}
    }
    fclose(fichier);
    // print_grid(t);
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



int main() { 
	// On déclare un talbeau de 81 entier, pour les cases vide on asigne la valeur 0.
	// int t[81] = {	0,	0,	0,		0,	0,	0,		0,	0,	7, 
	// 				7,  2,  0,		3,	0,	9,		0,	0,	1,
	// 				0,	0,	8,		7,	0,	5,		0,	6,	0,

	// 				5,	0,	2,		8,	9,	0,		0,	0,	0,
	// 				0,	4,	0,		5,	0,	1,		0,	9,	0,
	// 				0,	0,	0,		0,	6,	3,		7,	0,	5,

	// 				0,	3,	0,		9,	0,	6,		1,	0,	0,
	// 				2,	0,	0,		1,	0,	7,		0,	5,	3,
	// 				9,	0,	0,		0,	0,	0,		0,	0,	0,	};
	// On déclare une variable length égale à 81 pour avoir pouvoir adapter le code si on doit faire une programme pour des sudoku avec plus de cases 
	int t[len]; 
	read_grid(t);
	write_grid(t);
	return 0;
}