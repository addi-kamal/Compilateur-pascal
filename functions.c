#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"
#include <ctype.h>
#define IDFMAX 70
#define NUMBERMAX 40
bool isNumber=true;
int mots_cles_size=sizeof(mot_cles)/sizeof(mot_cles[0]);
int sym_speci_size=sizeof(symbol_speci)/sizeof(symbol_speci[0]);


bool estmotcle(char *c);

int symbole=0;






est_num getNumber(char digit, bool isNegative){
	int i=0;
	// variable qui indique s'il s'agit d'un entier ou un flottant
	bool isInt = true;
	// Allocation de la mémoire
	char* memory = (char *)malloc(NUMBERMAX*sizeof(char));
	est_num A;
	// Lire tout le chiffre
	do{
		memory[i] = digit;
		digit = fgetc(fichSrc); // caractère suivant
		i++;
		if(digit == '.'){ // si on arrive à une virgule (un nombre flottant)
			isInt = false;
			memory[i] = '.';
			digit = fgetc(fichSrc);
			i++;
		}
	}while(isdigit(digit)!= 0);
	memory[i] = '\0';
	/*
		on sort de do..while Lorsqu'on lit un caractère non numérique
		il faut retourner le curseur pour que cette caractère ne soit perdu
	*/
	ungetc(digit, fichSrc);
	A.isInt = isInt;
	if(isInt == true){ // un nombre entier
		// transformer le nombre dans la chaine de caractère memory en int et le stoker dans A.value.n
		A.value.n = atoi(memory);
		if(isNegative == true){
			A.value.n = - A.value.n;
		}
	}else{ // un nombre flottant
		// transformer le nombre dans la chaine de caractère memory en float et le stoker dans A.value.x
		A.value.x = atof(memory);
		if(isNegative == true){
			A.value.x = - A.value.x;
		}
	}
	return A;
}

