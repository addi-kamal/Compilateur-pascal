#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

FILE* f;
char Car_Cour;
char *pchar,*pchar_temp, chaine[20],chainePR[20];

typedef enum{
	//Mots cles :
	PROGRAM_TOKEN,CONST_TOKEN,VAR_TOKEN,BEGIN_TOKEN,END_TOKEN,IF_TOKEN,THEN_TOKEN,
	WHILE_TOKEN,DO_TOKEN,READ_TOKEN,WRITE_TOKEN,REPEAT_TOKEN,FOR_TOKEN,CASE_TOKEN,ELSE_TOKEN,
     //LES SYMBOLES SPECIAUX :
     DP_TOKEN,PV_TOKEN,PT_TOKEN,PLUS_TOKEN,MOINS_TOKEN,MULT_TOKEN,DIV_TOKEN,VIR_TOKEN,AFF_TOKEN,
     INF_TOKEN,INFEG_TOKEN,SUP_TOKEN,SUPEG_TOKEN,DIFF_TOKEN,PO_TOKEN,PF_TOKEN,FIN_TOKEN,EG_TOKEN,OF_TOKEN,UNTIL_TOKEN,
     //Autres :
     INTO_TOKEN,
     DOWNTO_TOKEN,
     ID_TOKEN,
     NUM_TOKEN,
     ERROR_TOKEN,
     COM_TOKEN,ACC_TOKEN
}CODE_LEX;


CODE_LEX Sym_Cour;

typedef enum{
	ERR_CAR_INC, ERR_FIC_VID, ERR_PROGRAM, ERR_ID, ERR_PV, ERR_PT, ERR_EGAL, ERR_NUM, ERR_CONST, ERR_VAR_BEGIN, ERR_CONST_VAR_BEGIN, ERR_BEGIN,
	ERR_END, ERR_AFF, ERR_IF, ERR_WHILE, ERR_DO, ERR_WRITE,  ERR_READ, ERR_THEN, ERR_PO, ERR_PF,ERR_MOINS, ERR_PLUS, ERR_MULT, ERR_DIV,
	ERR_COND, ERR_TERM, ERR_FACT, ERR_INF, ERR_SUP, ERR_SUPEG, ERR_INFEG, ERR_DIFF, ERR_ACC, ERR_EXPR,ERR_ELSE,ERR_REPEAT,ERR_UNTIL,ERR_FOR,ERR_CASE,
	ERR_INTO,ERR_DOWNTO,ERR_DP,ERR_TOKEN,ERR_OF,ERR_INST,ERR_NOTEXISTID,ERR_EXISTID,ERR_CONSTCH,ERR_PROGD, ERR_PLEIN, ERR_AFFECT_ABS
	}CODE_ERR;
     
typedef  struct  { 
   CODE_ERR code_erreur ;
   char message[40];
    }Erreur;
    

