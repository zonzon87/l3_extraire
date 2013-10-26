#include <stdio.h>
#include <stdlib.h>

#include "requete.h"
#include "table.h"

void jointure(requete * req, tables * tEA) {
	int i;
	file_parcours parcoursConditions = NULL;
	xEArray * parcoursTableEArray = NULL;
	xEArray * tabCEArray = NULL;

	creerXEArray(&parcoursTableEArray, tEA->nbElements, NULL, liberer);
	for (i = 0; i < parcoursTableEArray->nbElements; i++) {

	}

}

int main(int argc, const char * argv[]) {
	requete * req = NULL;
	tables * tEA = NULL;

	if (createRequete(&req, argc, argv) == 0) {
		if (createTables(&tEA, req->nomsFichiers, req->ordreApparitions) == 0) {
			tableToPrint(accesXEArray(tEA, 0));
			tableToPrint(accesXEArray(tEA, 1));
		} else {
			printf("\nProblème ! : création des tables.\n");
		}
		destroyTables(&tEA);
	} else {
		printf("\nProblème ! : création de la requête.\n");
	}
	destroyRequete(&req);

	return 0;
}
