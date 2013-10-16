#include <string.h>

#include "requete.h"

int main(void) {
	
	return 0;
}

int test_copierCharEtoile(void) {
	char c1[] = "Bonjour";
	void * c2;
	copierCharEtoile(c1, &c2);
	c2 = (char *) c2;
	if (strcmp(c1, c2) != 0) {
		return 1;
	}

	return 0;
}