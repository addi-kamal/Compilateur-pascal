#include <stdbool.h>


//MOTS CLES :

const char* mot_cles[]={"program","const","var","begin","end","if","then","while","do","read","write"};
//LES SYMBOLES SPECIAUX :

char* symbol_speci[]={";",".","+","-","*","/",",",":=","<","<=",">",">=","<>","(",")","EOF","="};

//L'univers de TOKEN :
typedef enum{
	//Mots cles :
	PROGRAM_TOKEN,CONST_TOKEN,VAR_TOKEN,BEGIN_TOKEN,END_TOKEN,IF_TOKEN,THEN_TOKEN,
	WHILE_TOKEN,DO_TOKEN,READ_TOKEN,WRITE_TOKEN,
     //LES SYMBOLES SPECIAUX :
     PV_TOKEN,PT_TOKEN,PLUS_TOKEN,MOINS_TOKEN,MULT_TOKEN,DIV_TOKEN,VIR_TOKEN,AFF_TOKEN,
     INF_TOKEN,INFEG_TOKEN,SUP_TOKEN,SUPEG_TOKEN,DIFF_TOKEN,PO_TOKEN,PF_TOKEN,FIN_TOKEN,EG_TOKEN,
     //Autres :
     ID_TOKEN,
     NUM_TOKEN,
     ERREUR_TOKEN
}CODE_LEX;

//structure d'un identificateur :

typedef struct{
   char *name;	
}id_token;

//structure d'un nombre :

typedef struct{
	bool isInt; // entier ou flottant
	union{
		int n;
		float x;
	}value;
}est_num;




// La structure d'un token
typedef struct{
	CODE_LEX name; // Le nom du token
	union{
		id_token idf; // les informations de l'id (si le token est ID_TOKEN)
		est_num number; // les informations du nombre (si le token est NUMBER_TOKEN)
	}properties;
} token;








