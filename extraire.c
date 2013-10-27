#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outils.h"
#include "requete.h"
#include "table.h"

/* Seuls les parcours sont alloués. */

int testerCondition(condition * co, xEArray * tablesL) {
    char * str1 = NULL;
    char * str2 = NULL;

    str1 = (char *) accesXEArray((xEArray *) accesXEArray(tablesL, co->champ1->table), co->champ1->row);
    str2 = (char *) accesXEArray((xEArray *) accesXEArray(tablesL, co->champ2->table), co->champ2->row);

    if (co->comparisonOperator[0] == '>') {
        char * mem = NULL;

        mem = str1;
        str1 = str2;
        str2 = mem;
        co->comparisonOperator[0] = '<';
    }
    if (strcmp(co->comparisonOperator, "<") == 0) {
        if (strcmp(str1, str2) < 0) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "<=") == 0) {
        if (strcmp(str1, str2) <= 0) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "<.") == 0) {
        if (atoi(str1) < atoi(str2)) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "<=.") == 0) {
        if (atoi(str1) <= atoi(str2)) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "!=") == 0) {
        if (strcmp(str1, str2) != 0) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "=") == 0) {
        if (strcmp(str1, str2) == 0) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "!=.") == 0) {
        if (atoi(str1) != atoi(str2)) {
            return 0;
        } else {
            return 1;
        }
    }
    if (strcmp(co->comparisonOperator, "=.") == 0) {
        if (atoi(str1) == atoi(str2)) {
            return 0;
        } else {
            return 1;
        }
    }

    return 1;
}

int testerConditions(file conditions, xEArray * tablesL) {
    int result;
    condition * co = NULL;
    file_parcours parcours = NULL;

    parcours = file_parcours_creer(conditions);
    while (!file_parcours_est_fini(parcours)) {
        file_parcours_suivant_lien(parcours, (void **) &co);
        result = testerCondition(co, tablesL);
        if (result != 0) {
            file_parcours_detruire(&parcours);
            return result;
        }
    }
    file_parcours_detruire(&parcours);

    return 0;
}

void extraireLigne(tableLine ** tL, file champsSortie, xEArray * tablesL) {
    int i = 0;
    champ * ch = NULL;
    file_parcours parcours = NULL;

    creerXEArray(tL, file_taille(champsSortie), &copierCharE, &libererSimple);
    parcours = file_parcours_creer(champsSortie);
    while (!file_parcours_est_fini(parcours)) {
        file_parcours_suivant_lien(parcours, (void **) &ch);
        ajouterXEArray(* tL, i, accesXEArray((xEArray *) accesXEArray(tablesL, ch->table), ch->row));
        i++;
    }
    file_parcours_detruire(&parcours);
}

int parcoursTablesEstFini(xEArray * parcoursTableEArray, int nbElements) {
    int i;

    for (i = 0; i < nbElements; i++) {
        if (!file_parcours_est_fini(accesXEArray(parcoursTableEArray, i))) {
            return 0;
        }
    }

    return 1;
}

void jointure(requete * req, tables * tEA) {
	int i;
	file_parcours parcours = NULL;
	/* Tableau des parcours, un parcours par table, sera alloué. */
	xEArray * parcoursTableEArray = NULL;
	/* Pointeur d'une ligne d'une table, ne sera pas alloué. */
	tableLine * tL = NULL;
	/* Pointeur de tableau qui contient une ligne de chaque table, ne sera pas alloué. */
	xEArray * tablesL = NULL;
	/* Table jointe utilisée pour la sortie. */
	table * joinTab = NULL;

    /* On initialise la table jointe. */
	file_creer(&joinTab, &copierXEArray, &libererXEArray);

	creerXEArray(&parcoursTableEArray, tEA->nbElements, NULL, &libererFile_parcours);
	for (i = 0; i < parcoursTableEArray->nbElements; i++) {
        parcours = file_parcours_creer(accesXEArray(tEA, i));
        ajouterLienXEArray(parcoursTableEArray, i, parcours);
	}

    /* On génère toutes les combinaisons possibles de lignes de chaque fichier. */
    creerXEArray(&tablesL, tEA->nbElements, NULL, &libererLienXEArray);
    /* Initialisation, pré-algorithme. */
    for (i = 0; i < tEA->nbElements - 1; i++) {
        file_parcours_suivant_lien(accesXEArray(parcoursTableEArray, i), (void **) &tL);
        ajouterLienXEArray(tablesL, i, tL);
    }
    /* Algorithme de parcours. */
    while (!parcoursTablesEstFini(parcoursTableEArray, tEA->nbElements)) {
        for (i = tEA->nbElements - 1; i >= 0; i--) {
            if (!file_parcours_est_fini(accesXEArray(parcoursTableEArray, i))) {
                file_parcours_suivant_lien(accesXEArray(parcoursTableEArray, i), (void **) &tL);
                ajouterLienXEArray(tablesL, i, tL);
                break;
            } else {
                file_parcours_reset(accesXEArray(parcoursTableEArray, i));
                file_parcours_suivant_lien(accesXEArray(parcoursTableEArray, i), (void **) &tL);
                ajouterLienXEArray(tablesL, i, tL);
            }
        }

        /* Quand on obtient une combinaison, on teste sur elle les conditions. */
        if (testerConditions(req->conditions, tablesL) == 0) {
            tableLine * joinTL = NULL;
            /* On crée une nouvelle ligne pour la table de sortie. */
            extraireLigne(&joinTL, req->champsSortie, tablesL);
            file_ajouter(joinTab, joinTL);
            libererXEArray((void **) &joinTL);
        }

    }

    /* Enfin, on affiche la table jointe ! */
    tableToPrint(joinTab);

    file_detruire(&joinTab);
    libererLienXEArray((void **) &tablesL);
    libererXEArray((void **) &parcoursTableEArray);
}

int main(int argc, const char * argv[]) {
	requete * req = NULL;
	tables * tEA = NULL;

	if (createRequete(&req, argc, argv) == 0) {
		if (createTables(&tEA, req->nomsFichiers, req->ordreApparitions) == 0) {
			jointure(req, tEA);
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
