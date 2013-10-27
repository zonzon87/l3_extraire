/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#ifndef __OUTILS_H__
#define __OUTILS_H__

#include <stdio.h>
#include <setjmp.h>

#define TABLE_BUFFER 128
#define LINE_EOF 1
#define ERROR_MISSINGFILE 1
#define P_ERROR_MISSINGFILE(x) printf("Fichier inexistant : %s", x)

#define TRY do { jmp_buf ex_buf; switch(setjmp(ex_buf)){ case 0: while(1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } } while(0)
#define THROW(x) longjmp(ex_buf, x)


extern void libererSimple(void ** lieu);

extern void copierIntE(const void * valeur, void ** lieu);

extern void copierCharE(const void * valeur, void ** lieu);

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

extern int isInVAList(char c, int argc, ...);

extern int isNumeric(const char * str, const int strLength);

extern void removeHeadAndTailChar(char ** str, char c);


#endif
