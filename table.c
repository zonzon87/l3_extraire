#include <stdio.h>
#include <stdlib.h>

#include "table.h"
#include "outils.h"


int createTable(table ** tab, char * fileName, int ordreLength, int * ordre) {
	FILE * fichier = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL)
	{
		char ** champ;

		(* tab) = (table *) malloc(sizeof (table));
		file_creer(&((* tab)->lines), &copierCharEtoileArray, &libererCharEtoileArray);

		fclose(fichier);
	} else {
		/* MINCE ALORS */
		return 1;
	}


	return 0;
}

int createTables(file * tables, requete * reqIn, requete ** reqOut) {

	return 0;
}