#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requete.h"
#include "file.h"

void copierChamp (const void * valeur, void ** lieu) {
	champ * in = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *)(* lieu))->c = in->c;
	((champ *)(* lieu))->c = in->n;
}

void libererChamp (void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

parameters * analyzeArgs(int argc, char * argv[]) {
	char separatorChamp='.';
	parameters * param = NULL;
	int i = 0;
	champ temp;

	param = (parameters *) malloc(sizeof (parameters));
	file_creer(&(p->champsSortie), &copierChamp, &libererChamp);

	if (argc < 6) {
		printf("requête trop courte.\n");
		exit(0);
	}

	for( i=1; argv[i][1]=='.' && (argv[i][0]=='a' || argv[i][0]=='b'); ++i ) {
		temp.c=argv[i][0];
		strtok(argv[i], &separatorChamp);
		temp.n=atoi(strtok(NULL, &separatorChamp));
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
	table1=strtok(comparaison1, separatorChamp);
	comparaison1=strtok(NULL, separatorChamp);
	chercher_noeud (ma_jointure, table1[0], atoi(strtok(comparaison1, separatorChamp)));	

	comparaison=strtok(NULL, egal);

	comparaison2=comparaison;
	table2=strtok(comparaison2, separatorChamp);
	comparaison2=strtok(NULL, separatorChamp);
	chercher_noeud (ma_jointure, table2[0], atoi(strtok(comparaison2, separatorChamp)));
	*/

  return NULL;

}
