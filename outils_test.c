#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "outils.h"


#define PRINT_T(x) printf("\tTest %d [", x)
#define PRINT_T_OK() printf("] OK !\n")
#define PRINT_T_ERROR() printf("] Failed !\n")


int copierCharE_test() {
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
	printf("copierCharE_test() : \n");
	if (copierCharE_test() == 0) {
		PRINT_OK();
	}
	printf("removeHeadAndTailChar_test() : \n");
	if (removeHeadAndTailChar_test() == 0) {
		PRINT_OK();
	}

	return 0;
}
