#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "outils.h"
#include "requete.h"

#define TABLE_BUFFER 128
#define TOKENDELIMITOR "|"
#define TOKENGARBAGE ' '
#define LINE_EOF 1
#define ERROR_MISSINGFILE 1
#define P_ERROR_MISSINGFILE(x) printf("Fichier inexistant : %s\n", x)
#define ERROR_INEXISTANTCHAMP 2
#define P_ERROR_INEXISTANTCHAMP(x, y) printf("Erreur demande d'une colonne inexistante d'un fichier : %s.%d\n", x, y)
#define ERROR_MALFORMEDFILE 3
#define P_ERROR_MALFORMEDFILE(x) printf("Fichier malformé : %s\n", x)


typedef struct table {
	int row;
	file lines;
} table;


extern void destroyTable(table ** tab);

/*	Permet la copie d'une ligne d'un fichier, en supposant que le curseur de
	fichier est positionné 1 caractère avant le premier de la ligne à extraire.
	Entrée : char ** line avec (* line) non-alloué, FILE * fichier ouvert
		(non NULL).
	Sortie : char * via (* line) de taille TABLE_BUFFER * n (où est le
		minimum pour stocker la chaîne).
	- supporte \r (Mac OS) \r\n (Windows) \n (UNIX) comme marqueurs de fin de
		ligne.
	- la chaîne retournée est dépourvu du marqueur de fin de ligne lu.
	- marqueur de fin de chaîne '\0' présent.
	- garantit que (* line) soit différent de NULL.
*/
extern int getLine(char ** line, FILE * fichier);

extern int countNumberOfChamps(const char * ch, const char * delimitor);

extern int divideCharEtoiletoCharEtoileArray(charEtoileArray ** dest, int nbElements, const char * delimitor, char * src);

extern int rearrangeTableRows(charEtoileArray ** cEAOut, charEtoileArray * cEAIn, file_parcours * values, int nbValues);

extern int createTable(table ** tab, const char * fileName, file ordreApparitions, int maxValue);

extern int createTables(file * tables, requete * reqIn, requete ** reqOut);


#endif
