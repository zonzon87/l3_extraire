#ifndef REQUETE
#define REQUETE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "file.c"


typedef struct champ {
	char c;
	int n;
} champ;

typedef struct parameters {
	file champsSortie;
	char * fichier1;
	char * fichier2;
	champ * champ1;
	champ * champ2;
} parameters;


void copier (const void * valeur, void ** lieu) {
	champ * in = (champ *) valeur;
	* lieu = (champ *) malloc(sizeof (champ));
	(* lieu)->c = in->c;
	/*(* lieu)->n = in->n; */
}

void liberer ( void ** lieu) {
	free(*lieu);
}

  
int main (int argc, char * argv[]) {

	char point[]=".";
	char egale[]="=";

	int i=0;
	champ temp;
 	parameters * p = (parameters *) malloc(sizeof (parameters));
	file_creer(&(p->champsSortie), &copier, &liberer);

  

  if (argc<7) {
    printf("requête trop courte.\n");
    exit(0);
  }

 
  for( i=1; argv[i][1]=='.' && (argv[i][0]=='a' || argv[i][0]=='b'); ++i ) {
			temp.c=argv[i][0];
			strtok(argv[i], point);
			temp.n=atoi(strtok(NULL, point));
      file_ajouter(p->champsSortie, &temp);
  }

  if (strcmp(argv[i], "de")!=0) {
    printf("echec comparaison : de en %d\n", i);
    exit(0);
  }

  if (strcmp(argv[i+3], "avec")!=0) { 
    printf("echec comparaison : avec en %d\n", i+3);
    exit(0);
  }

  
  

  

  /*
  comparaison=strtok(argv[i+4], egal);
  
  comparaison1=comparaison;
  table1=strtok(comparaison1, point);
  comparaison1=strtok(NULL, point);
  chercher_noeud (ma_jointure, table1[0], atoi(strtok(comparaison1, point)));	
 
  comparaison=strtok(NULL, egal);

  comparaison2=comparaison;
  table2=strtok(comparaison2, point);
  comparaison2=strtok(NULL, point);
  chercher_noeud (ma_jointure, table2[0], atoi(strtok(comparaison2, point)));
  */


  return 0;

}


































#endif 