token getToken(){
	bool previousIsNumber = isNumber;
    isNumber = false;
	// Lire le caractère suivant
	char character = fgetc(fichSrc);
	token A;
	int i = 0;
	// Eliminer les blancs
	if(character == ' ' || character == '\t' || character == '\n'){
		return getToken();
	}
	// Reconnaissance des mots-clé et des identificateurs
	else if(isalpha(character) != 0){
		// Allocation de la mémoire pour sauvegarder le mot
		A.properties.idf.name = (char *)malloc(IDFMAX*sizeof(char));
		char *mot=(char *)malloc(IDFMAX*sizeof(char));
		// Lire tout le mot
        do{
        	mot[i]=character;
        	character=fgetc(fichSrc);
        	i++;
		}while((isalpha(character)!=0) || (isdigit(character)!=0));
		strcpy(A.properties.idf.name,mot); // il faut poser \0 à la fin du mot
		ungetc(character, fichSrc);
		i=0;
		// vérifier si lo mot obtenue est un mot-clé :
		while(i < mots_cles_size){
			if(stricmp(A.properties.idf.name, mot_cles[i]) == 0){ // Le mot est un mot-clé
				// Le nom token c'est (nameToken) i
				A.name = (CODE_LEX) i;
				// Libérer la case mémoire A.properties.idf.name
				// On a besoin du nom de token seulement dans ce cas (et pas sa valeur)
				free(A.properties.idf.name);
				return A; // retourner le token
			}
			i++;
		}
		// Si on arrive à cette étape, le mot n'est pas mot-clé
		// Donc c'est un identificateur, et son nom est stocké dans A.properties.idf.name

		A.name = ID_TOKEN;
		return A; 
	}
	// Reconnaissance des nombres
	else if(isdigit(character) != 0){
		isNumber = true;
		A.name = NUM_TOKEN;
		// récupérer le nombre et le stocker dans A.properties.number
		A.properties.number = getNumber(character, false);
		return A;	
	}
	else if(character == '+' || character == '-'){
		CODE_LEX symboleToken = (character == '+' ? PLUS_TOKEN : MOINS_TOKEN);
		bool isNegative = (character == '-' ? true : false);
		// Eliminer les espaces
		do{
			character = fgetc(fichSrc);
		}while(character == ' ');
		// Si le caractère suivant de +/- est un chiffre
		if(isdigit(character) != 0){
			if(previousIsNumber == true){
				// le token précédant est un nombre, exemple de cette situation : "5 - 3"
				ungetc(character, fichSrc); // retourner le curseur en arrière
				A.name = symboleToken; // token : PLUS_TOKEN ou MINUS_TOKEN
				return A;
			}else{
				// le token précédant n'est pas un nombre, exemple cette situation : "= - 3"
				isNumber = true; // token actuel est un nombre
				A.name = NUM_TOKEN;
				A.properties.number = getNumber(character, isNegative);
				return A;
			}
		}else{ // Le caractère suivant de +/- n'est pas un chiffre
			ungetc(character, fichSrc); // retourner le curseur en arrière
			A.name = symboleToken;
			return A;
		}
	}
	

	// Elimination des commentaires
	else if(character == '{'){
// Lire le caractère suivant
		character = fgetc(fichSrc);
		if(character == '{'){ // un commentaire ligne
			// dépasser tous les caractères jusqu'on arrive à \n
			do{
				character = fgetc(fichSrc);
			}while(character != '\n' && character != EOF);
			return getToken(); // rappeler la fonction
		}
		else if(character == '*'){ // un commentaire bloque
			// dépasser tous les caractères jusqu'on arrive à */
			do{
				character = fgetc(fichSrc);
			}while(character != '*');
			// Lire le caractère suivant
			character = fgetc(fichSrc);
			if(character == '}'){ // Fin du commentaire bloque
				return getToken(); // rappeler la fonction
			}
		}

	}
	// Reconaissance des symboles simples
	else if(character==';'){
		A.name = PV_TOKEN;
		return A;	}
	else if(character=='.'){
		A.name = PT_TOKEN;
		return A;	}
	else if(character=='*'){
		A.name = MULT_TOKEN;
		return A;
	}
	else if(character=='/'){
		A.name = DIV_TOKEN;
		return A;
	}
	else if(character==','){
		A.name = VIR_TOKEN;
		return A;
	}
	else if(character=='('){
		A.name = PO_TOKEN;
		return A;
	}	
	else if(character==')'){
		A.name = PF_TOKEN;
		return A;
	}
	else if(character=='='){
		A.name = EG_TOKEN;
		return A;
	}	
	/*
	Reconnaissance des symboles complexes
	*/
	// Reconnaissance de <, <>,<=
	else if(character == '<'){
		// Lire le caractère suivant
		character = fgetc(fichSrc);
		if(character == '>'){ 
			A.name = DIFF_TOKEN;
			return A;
		}else if(character=='='){
			A.name = INFEG_TOKEN;
			return A;
		}else{
			A.name = INF_TOKEN;
			ungetc(character, fichSrc);
			return A;	
    	}
    }   
	else if(character=='>'){
		character = fgetc(fichSrc);
		if(character=='='){ // On a le symbole >=
    	A.name = SUPEG_TOKEN;
		return A;}
		else{
		A.name = SUP_TOKEN;
		ungetc(character, fichSrc);
		return A;	
		}	
	}
	// Reconnaissance de >, >=
	else if(character == ':'){
		// Lire le caractère suivant
		character = fgetc(fichSrc);
		if(character == '='){ // On a le symbole :=
			A.name = AFF_TOKEN;
			return A;
		}else{ 
			A.name = ERREUR_TOKEN;
			ungetc(character, fichSrc);
			return A;
		}
	}
	// La fin de la lecture
	else if(character == EOF){
		A.name = FIN_TOKEN;
		return A;
	}
	// Si on rencontre d'autre caractères non analysés
	else{
		A.name = ERREUR_TOKEN;
		return A;
	}
}