Erreur Erreurs[]={{ERR_CAR_INC ,"caractére inconnu." },
						{ERR_FIC_VID,"fichier est vide"},
						{ERR_PROGRAM,"Erreur dans la syntaxe PROGRAM"},
						{ERR_ID,"Erreur dans la syntaxe ID"},
						{ERR_PV," ';' manquee "},
						{ERR_PT," '.' manquee"},
						{ERR_EGAL,"Erreur dans la syntaxe '=' "},
						{ERR_NUM,"Erreur dans la syntaxe NUM"},
						{ERR_CONST,"Erreur dans la syntaxe CONST"},
						{ERR_VAR_BEGIN,"Erreur dans la syntaxe VAR BEGIN "},
						{ERR_CONST_VAR_BEGIN,"Erreur dans la syntaxe CONST VAR BEGIN"},
						{ERR_BEGIN,"Erreur dans la syntaxe  BEGIN"},
						{ERR_END,"Erreur dans la syntaxe END"},
						{ERR_AFF,"Erreur dans la syntaxe AFFECTATION"},
						{ERR_IF,"Erreur dans la syntaxe IF"},
						{ERR_WHILE,"Erreur dans la syntaxe WHILE"},
						{ERR_DO,"Erreur dans la syntaxe DO"},
						{ERR_WRITE,"Erreur dans la syntaxe WRITE"},
						{ERR_READ,"Erreur dans la syntaxe READ"},
						{ERR_THEN,"Erreur dans la syntaxe THEN"},
						{ERR_PO,"Erreur dans la syntaxe '('"},
						{ERR_PF,"Erreur dans la syntaxe ')'"},
						{ERR_MOINS,"Erreur dans la syntaxe '-'"},
						{ERR_PLUS,"Erreur dans la syntaxe '+'"},
						{ERR_MULT,"Erreur dans la syntaxe '*'"},
						{ERR_DIV,"Erreur dans la syntaxe '/'"},
						{ERR_INF,"Erreur dans la syntaxe '<'"},
						{ERR_SUP,"Erreur dans la syntaxe '>'"},
						{ERR_INFEG,"Erreur dans la syntaxe '<='"},
						{ERR_SUPEG,"Erreur dans la syntaxe '>='"},
						{ERR_COND,"Erreur dans la syntaxe COND"},
						{ERR_TERM,"Erreur dans la syntaxe TERM"},
						{ERR_FACT,"Erreur dans la syntaxe FACT"},
						{ERR_DIFF,"Erreur dans la syntaxe '<>'"},
						{ERR_ACC,"Erreur dans la syntaxe '{'"},
						{ERR_EXPR,"Erreur dans la syntaxe EXPR"},
						{ERR_REPEAT,"Erreur dans la syntaxe REPETER"},
						{ERR_UNTIL,"Erreur dans la syntaxe UNTIL"},
						{ERR_FOR,"Erreur dans la syntaxe FOR"},
						{ERR_CASE,"Erreur dans la syntaxe CASE"},
						{ERR_INTO,"Erreur dans la syntaxe INTO "},
						{ERR_DOWNTO,"Erreur dans la syntaxe DOWNTO "},
						{ERR_DP,"Erreur dans la syntaxe ':'"},
						{ERR_OF,"Erreur dans la syntaxe OF "},
						{ERR_AFF,"Erreur dans la syntaxe ':=' "},
						{ERR_ELSE,"Erreur dans la syntaxe ELSE "},
						{ERR_INST,"Erreur double ';' "},
						{ERR_NOTEXISTID,"Erreur ID inconnu"},
						{ERR_EXISTID,"Erreur ID Deja declarer "},
						{ERR_CONSTCH,"impossible de modifier un CONST type "},
						{ERR_PROGD,"impossible d'utiliser PROGRAM "},
						{ERR_PLEIN,"Erreur de PCODE "},
						{ERR_AFFECT_ABS,"manque ':='"}
					};

                  
                  
void ERROR(CODE_ERR);
void sym_suiv();
void PROGRAM();
void BLOCK();
void CONSTS();
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void COND();
void EXPR();
void TERM();
void FACT();         
void REPETER();
void POUR();
void CAS();

FILE *file;

void lire_nombre(){
  while(isdigit(Car_Cour)){
  	*pchar++ = Car_Cour;
    Car_Cour = fgetc(file);	  
  }
  if(!isalpha(Car_Cour)){
 	Sym_Cour=NUM_TOKEN;
  }
  
}

void test_symbole(CODE_LEX cl, CODE_ERR erreur){

	if(Sym_Cour==cl){
		memset (chainePR, 0, sizeof (chaine));
		strcpy(chainePR,chaine);
//		printf("***/////");
		sym_suiv();
	}
	else ERROR(erreur);
}


void PROGRAM(){
	test_symbole(PROGRAM_TOKEN,ERR_PROGRAM);
	test_symbole(ID_TOKEN,ERR_ID);
	test_symbole(PV_TOKEN,ERR_PV);
	BLOCK();
	test_symbole(PT_TOKEN,ERR_PT);
	}

