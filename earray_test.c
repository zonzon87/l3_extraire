#include <stdio.h>
#include <string.h>

#include "test.h"
#include "earray.h"
#include "outils.h"


int copierXEArray_test() {
	int i;
	xEArray * cEA1 = NULL;
	xEArray * cEA2 = NULL;

	char c1[] = "Coucou";
	char c2[] = "Salut, comment vas-tu ?";
	char c3[] = "Bonjour";

	creerXEArray(&cEA1, 3, &copierCharE, &libererSimple);
	(* (cEA1->copier))((void *) c1, (void **) &(cEA1->elements[0]));
	ajouterXEArray(cEA1, 1, (void *) c1);
	ajouterXEArray(cEA1, 1, (void *) c2);
	(* (cEA1->copier))((void *) c3, (void **) &(cEA1->elements[2]));

	copierXEArray((void *) cEA1, (void **) &(cEA2));

	PRINT_T(1);
	for (i = 0; i < cEA1->nbElements; i++) {
		if (strcmp(cEA1->elements[i], cEA2->elements[i]) != 0) {
			PRINT_T_ERROR();
			return 1;
		}
	}
	PRINT_T_OK();

	libererXEArray((void **) &cEA1);
	libererXEArray((void **) &cEA2);

	return 0;
}

int main(void) {
	printf("copierXEArray_test() : \n");
	if (copierXEArray_test() == 0) {
		PRINT_OK();
	}

	return 0;
}
