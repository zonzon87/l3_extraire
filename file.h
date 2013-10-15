#ifndef __FILE_H__
#define __FILE_H__

#define FILE_ERREUR_FILE_VIDE 1

#define FILE_ERREUR_NOT_A_FILE -1


/*	ATTENTION CONFUSION POSSIBLE !!!
	file décrit une liste non un fichier !
*/

typedef struct maillon_struct {
	struct maillon_struct * prev;
	void * data;
	struct maillon_struct * next;
} maillon_struct;

typedef struct maillon_struct * maillon;

typedef struct file_struct {
	maillon head;
	void (* copier)(const void * valeur, void ** lieu);
	void (* liberer)(void ** lieu);
} file_struct;

/*	Type file
	(caché dans le corps du module).
*/
typedef struct file_struct * file;

/* Création d'une file générique.
	- file_lieu : l'emplacement de la file à créer
	- copier : sert à copier une valeur (de type inconnu, utilisation de void * pour cela, alloue de la mémoire si nécessaire)
	- liberer : sert à détruire une valeur créée avec copier (désalloue de la mémoire le cas échéant).
	* retourne 0 en cas de succès sinon un code d'erreur.
*/
extern int file_creer(file * file_lieu, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu));

/*	Détruit la file.
	DOIT être invoqué pour ne pas avoir de fuite mémoire.
	Utilise liberer.
*/
extern int file_detruire(file *);

/*	Ajoute un élément en fin de file.
	Utilise copier.
*/
extern int file_ajouter(file, const void * valeur);

/*	Retire le premier élément de la file et le met où indiqué.
	lieu : lieu où ranger la valeur
	Utilise copier et liberer.
*/
extern int file_retirer(file, void ** lieu);

/*	Dit si la file est vide.
*/
extern int file_est_vide(const void *);

/*	Retourne le nombre d'éléments dans la file.
*/
extern int file_taille(file);


/*
 *	PARCOURS DE LA file.
 *
 *	Va du haut (le plus ancien) au bas (plus récent).
 *	Permet d'accéder aux éléments ajoutés après la création.
*/

typedef struct file_parcours_struct {
	file list;
	maillon begin;
	maillon end;
} file_parcours_struct;

/*	Type servant à enregistrer le parcours
	(caché dans le corps du module).
*/
typedef struct file_parcours_struct * file_parcours;

/*	Création de parcours.
	Initialement avant le premier élément.
*/
extern file_parcours file_parcours_creer(file);

/*	Destruction du parcours.
	Appel obligatoire sinon fuite de mémoire.
*/
extern void file_parcours_detruire(file_parcours *);

/*	Sert à passer au suivant, la valeur est alors passé au second paramètre avec la fonction copier de la file.
	Permet ou non de modifier la valeur dans la file selon la nature de copier.
	Utilise copier.
	L'appelant doit faire le nécessaire pour la destruction de la valeur (p.e. en appelant liberer).
*/
extern void file_parcours_suivant(file_parcours, void ** lieu);

/*	Test pour savoir si on a atteint la fin de la file.
	Si positif, alors il n'est plus possible d'accéder à de nouveaux éléments.
*/
extern int file_parcours_est_fini(file_parcours);


#endif
