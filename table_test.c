#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "table.h"
#include "outils.h"
#include "requete.h"


int getLine_test(char * fileName) {
	int lastLine = 0;
	FILE * fichier = NULL;
	char * ch = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL)
	{
		printf("%s:\n", fileName);
		while (lastLine != LINE_EOF) {
			lastLine = getLine(&ch, fichier);
			printf("%s\n", ch);
			libererSimple((void **) &ch);
		}

		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return 1;
	}

	return 0;
}

void createTables_test() {
    int i;
    requete * req;
    table * tables = NULL;

	const char * c1In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "Donnees/sport.table", "Donnees/repas.table", "avec", "a.1=b.1"};
	const char * c1Out[] = {"10"};

	const char ** cIn[14];
	const char ** cOut[14];
	cIn[0] = c1In;
	cOut[0] = c1Out;

	for (i = 0; i < 1; i++) {
		createRequete(&req, atoi(cOut[i][0]), cIn[i]);
		createTables(&tables, req->nomsTables, req->tabFChamps);
		destroyTables(&tables, file_taille(req->nomsTables));
		destroyRequete(&req);
	}
}

int main(void) {
	printf("getLine_test() : \n");
	if (getLine_test("Donnees/repas.table") == 0) {
		printf("Fichier lu, vérifier les données manuellement!\n");
	}
	printf("createTables_test() : Memory only\n");
    createTables_test();

	return 0;
}
