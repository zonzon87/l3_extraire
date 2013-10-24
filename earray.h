#ifndef __EARRAY_H__
#define __EARRAY_H__


#define EARRAY_WRONGINDEX 1
#define EARRAY_NOT_A_EARRAY -1


typedef struct xEArray {
	int nbChs;
	void ** chs;
	void (* copier)(const void * valeur, void ** lieu);
	void (* liberer)(void ** lieu);
} xEArray;


extern void creerXEArray(xEArray ** xEA, int nbElements, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu));

extern void * accesXEArray(xEArray * xEA, int index);

extern int ajouterXEArray(xEArray * xEA, int index, const void * valeur);

extern void copierXEArray(const void * valeur, void ** lieu);

extern void libererXEArray(void ** lieu);


#endif
