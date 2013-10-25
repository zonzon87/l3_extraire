#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "earray.h"
#include "outils.h"

#define TOKENDELIMITOR "|"
#define TOKENGARBAGE ' '
#define TABLE_BUFFER 128
#define LINE_EOF 1
#define ERROR_MISSINGFILE 1
#define P_ERROR_MISSINGFILE(x) printf("Fichier inexistant : %s\n", x)
#define ERROR_INEXISTANTCHAMP 2
#define P_ERROR_INEXISTANTCHAMP(x, y) printf("Erreur demande d'une colonne inexistante d'un fichier : %s >%d\n", x, y)
#define ERROR_MALFORMEDFILE 3
#define P_ERROR_MALFORMEDFILE(x) printf("Fichier malformé : %s\n", x)


typedef struct table {
	int nbRows;
	file lines;
} table;


extern void destroyTable(void ** tab);

extern void newTables(void ** tabs, int nbTables);

extern void destroyTables(void ** tabs, int nbTables);

/*	Permet la copie d'une ligne d'un fichier, en supposant que le curseur de
	fichier est positionné 1 caractère avant le premier de la ligne à extraire.
	Entrée : char ** line avec (* line) non-alloué, FILE * fichier ouvert
		(non NULL).
	Sortie : char * via (* line) de taille TABLE_BUFFER * n (où est le
		minimum pour stocker la chaîne).
	- supporte \r (Mac OS) \r\n (Windows) \n (UNIX) comme marqueurs de fin de
		ligne.
	- la chaîne retournée est dépourvu du marqueur de fin de ligne lu.
	- marqueur de fin de chaîne '\0' toujours présent.
	- garantit que (* line) soit différent de NULL.
*/
extern int getLine(char ** line, FILE * fichier);

extern int countNumberOfChamps(char * str, const char * delimitor);

extern int divideCharEToCharEArray(xEArray ** dest, int nbElements, const char * delimitor, char * src);

extern int rearrangeLineRows(xEArray ** cEAOut, xEArray * cEAIn, const file_parcours values, int nbValues);

extern int createTable(table * tab, const char * fileName, const file ordreApparitions);

extern int createTables(xEArray * tEA, const file nomsTables, const file * tabFChamps);

extern void charEArrayToPrint(xEArray * cEA);

extern void tableToPrint(table * tab);


#endif
