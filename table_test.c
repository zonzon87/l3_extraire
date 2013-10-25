#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "table.h"
#include "outils.h"
#include "requete.h"


/* Vérifié. */
int getLine_test(char * fileName) {
	int lastLine = 0;
	FILE * fichier = NULL;
	char * ch = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL)
	{
		printf("%s:\n", fileName);
		printf("### BOF ###\n");
		while (lastLine != LINE_EOF) {
			lastLine = getLine(&ch, fichier);
			printf("%s\n", ch);
			libererSimple((void **) &ch);
		}
		printf("### EOF ###\n");

		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return 1;
	}

	return 0;
}

/* Vérifié. */
int divideCharEToCharEArray_test() {
	int returnValue = 0;
	const char * delimitor = "|";
	char stack[] = "alice | poisson | yourt";
	int result = 0;

	char * str = NULL;
	xEArray * cEA = NULL;

	PRINT_T(1);
	copierCharE((void *) stack, (void **) &str);
	result = divideCharEToCharEArray(&cEA, 3, delimitor, str);
	if (result != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererXEArray((void **) &cEA);
	libererSimple((void **) &str);

	PRINT_T(2);
	copierCharE((void *) stack, (void **) &str);
	result = divideCharEToCharEArray(&cEA, 2, delimitor, str);
	if (result != ERROR_MALFORMEDFILE) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &str);

	return returnValue;
}

int rearrangeLineRows_test() {
	int returnValue = 0;
	const char * delimitor = "|";
	char stack[] = "alice | poisson | yourt";

	char * str = NULL;
	file_parcours values = NULL;
	requete * req = NULL;
	xEArray * cEAIn = NULL;
	xEArray * cEAOut = NULL;

	const char * cIn[] = {"./extraire", "a.1", "b.3", "a.2", "b.1", "de", "Donnees/sport.table", "Donnees/repas.table", "avec", "a.1=b.1"};
	const char * cOut = {"10"};

	createRequete(&req, atoi(cOut), cIn);
	copierCharE((void *) stack, (void **) &str);
	divideCharEToCharEArray(&cEAIn, 3, delimitor, str);
	values = file_parcours_creer(req->tabFChamps[1]);

	rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(req->tabFChamps[1]));
	libererXEArray((void **) &cEAOut);

	file_parcours_detruire(&values);
	libererXEArray((void **) &cEAIn);
	libererSimple((void **) &str);
	destroyRequete(&req);

	return returnValue;
}

void createTables_test() {
    int i;
    requete * req;
    table ** tables = NULL;

	const char * c1In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "Donnees/sport.table", "Donnees/repas.table", "avec", "a.1=b.1"};
	const char * c1Out = {"10"};

	const char ** cIn[1];
	const char * cOut[1];
	cIn[0] = c1In;
	cOut[0] = c1Out;

	for (i = 0; i < 1; i++) {
		createRequete(&req, atoi(cOut[i]), cIn[i]);
		createTables((void **) tables, req->nomsTables, req->tabFChamps);
		destroyTables((void **) tables, file_taille(req->nomsTables));
		destroyRequete(&req);
	}
}

int main(void) {
	printf("getLine_test() : \n");
	if (getLine_test("Donnees/repas.table") == 0) {
		printf("Fichier lu, vérifier les données manuellement !\n");
	}
	printf("divideCharEToCharEArray_test() : \n");
	if (divideCharEToCharEArray_test() == 0) {
		PRINT_OK();
		printf("Ligne créée, vérifier les données manuellement !\n");
	}
	printf("rearrangeLineRows_test() : \n");
	if (rearrangeLineRows_test() == 0) {
		printf("Ligne arrangée, vérifier les données manuellement !\n");
	}
	/*printf("createTables_test() : Memory only\n");
    createTables_test();*/

	return 0;
}
