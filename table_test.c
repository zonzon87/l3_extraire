#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "table.h"
#include "requete.h"
#include "outils.h"

#define TABLETOPRINT 0 /* 0 désactivé / 1 activé */


int getLine_test(char * fileName) {
	int lastLine = 0;
	FILE * fichier = NULL;
	char * ch = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL)
	{
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

int countNumberOfChamps_test() {
	int returnValue = 0;
	int result;
	int useless;
	const char * delimitor = TOKENDELIMITOR;
	char stack1[] = "   alice  |  poisson  |     yourt  | ";
	char stack2[] = "   alice  |  poisson  |     yourt  |";
	char stack3[] = "   alice  |  poisson  |     yourt  ";

	char * str = NULL;

	PRINT_T(1);
	copierCharE((void *) stack1, (void **) &str);
	result = countNumberOfChamps(str, delimitor, &useless);
	if (result != 3) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &str);

	PRINT_T(2);
	copierCharE((void *) stack2, (void **) &str);
	result = countNumberOfChamps(str, delimitor, &useless);
	if (result != 3) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &str);

	PRINT_T(3);
	copierCharE((void *) stack3, (void **) &str);
	result = countNumberOfChamps(str, delimitor, &useless);
	if (result != 3) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &str);

	return returnValue;
}

int divideCharEToCharEArray_test() {
	int returnValue = 0;
	int result;
	const char * delimitor = TOKENDELIMITOR;
	char stack[] = "   alice  |  poisson  |     yourt  | ";

	char * str = NULL;
	xEArray * cEA = NULL;

	PRINT_T(1);
	copierCharE((void *) stack, (void **) &str);
	result = divideCharEToCharEArray(&cEA, 3, 4, delimitor, str);
	if (result != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
		printf("In  :\t");
		printf("\"%s\"\n", stack);
		printf("Out :\t");
		charEArrayToPrint(cEA);
	}
	libererXEArray((void **) &cEA);
	libererSimple((void **) &str);

	PRINT_T(2);
	copierCharE((void *) stack, (void **) &str);
	result = divideCharEToCharEArray(&cEA, 2, 4, delimitor, str);
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
	const char * delimitor = TOKENDELIMITOR;
	char stack[] = "alice | poisson | yourt";

	char * str = NULL;
	file_parcours values = NULL;
	requete * req = NULL;
	xEArray * cEAIn = NULL;
	xEArray * cEAOut = NULL;

	const char * cIn[] = {"./extraire", "a.1", "b.3", "a.2", "b.1", "de", "Donnees/sport.table", "Donnees/repas.table", "avec", "a.1=b.1"};
	const char * cOut = "10";

	createRequete(&req, atoi(cOut), cIn);
	copierCharE((void *) stack, (void **) &str);
	divideCharEToCharEArray(&cEAIn, 3, 3, delimitor, str);
	values = file_parcours_creer(req->tabFChamps[1]);

	rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(req->tabFChamps[1]));
	printf("cEAIn  :\t");
	charEArrayToPrint(cEAIn);
	printf("cEAOut :\t");
	charEArrayToPrint(cEAOut);
	libererXEArray((void **) &cEAOut);

	file_parcours_detruire(&values);
	libererXEArray((void **) &cEAIn);
	libererSimple((void **) &str);
	destroyRequete(&req);

	return returnValue;
}

int createTable_test() {
	int returnValue = 0;
	int result;
	requete * req = NULL;
	table * tab = NULL;
	
	const char * c1In[] = {"./extraire", "b.3", "b.3", "b.5", "b.1", "de", "Donnees/r1-1.table", "Donnees/r2-2.table", "avec", "a.1=b.2"};
	const char * c1Out = "10";
	
	PRINT_T(1);
	createRequete(&req, atoi(c1Out), c1In);
	result = createTable(&tab, "Donnees/r2-2.table", req->tabFChamps[1]);
	if (result != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		if (TABLETOPRINT) {
			tableToPrint(tab);
		}
		PRINT_T_OK();
	}
	destroyTable((void **) &tab);
	destroyRequete(&req);

	return returnValue;
}

/*void createTables_test() {
    int i;
    requete * req;
    table ** tables = NULL;

	const char * cIn[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "Donnees/sport.table", "Donnees/repas.table", "avec", "a.1=b.1"};
	const char * cOut = {"10"};

	createRequete(&req, atoi(cOut[i]), cIn[i]);

	destroyRequete(&req);

}*/

int main(void) {
	printf("getLine_test(\"Donnees/repas.table\") : \n");
	if (getLine_test("Donnees/repas.table") == 0) {
		printf("Fichier lu, vérifier les données manuellement !\n");
	}
	printf("countNumberOfChamps_test() : \n");
	if (countNumberOfChamps_test() == 0) {
		PRINT_OK();
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
	printf("createTable_test() : \n");
	if (createTable_test() == 0) {
		if (TABLETOPRINT) {
			printf("Table créée et organisée, vérifier les données manuellement !\n");
		} else {
			printf("#define TABLETOPRINT 0 -> table non-affichée.\n");
		}
	}
	/*printf("createTables_test() : Memory only\n");
    createTables_test();*/

	return 0;
}
