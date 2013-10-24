#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outils.h"


#define PRINT_T(x) printf("\tTest %d [", x)
#define PRINT_T_OK() printf("] OK !\n")
#define PRINT_T_ERROR() printf("] Failed !\n")


int copierCharEtoile_test() {
	char c1[] = "Bonjour, comment vas-tu ?";
	void * c2 = NULL;

	copierCharE(c1, &c2);

	PRINT_T(1);
	if (strcmp(c1, c2) != 0) {
		PRINT_T_ERROR();
		return 1;
	}
	PRINT_T_OK();

	libererSimple((void **) &c2);

	return 0;
}

int copierCharEtoileArray_test() {
	int i;
	xEArray * cEA1 = NULL;
	xEArray * cEA2 = NULL;

	char c1[] = "Coucou";
	char c2[] = "Salut, comment vas-tu ?";
	char c3[] = "Bonjour";

	cEA1 = (xEArray *) malloc(sizeof (xEArray));
	cEA1->nbChs = 3;
	cEA1->chs = (char **) malloc((sizeof (char *)) * cEA1->nbChs);
	copierCharE((void *) c1, (void **) &(cEA1->chs[0]));
	copierCharE((void *) c2, (void **) &(cEA1->chs[1]));
	copierCharE((void *) c3, (void **) &(cEA1->chs[2]));

	copierXEArray((void *) cEA1, (void **) &(cEA2));

	PRINT_T(1);
	for (i = 0; i < cEA1->nbChs; i++) {
		if (strcmp(cEA1->chs[i], cEA2->chs[i]) != 0) {
			PRINT_T_ERROR();
			return 1;
		}
	}
	PRINT_T_OK();

	libererXEArray((void **) &cEA1);
	libererXEArray((void **) &cEA2);

	return 0;
}

int removeHeadAndTailChar_test() {
	char c1[] = "---Bonjour, comment vas-tu ?-------";
	char * c2;

	copierCharE((void *) c1, (void **) &c2);
	removeHeadAndTailChar(&c2, '-');

	PRINT_T(1);
	if (strcmp("Bonjour, comment vas-tu ?", c2) != 0) {
		PRINT_T_ERROR();
		return 1;
	}
	PRINT_T_OK();

	libererSimple((void **) &c2);

	return 0;
}

int main(void) {
	printf("copierCharEtoile_test() : \n");
	if (copierCharEtoile_test() == 0) {
		printf("OK !\n");
	}
	printf("copierCharEtoileArray_test() : \n");
	if (copierCharEtoileArray_test() == 0) {
		printf("OK !\n");
	}
	printf("removeHeadAndTailChar_test() : \n");
	if (removeHeadAndTailChar_test() == 0) {
		printf("OK !\n");
	}

	return 0;
}