void BLOCK(){
	CONSTS();
	VARS();
	INSTS();
}

void CONSTS(){
		switch(Sym_Cour){
		
		case CONST_TOKEN: sym_suiv();
						  test_symbole(ID_TOKEN,ERR_ID);
						  test_symbole(EG_TOKEN,ERR_EGAL);
						  test_symbole(NUM_TOKEN,ERR_NUM);
						  test_symbole(PV_TOKEN,ERR_PV);
						  while(Sym_Cour==ID_TOKEN){
						  	sym_suiv();
							test_symbole(EG_TOKEN,ERR_EGAL);
						  	test_symbole(NUM_TOKEN,ERR_NUM);
						  	test_symbole(PV_TOKEN,ERR_PV);
						  }break;
		
		case VAR_TOKEN:break;
		case BEGIN_TOKEN: break;
		default :ERROR(ERR_CONST);break;
	}
	
}

void VARS(){
	
	switch(Sym_Cour){
		
		case VAR_TOKEN:sym_suiv();
						test_symbole(ID_TOKEN,ERR_ID);
						while(VIR_TOKEN==Sym_Cour){
						  	sym_suiv();
						  	test_symbole(ID_TOKEN,ERR_ID);
						  }test_symbole(PV_TOKEN,ERR_PV);break;
						  
		case BEGIN_TOKEN: break;
		default : ERROR(ERR_VAR_BEGIN);break;
	}
	
}

void INSTS(){
	test_symbole(BEGIN_TOKEN,ERR_BEGIN);
	INST();
	while(Sym_Cour==PV_TOKEN){
		sym_suiv();
		INST();
	}
}


void INST(){
	
	switch(Sym_Cour){
		
		case BEGIN_TOKEN: INSTS();break;
		case ID_TOKEN:  AFFEC();break; 
						
		case IF_TOKEN: SI();break;
		case WHILE_TOKEN: TANTQUE();break;
		case WRITE_TOKEN: ECRIRE();break;
		case READ_TOKEN: LIRE();break;
		case REPEAT_TOKEN : REPETER();break;
		case FOR_TOKEN : POUR();break;
		case CASE_TOKEN : CAS();break; 
	}
}

void AFFEC(){
	sym_suiv();
	switch(Sym_Cour){
		case AFF_TOKEN: test_symbole(AFF_TOKEN,ERR_AFF);break;
		default: ERROR(ERR_AFF);break;
	}
	EXPR();
}

void TANTQUE(){
	test_symbole(WHILE_TOKEN,ERR_WHILE);
	COND();
	test_symbole(DO_TOKEN,ERR_DO);
	INST();
}


void ECRIRE(){
	test_symbole(WRITE_TOKEN,ERR_WRITE);
	test_symbole(PO_TOKEN,ERR_PO);
	EXPR();
	while(Sym_Cour==VIR_TOKEN){
		sym_suiv();
		EXPR();
	}
	test_symbole(PF_TOKEN,ERR_PF);
}

void LIRE(){
	test_symbole(READ_TOKEN,ERR_READ);
	test_symbole(PO_TOKEN,ERR_PO);
	test_symbole(ID_TOKEN,ERR_ID);
	while(Sym_Cour==VIR_TOKEN){
		sym_suiv();
		test_symbole(ID_TOKEN,ERR_ID);
	}
	test_symbole(PF_TOKEN,ERR_PF);
}



void REPETER(){
	test_symbole(REPEAT_TOKEN,ERR_REPEAT);
	INST();
	test_symbole(UNTIL_TOKEN,ERR_UNTIL);
	COND();
}


