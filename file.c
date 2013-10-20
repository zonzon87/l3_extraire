#include <stdio.h>
#include <stdlib.h>

#include "file.h"


int file_creer(file * file_lieu, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu)) {
	(* file_lieu) = (file) malloc(sizeof (file_struct));
	(* file_lieu)->head = NULL;
	(* file_lieu)->copier = copier;
	(* file_lieu)->liberer = liberer;

	return 0;
}

int file_detruire(file * list) {
	void * olddata = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	/* On retire tous les éléments de la file. */
	while (file_est_vide(* list) != FILE_ERREUR_FILE_VIDE) {
		file_retirer(* list, &olddata);
		(* ((* list)->liberer))(&olddata);
	}
	/* On détruit la file. */
	free(* list);
	(* list) = NULL;
	
	return 0;
}

int file_ajouter(file list, const void * valeur) {
	maillon m = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	m = (maillon) malloc(sizeof (maillon_struct));

	(* (list->copier))(valeur, &(m->data));

	if (file_est_vide(list) == FILE_ERREUR_FILE_VIDE) {
		m->next = NULL;
		list->head = m;
		list->tail = m;
	} else {
		m->next = NULL;
		list->tail->next = m;
		list->tail = m;
	}

	return 0;
}

int file_retirer(file list, void ** lieu) {
	maillon m = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	if (file_est_vide(list) != FILE_ERREUR_FILE_VIDE) {
		(* (list->copier))(list->head->data, lieu);
		(* (list->liberer))(&(list->head->data));

		/* Si l'on retire l'unique maillon de la file. */
		if (list->head->next == NULL) {
			free(list->head);
			list->head = NULL;
		} else {
			m = list->head->next;
			free(list->head);
			list->head = m;
		}
	}

	return 0;
}

int file_est_vide(const void * pointer) {
	/*	On attend un (void *), mais la fonction s'appliquant à une file on cast 
		ce pointeur en (file_struct *)
	*/
	file list = NULL;

	if (pointer == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	list = (file_struct *) pointer;
	if (list->head == NULL) {
		return FILE_ERREUR_FILE_VIDE;
	}

	return 0;
}

int file_taille(file list) {
	int count = 0;
	maillon m = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	m = list->head;

	if (m != NULL) {
		count = 1;
		while (m->next != NULL) {
			count++;
			m = m->next;
		}
	}

	return count;
}

file_parcours file_parcours_creer(file list) {
	file_parcours iterate = NULL;

	if (list != NULL) {
		iterate = (file_parcours) malloc(sizeof (file_parcours_struct));
		iterate->list = list;
		iterate->pointerMaillon = iterate->list->head;
	}

	return iterate;
}

void file_parcours_detruire(file_parcours * iterate) {
	free(* iterate);
	(* iterate) = NULL;
}

void file_parcours_suivant(file_parcours iterate, void ** lieu) {
	if (file_parcours_est_fini(iterate) == 0) {
		(* (iterate->list->copier))(iterate->pointerMaillon->data, lieu);
		iterate->pointerMaillon = iterate->pointerMaillon->next;
	} else {
		(* lieu) = NULL;
	}
}

int file_parcours_est_fini(file_parcours iterate) {
	if (iterate == NULL) {
		return FILE_ERREUR_NOT_A_FILE_PARCOURS;
	}
	if (iterate->pointerMaillon == NULL) {
		return 1;
	}

	return 0;
}
