#include <stdio.h>
#include <stdlib.h>


FILE *fichSrc;
FILE *f;

#include "functions.c"
const char* tokens[] = {	//Mots cles :
	"PROGRAM_TOKEN","CONST_TOKEN","VAR_TOKEN","BEGIN_TOKEN","END_TOKEN","IF_TOKEN","THEN_TOKEN",
	"WHILE_TOKEN","DO_TOKEN","READ_TOKEN","WRITE_TOKEN",
     //LES SYMBOLES SPECIAUX :
     "PV_TOKEN","PT_TOKEN","PLUS_TOKEN","MOINS_TOKEN","MULT_TOKEN","DIV_TOKEN","VIR_TOKEN","AFF_TOKEN",
     "INF_TOKEN","INFEG_TOKEN","SUP_TOKEN","SUPEG_TOKEN","DIFF_TOKEN","PO_TOKEN","PF_TOKEN","FIN_TOKEN","EG_TOKEN",
     //Autres :
     "ID_TOKEN",
     "NUM_TOKEN",
     "ERREUR_TOKEN"};
int main(){
	//Fichier source :
	fichSrc=fopen("myfile.txt", "r");
	f=fopen("myfile_output1.txt","w");
	if(fichSrc==NULL){
		printf("Chemin non valide !!");
		exit(0);
	}
		int k;
	token currentToken = getToken();
	do{
		k = (int) currentToken.name;
		printf("%s ", tokens[k]);
		fprintf(f,"%s\n",tokens[k]);
		currentToken = getToken(); 
	}while(currentToken.name != FIN_TOKEN);
	fprintf(f,"FIN_TOKEN");
	printf("\nFIN_TOKEN");
	return 0;
}