void POUR(){
		test_symbole(FOR_TOKEN,ERR_FOR);
		test_symbole(ID_TOKEN,ERR_ID);
		test_symbole(AFF_TOKEN,ERR_AFF);
		test_symbole(NUM_TOKEN,ERR_NUM);
		switch(Sym_Cour){
			case INTO_TOKEN: test_symbole(INTO_TOKEN,ERR_INTO);break;
			default : DOWNTO_TOKEN : test_symbole(DOWNTO_TOKEN,ERR_DOWNTO);break;	
		}
		test_symbole(NUM_TOKEN,ERR_NUM);
		test_symbole(DO_TOKEN,ERR_DO);
		INST();
}


void  CAS(){
 	test_symbole(CASE_TOKEN,ERR_CASE);
 	test_symbole(ID_TOKEN,ERR_ID);
 	test_symbole(OF_TOKEN,ERR_OF);
 	test_symbole(NUM_TOKEN,ERR_NUM);
	test_symbole(DP_TOKEN,ERR_DP);
 	 	INST();
 	while(Sym_Cour == NUM_TOKEN){
 		sym_suiv();
 		test_symbole(DP_TOKEN,ERR_DP);
 	 	INST();
	 }
	 switch(Sym_Cour){
	 	case ELSE_TOKEN : test_symbole(ELSE_TOKEN,ERR_ELSE);
	 					  INST();break;
	 	default: break;
	 }
	 
	 test_symbole(END_TOKEN,ERR_END);	
 	
 }

void COND(){
	EXPR();
	switch(Sym_Cour){
		case EG_TOKEN:     test_symbole(EG_TOKEN,ERR_EGAL);break;
		case DIFF_TOKEN:   test_symbole(DIFF_TOKEN,ERR_DIFF);break;
		case INF_TOKEN:    test_symbole(INF_TOKEN,ERR_INF);break;
		case SUP_TOKEN:    test_symbole(SUP_TOKEN,ERR_SUP);break;
		case INFEG_TOKEN : test_symbole(INFEG_TOKEN,ERR_INFEG);break;
		case SUPEG_TOKEN : test_symbole(SUPEG_TOKEN,ERR_SUPEG);break;
		default: ERROR(ERR_COND); break;
	}
	EXPR();
}



void EXPR() {  
  TERM();   
   while(Sym_Cour==PLUS_TOKEN||Sym_Cour==MOINS_TOKEN){   
		    sym_suiv();      
			TERM();    
   }
}

void TERM() {
	FACT(); 
	while(Sym_Cour==MULT_TOKEN||Sym_Cour==DIV_TOKEN) {
		sym_suiv();
		FACT();
	} 
}


void FACT() {   
	switch(Sym_Cour) {        
		case ID_TOKEN:  sym_suiv();break;  
					  
		case NUM_TOKEN:  sym_suiv(); break;   
						 
		case PO_TOKEN: sym_suiv(); EXPR();  break;
		case PF_TOKEN: sym_suiv(); break;
   }
}
void SI(){
	test_symbole(IF_TOKEN,ERR_IF);
	COND();
	test_symbole(THEN_TOKEN,ERR_THEN);
	INST();
}
void ERROR(CODE_ERR erreur){
		printf("ERREUR[%d]: %s \n",Erreurs[erreur].code_erreur,Erreurs[erreur].message);
		exit(0);
}


