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

	/* On retire tous les éléments de la file. */
	while (file_est_vide(* list) == 0) {
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

	m = (maillon) malloc(sizeof (maillon_struct));

	(* (list->copier))(valeur, &(m->data));

	if (file_est_vide(list)) {
		list->head = m;
		m->prev = m;
		m->next = m;
	} else {
		m->prev = list->head->prev;
		m->next = list->head;
		list->head->prev->next = m;
		list->head->prev = m;
	}

	return 0;
}

int file_retirer(file list, void ** lieu) {
	maillon m = NULL;

	if (file_est_vide(list) == 0) {
		(* (list->copier))(list->head->data, lieu);
		(* (list->liberer))(&(list->head->data));
		list->head->data = NULL;

		/* Si l'on retire l'unique maillon de la file. */
		if (list->head == list->head->prev) {
			free(list->head);
			list->head = NULL;
		} else {
			m = list->head->next;
			/* Si en retirant list->head, m reste l'unique maillon */
			if (m->next == list->head) {
				m->prev = m;
				m->next = m;
			} else {
				m->prev = list->head->prev;
			}
			free(list->head);
			list->head = m;
		}
	} else {
		puts("WTF ?");
	}

	return 0;
}

int file_est_vide(const void * pointer) {
	file list = (file_struct *) pointer;

	if (list->head == NULL) {
		return FILE_ERREUR_FILE_VIDE;
	}

	return 0;
}

int file_taille(file list) {
	int count = 0;
	maillon begin = list->head;
	maillon end = list->head->prev;

	if (begin != NULL) {
		count = 1;
		while (begin != end) {
			count++;
			begin = begin->next;
		}
	}

	return count;
}

file_parcours file_parcours_creer(file list) {
	file_parcours iterate = NULL;

	iterate = malloc(sizeof (file_parcours_struct));
	iterate->list = list;
	iterate->begin = NULL;
	iterate->end = iterate->list->head->prev;

	return iterate;
}

void file_parcours_detruire(file_parcours * iterate) {
	free(* iterate);
	(* iterate) = NULL;
}

void file_parcours_suivant(file_parcours iterate, void ** lieu) {
	if (file_parcours_est_fini(iterate) == 0) {
		if (iterate->begin == NULL) {
			iterate->begin = iterate->list->head;
		} else {
			iterate->begin = iterate->begin->next;
		}
		(* (iterate->list->copier))(iterate->begin->data, lieu);
	} else {
		(* lieu) = NULL;
	}
}

int file_parcours_est_fini(file_parcours iterate) {
	if (iterate->begin == iterate->end) {
		return 1;
	}

	return 0;
}
