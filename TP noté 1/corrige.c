#include <stdio.h>

#ifdef main
const char** __str;
#define scanf(...) sscanf(*(__str++), ##__VA_ARGS__)
#endif

/* =========================================================================
                     NE RIEN MODIFIER AVANT CETTE LIGNE
   ========================================================================= */

int exercice1(int k) { //on doit avoir k >= 2
	int test = 1;
	if (k < 2) {
		printf("Il faut que la valeur entré soit superieur ou égale à deux. \n");
	}
	for(int i = 2; i < k; i++) {
		if ( k % i == 0){
			test = 0;
		}
	}
	return test; 
}

void exercice2() {
	int S = 0, x=0;
	while (x >= 0){
		S = S + x;
		printf("Entrer un entier relatif.\n");
		scanf("%d", &x);
	}
	printf("La somme des entiers positifs entrés est de : %d\n", S);
}

int exercice3(int k) {
//On suppose que la suite atteint toujours la valeur une puis que les valeurs 1, 4 et 2 se répètent à l'infini.
	int max = 4, S;
	/*printf("Entré la valeur de la suite pour n = 0 :\n");
	scanf("%d", &S); */
	S = k;
	while (S != 1){
		if (S % 2 == 0) {
			S = S/2;
		} else {
			S = 3*S +1;
		}

		if (S > max) {
			max = S;
		}	
	}
	return max;
}

void exercice4(int k) {
	for (int i = 1; i <= k + 2; i++){
		printf("#");
	}
	printf("\n");

	for (int i = 0; i < k; i++){
		printf("#");
		for (int j = 1; j <= k; j++){
			if (j == k - i) {
				printf("/");
			} else {
				printf(" ");
			}
		}
		printf("#");
		printf("\n");
	}

	for (int i = 1; i <= k +2; i++){
		printf("#");
	}
	printf("\n");
}

int main() {
	exercice4(20);
	return 0;
}
