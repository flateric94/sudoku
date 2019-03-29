#include <stdio.h>
#include <stdlib.h>

int len = 81;        // taille du tableau modelisant la grille
int n = 3;          // taille du cote d'un bloc
int cotegrille = 9; // longueur d'un cote de la grille
int cotebloc = 3;   // longeur d'un bloc	
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
void check_case(int t[], int numero_element);


int main() {
	int t[len];
	read_grid(t);
	print_grid(t);
	check_case(t, 26);
	return 0;
}

int check_ligne(int t[], int numero_ligne) {
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


int check_colonne(int t[], int numero_colonne) {
		int colonne[cotegrille];
		int i = 0;
		for (int k = (numero_colonne - 1); k <= numero_colonne + 72; k = k + 9) {
			colonne[i]=t[k];
			i++;
		}
		if(check_element(colonne, cotegrille) == 1) {
			return 0;
		} else {
			return 1;
		}
}

int check_bloc(int t[], int numero_bloc) {
	printf("CC\n");
	int repere_bloc[9][9] = {	{0,1,2,9,10,11,18,19,20},
						{3,4,5,12,13,14,21,22,23},
						{6,7,8,15,16,17,24,25,26},
						{27,28,29,36,37,38,45,46,47},
						{30,31,32,39,40,41,48,49,50},
						{33,34,35,42,43,44,51,52,53},
						{54,55,56,63,64,65,72,73,74},
						{57,58,59,66,67,68,75,76,77},
						{60,61,62,69,70,71,78,79,80}	};

	int bloc[cotegrille];
	int curseur; //Il sert à récuperer les valeurs dans repere_bloc pour ensuite faire t[curseur] 
	for (int k = 0; k <= 8; k++) {
		curseur = repere_bloc[numero_bloc][k];
		bloc[k] = t[curseur];
	}
	printf("jte baise\n");
	if(check_element(bloc, cotegrille) == 1) {
		return 0;
	}
	return 1;
}

void check_case (int t[], int numero_element) {
	int numero_colonne = numero_element % 9 + 1;
	printf("Numero de colonne = %d\n", numero_colonne);
	// if(check_colonne(t, numero_colonne) == 0) {
	// 	return 0;
	// } else {
	// 	continue;
	// }

	int numero_ligne = (numero_element - numero_colonne)/9 +2;
	printf("Numéro de ligne = %d\n", numero_ligne);

	//Il faut reussir à récuperer le numero de bloc de manière inteligente.
	
	// printf("numero bloc = %d\n", numero_bloc);						
	
	
}	

int check_grid (int t[]) {

//On verifie que les lignes sont compatibles.
	for(int numero_ligne = 1; numero_ligne <= cotegrille; numero_ligne++) { //On fait une boucle pour verifier toutes les lignes, elles sont numérotées de 1 à coter grille.
		if(check_ligne(t, numero_ligne) == 0) {
			return 0;
		} else {
			continue;
		}
	}

// On verifie que les colonnes sont compatibles. 
	for(int numero_colonne = 1; numero_colonne <= cotegrille; numero_colonne++) { //On sait que les éléments de la nieme colonne sont congrus a (n - 1) mod 9
		if(check_colonne(t, numero_colonne) == 0) {
			return 0;
		} else {
			continue;
		}
	}
// On verifie que les blocs sont compatibles.
	for(int numero_bloc = 0; numero_bloc < cotegrille; numero_bloc++) {
		if(check_bloc(t, numero_bloc) == 0) {
			return 0;
		} else {
			continue;
		}		
	}
return 1;
}

int check_element(int tableau[], int longueur_tableau){
	int masque[9];
	for(int k = 0; k < 9; k++) {
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


int read_grid(int t[]) { 

 	FILE* fichier = NULL;
    fichier = fopen("test.txt", "r");

	if (fichier == NULL) {
		printf("Impossible d'ouvrir le fichier test.txt");
    } else {

    	int deux_dernieres_caracteres[2]; //On mémorise les deux dernieres caratères
		int caractereActuel = 0;
		int caractere_converti;
		int cpt = 0;

		while(caractereActuel != EOF) {

					caractereActuel = fgetc(fichier); // On lit le caractere
					deux_dernieres_caracteres[0] = deux_dernieres_caracteres[1];
					deux_dernieres_caracteres[1] = caractereActuel;

					if( (deux_dernieres_caracteres[0] > 48 && deux_dernieres_caracteres[0] < 58) && (deux_dernieres_caracteres[1] > 48 && deux_dernieres_caracteres[1] < 58) ) { //On vérifie qu'il n'y ait pas deux chiffres NON NULS consécutifs.
						return(-1); //Le programe contient des nombre à plus de deux chiffres.
					}
					if( (caractereActuel >= 33 && caractereActuel < 48) || (caractereActuel > 57) ) { //Si on détecte autre chose que des chiffres entre 0 et 9 ou des espaces et retours à la ligne on renvoit un message d'erreur.
						return(-1); //Un caractère incorect est présent dans le fichier.
					}
					if(caractereActuel <= 32 ) {
						continue; //on ne s'interesse pas à tout ce que n'est pas un chiffre entre 0 et 9
					} else {
						caractere_converti = caractereActuel - 48; //On converti les caractères en entiers.
						t[cpt] = caractere_converti; //On rentrer le caractère dans le tableau.
						cpt ++;
					}
					if(cpt > 81) { //Si on a plus de 81 chiffres on revoit une erreur 
						return(-1); //Il y a plus de 81 chffre dans le fichier.
					}
    	}

    	if(cpt < 81) {
    		return(-1); //Le fichier ne contient pas asser de chiffre.
   		 }

   		 for (int k = 0; k < len; k++) { //On verifie si le tableau n'est pas déjà plein.
  		  	if(t[k] == 0) {return 0;}
  	 	 }
    	return(-1); //Le fichier est déjà plein.
		}

    fclose(fichier);
}

int solve (int t[]){

	//On commence par vérifier si la sudoku peut être résolue.
	if (check_grid(t) == 0){
		printf("La grille n'est pas compatible.\n");
		return 1;
	}

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
		for(int i = t[k] + 1; i <=9; i++) { //On tente tout les valeurs pour t[k], si aucune n'est valide on remonte et k = previous_modifiable.
			t[k] = i; //On associe à notre terme la valeur de i.
			compteur_tentatives++;
			if(check_grid(t) == 1) { 
				k = next_modifiable(t_save, k); //Si grille reste compatible on passe a la case suivante
				if(k>80 || k < 0) { //On arrête la boucle while si on est en dehors du tableau.
					return 0;
				}
				break;
			} else if (check_grid(t) == 0 && i == 9) { //Si on a essayé tout les valeurs pour t[k] jusqu'à 9 et que ça ne fonctionne toujours pas on modifie la case précédente.
				while (t[k] == 9) { //On va remonter juqu'à trouver une case qui n'est pas déja à 9 et on remet à zero toute celles qu'on rencontre en remontant.
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