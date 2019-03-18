void print_grid(int t[]) { //Attention a bien mettre les crochets après le t.
	for (int k = 0; k < length; k++){
		printf("|");
		if(t[k] == 0) {
			printf(" ");
		}
		else {
			printf("%d", t[k]);
		}
		if((k+1)%9 == 0) {
			printf("|\n");
		}
	}
}