void lire_mot(){
  while(isdigit(Car_Cour) || isalpha(Car_Cour)){
  	*pchar++ = Car_Cour;
    Car_Cour = fgetc(file); 
  }
 // printf("%s",chaine);
  if(!stricmp(chaine,"PROGRAM")) Sym_Cour = PROGRAM_TOKEN;
  else if(!stricmp(chaine,"begin")) Sym_Cour = BEGIN_TOKEN;
  else if(!stricmp(chaine,"end")) Sym_Cour = END_TOKEN;
  else if(!stricmp(chaine,"if")) Sym_Cour = IF_TOKEN;
  else if(!stricmp(chaine,"then")) Sym_Cour = THEN_TOKEN;
  else if(!stricmp(chaine,"else")) Sym_Cour = ELSE_TOKEN;
  else if(!stricmp(chaine,"read")) Sym_Cour = READ_TOKEN;
  else if(!stricmp(chaine,"write")) Sym_Cour = WRITE_TOKEN;
  else if(!stricmp(chaine,"const")) Sym_Cour = CONST_TOKEN;
  else if(!stricmp(chaine,"var")) Sym_Cour = VAR_TOKEN;
  else if(!stricmp(chaine,"while")) Sym_Cour = WHILE_TOKEN;
  else if(!stricmp(chaine,"do")) Sym_Cour = DO_TOKEN;
  else Sym_Cour = ID_TOKEN;
  
}




void sym_suiv(){
	memset (chaine, 0, sizeof (chaine));
	pchar = chaine;
  while( isspace(Car_Cour) && Car_Cour != EOF){
    Car_Cour = fgetc(file);
}
	*pchar = Car_Cour;

  if(isdigit(Car_Cour))lire_nombre();
  else {
    if(isalpha(Car_Cour))lire_mot();
    else {
      switch (Car_Cour){
        case '(' : Sym_Cour = PO_TOKEN; break;
        case ')' : Sym_Cour = PF_TOKEN; break;
        case ';' : Sym_Cour = PV_TOKEN; break;
        case ',' : Sym_Cour = VIR_TOKEN; break;
        case '=' : Sym_Cour = EG_TOKEN; break;
        case '+' : Sym_Cour = PLUS_TOKEN; break;
        case '-' : Sym_Cour = MOINS_TOKEN; break;
        case '*' : Sym_Cour = MULT_TOKEN; break;
        case '/' : Sym_Cour = DIV_TOKEN; break;
        case '.' : Sym_Cour = PT_TOKEN; break;
        case ':' : Car_Cour = fgetc(file);
        
                  if(Car_Cour == '=') Sym_Cour = AFF_TOKEN; 
                  else Sym_Cour = DP_TOKEN; break;
        case '>' : Car_Cour = fgetc(file); 
                  	if(Car_Cour == '=') Sym_Cour = SUPEG_TOKEN;
                  	else Sym_Cour = SUP_TOKEN; break;
        case '<' : Car_Cour = fgetc(file);
                  	if(Car_Cour == '=') Sym_Cour = INFEG_TOKEN;
                  	else if (Car_Cour == '>') Sym_Cour = DIFF_TOKEN;
                        else Sym_Cour = INF_TOKEN; break;
        case '{' : Car_Cour=fgetc(file);
        			if(Car_Cour=='*'){
						while(Car_Cour!=EOF){
        					if(Car_Cour=='*'){
        						Car_Cour=fgetc(file);
        						if(Car_Cour=='}'){
        							Sym_Cour = COM_TOKEN;break;
								}
								else{Car_Cour=fgetc(file);}}
							else {
							Car_Cour=fgetc(file);}}}	
					else{
						Sym_Cour = ACC_TOKEN;
					}break;
        case EOF : Sym_Cour = FIN_TOKEN;break;
        
        default:
        //	ERROR(ERREUR_PROGRAM);
        	exit(0);
      }
      if(Sym_Cour != ERROR_TOKEN && Sym_Cour != INF_TOKEN && Sym_Cour != SUP_TOKEN && Sym_Cour != FIN_TOKEN) Car_Cour = fgetc(file);
    //	printf("%s  \n",deter_Token_code(Sym_Cour));
    }
  }
}


int main() {
	int i;
	  file = fopen("myfile.txt","r+");
	  Car_Cour = fgetc(file);
	  if(Car_Cour==EOF){
	  	printf("fichier vide !!!!");
	        	exit(0);
	  }
	  
	    sym_suiv();
	    PROGRAM();
	  printf("BRAVO: le programe est correct!! \n"); 

	  return 0;
}



