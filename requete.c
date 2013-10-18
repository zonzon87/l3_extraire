#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>
#include <string.h>

#include "requete.h"
#include "file.h"

void copierChamp(const void * valeur, void ** lieu) {
	champ * in = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *)(* lieu))->table = in->table;
	((champ *)(* lieu))->row = in->row;
}

void copierCharEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc(sizeof ((char *) valeur));
	memcpy((* lieu), valeur, sizeof ((char *) valeur));
}

void liberer(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

int base26to10(int * result, const char * str, const int strLength) {
	int i;
	int temp;

	for (i = 0; i < strLength; i++) {
		temp = (int) str[i];
		if ((temp > 96) && (temp < 123)) { /* Si le caractère est compris entre 'a' et 'z' inclus. */
			(* result) += (temp - 96) * pow(26, (strLength - i - 1));
		} else {
			return 1;
		}
	}
	(* result)--;

	return 0;
}

int isNumeric(const char * str, const int strLength) {
	int i;
	int temp;

	for (i = 0; i < strLength; i++) {
		temp = (int) str[i];
		if ((temp < 48) || (temp > 57)) { /* Si le caractère n'est pas compris entre '0' et '9' inclus, bref : n'est pas un chiffre. */
			return 1;
		}
	}

	return 0;
}

int parseSyntaxChamp(const char separatorChamp, const char * c, int * fileNumber, int * rowNumber) {
	int a;
	int length;
	int separatorChampPosition = -1;

	length = (int) strlen(c);
	for (a = 0; a < length; a++) {
		if (c[a] == separatorChamp) {
			if (separatorChampPosition < 0) {
				separatorChampPosition = a;
			} else {
				return SYNTAX_CHAMP_EXCEPTION;
			}
		}
	}

	if (!((separatorChampPosition > 0) && (separatorChampPosition < (length - 1)))) {
		return SYNTAX_CHAMP_EXCEPTION;
	}

	{
		int tokenLength;
		char * token = NULL;

		(* fileNumber) = 0;
		(* rowNumber) = 0;

		/* To comment */
		tokenLength = separatorChampPosition;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = base26to10(fileNumber, token, tokenLength);
		free(token);

		if (a != 0) {
			return SYNTAX_CHAMP_EXCEPTION;
		}

		/* To comment */
		tokenLength = length - (separatorChampPosition + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c + separatorChampPosition + 1, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = isNumeric(token, tokenLength);
		(* rowNumber) = atoi(token);
		free(token);

		if ((a != 0) || ((* rowNumber) < 1)) {
			return SYNTAX_CHAMP_EXCEPTION;
		} else {
			(* rowNumber)--;
		}
	}

	return 0;
}

requete * analyzeArgs(int argc, const char * argv[]) {
	/* rappel: argv[0] = #commande d'appel# */
	const char of[] = "de";
	const char with[] = "avec";
	const char separatorChamp='.';

	int ofPosition = -1;
	int withPosition = -1;
	requete * req = NULL;

	req = (requete *) malloc(sizeof (requete));
	file_creer(&(req->champsSortie), &copierChamp, &liberer);
	file_creer(&(req->nomsTables), &copierCharEtoile, &liberer);

	TRY {
		/* Test du nombre d'arguments. */
		if (argc < 7) { /* 6 paramètres au minimum. */
			THROW(MISSING_ARGS_EXCEPTION);
		}

		/* Vérification des mots-clés. */
		{
			int argCount = 1;

			while (argCount < argc) {
				if (strcmp(argv[argCount], of) == 0) { /* On cherche un unique mot clé "de". */
					if (ofPosition < 0) {
						ofPosition = argCount;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				if (strcmp(argv[argCount], with) == 0) { /* On cherche un unique mot clé "avec". */
					if (withPosition < 0) {
						withPosition = argCount;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				argCount++;
			}
			if 	( /* Test final */
				!(
					((ofPosition > 1) && /* Au minimum un champ à extraire. */
					(withPosition < argc - 1)) /* Au minimum une condition. */
				&&
					(withPosition - ofPosition >= 2) /* Au minimum deux fichiers entre les mots-clés. */
				)
				)
			{
				THROW(KEYWORD_EXCEPTION);
			} /* else { Tout va bien } */
		}


		/* Vérification et ajout des champs à extraire. */
		{
			int i;
			int fileNumber;
			int rowNumber;
			int pSCResult;
			champ * tempChamp = NULL;

			tempChamp = (champ *) malloc(sizeof (champ));

			for (i = 1; i < ofPosition; i++) {
				pSCResult = parseSyntaxChamp(separatorChamp, argv[i], &fileNumber, &rowNumber);
				if (pSCResult == 0) {
					tempChamp->table = fileNumber;
					tempChamp->row = rowNumber;
					file_ajouter(req->champsSortie, tempChamp);
				} else {
					free(tempChamp);
					THROW(pSCResult);
				}
			}

			free(tempChamp);
		}

		/* Enregistrement des chemins des fichiers. */
		{
			int i;

			for (i = ofPosition + 1; i < withPosition; i++) {
				file_ajouter(req->nomsTables, argv[i]);
			}
		}

		/* Vérification et ajout des conditions d'extraction. */
		{

		}

	} CATCH (MISSING_ARGS_EXCEPTION) {

	} CATCH (KEYWORD_EXCEPTION) {

	} CATCH (SYNTAX_CHAMP_EXCEPTION) {

	} CATCH (SYNTAX_CONDITION_EXCEPTION) {

	} CATCH (CONDITION_EXCEPTION) {

	} FINALLY {
		/* On clean tout si une exception a été levée. */
		if (0) {
			destroyRequete(&req);
		}
	} ETRY;

	return NULL;
}

int destroyRequete(requete ** req) {
	file_detruire(&((* req)->champsSortie));
	file_detruire(&((* req)->nomsTables));
	free(* req);
	(* req) = NULL;

	return 0;
}