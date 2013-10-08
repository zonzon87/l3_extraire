#include <stdio.h>
#include <stdlib.h>

#include "file.h"

int file_creer(file * file_lieu, void (* copier) (const void * valeur, void ** lieu), void (* liberer) (void ** lieu)) {
	(* file_lieu) = malloc(sizeof(file_struct));
	(* file_lieu)->head = NULL;
	(* file_lieu)->copier = copier;
	(* file_lieu)->liberer = liberer;

	return 0;
}

int file_detruire(file * list) {
	void * olddata;

	/* On retire tous les éléments de la file. */
	while (file_est_vide(list) != FILE_ERREUR_FILE_VIDE) {
		file_retirer((* list), &olddata);
		(* ((* list)->liberer))(&olddata);
	}
	/* On détruit la file. */
	free(* list);
	(* list) = NULL;
	list = NULL;

	return 0;
}

int file_ajouter(file list, const void * valeur) {
	maillon m;
	void * newdata;

	m = (maillon) malloc(sizeof (maillon_struct));
	if (list->head == NULL) {
		return 1;
	}
	return 0;
}

int file_retirer(file list, void ** lieu) {
/* si dernier maillon après l'avoir retirer penser à mettre head à NULL */
	return 0;
}

int file_est_vide(const file * list) {
	if ((* list)->head == NULL) {
		return FILE_ERREUR_FILE_VIDE;
	}

	return 0;
}

int file_taille(file list) {

	return 0;
}