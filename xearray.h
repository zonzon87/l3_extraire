#ifndef __XEARRAY_H__
#define __XEARRAY_H__


#define XEARRAY_WRONGINDEX 1
#define XEARRAY_NOT_A_EARRAY -1


typedef struct xEArray {
	int nbElements;
	void ** elements;
	void (* copier)(const void * valeur, void ** lieu);
	void (* liberer)(void ** lieu);
} xEArray;


extern void creerXEArray(xEArray ** xEA, int nbElements, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu));

extern void * accesXEArray(xEArray * xEA, int index);

extern int ajouterXEArray(xEArray * xEA, int index, const void * valeur);

extern void copierXEArray(const void * valeur, void ** lieu);

extern void libererXEArray(void ** lieu);


#endif
