#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "requete.h"
#include "file_extend.h"
#include "outils.h"


/* Vérifié. */
void copierChamp(const void * valeur, void ** lieu) {
	champ * ch = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *) (* lieu))->table = ch->table;
	((champ *) (* lieu))->row = ch->row;
}

/* Vérifié. */
int comparerChamp(const champ * ch1, const champ * ch2) {
	if (ch1->table > ch2->table) {
		return 1;
	} else if (ch1->table < ch2->table) {
		return -1;
	}
	if (ch1->row > ch2->row) {
		return 2;
	} else if (ch1->row < ch2->row) {
		return -2;
	}

	return 0;
}

/* Vérifié. */
void copierCondition(const void * valeur, void ** lieu) {
	condition * co = (condition *) valeur;
	(* lieu) = (condition *) malloc(sizeof (condition));

	copierChamp((void *) (co->champ1), (void **) &(((condition *) (* lieu))->champ1));
	copierCharE((void *) (co->comparisonOperator), (void **) &(((condition *) (* lieu))->comparisonOperator));
	copierChamp((void *) (co->champ2), (void **) &(((condition *) (* lieu))->champ2));
}

/* Vérifié. */
void libererCondition(void ** lieu) {
	if ((* lieu) != NULL) {
		libererSimple((void **) &(((condition *) (* lieu))->champ1));
		libererSimple((void **) &(((condition *) (* lieu))->comparisonOperator));
		libererSimple((void **) &(((condition *) (* lieu))->champ2));
		libererSimple(lieu);
	}
}

/* Vérifié. */
void newRequete(requete ** req) {
    (* req) = (requete *) malloc(sizeof (requete));
	file_creer(&((* req)->champsSortie), &copierChamp, &libererSimple);
	file_creer(&((* req)->nomsTables), &copierCharE, &libererSimple);
	file_creer(&((* req)->conditions), &copierCondition, &libererCondition);
	(* req)->option = C_NO_OPTION;
	(* req)->champOrdre = NULL;
	(* req)->tabFChamps = NULL;
}

/* Vérifié. */
void destroyRequete(requete ** req) {
	if ((* req) != NULL) {
	    int i;
        int nbFichiers = file_taille((* req)->nomsTables);

		file_detruire(&((* req)->champsSortie));
		file_detruire(&((* req)->nomsTables));
		file_detruire(&((* req)->conditions));
		libererSimple((void **) &((* req)->champOrdre));
		if ((* req)->tabFChamps != NULL) {
            for (i = 0; i < nbFichiers; i++) {
                file_detruire(&((* req)->tabFChamps[i]));
            }
        }
		libererSimple((void **) &((* req)->tabFChamps));
		libererSimple((void **) req);
	}
}

/* Vérifié. */
int base26to10(int * result, const char * str, const int strLength) {
	int i;
	int temp;

	(* result) = 0;

	for (i = 0; i < strLength; i++) {
		temp = (int) str[i];
		if ((temp > 96) && (temp < 123)) { /* Si le caractère est compris entre 'a' et 'z' inclus. */
			(* result) += (temp - 96) * pow(26, strLength - i - 1);
		} else {
			return 1;
		}
	}
	(* result)--;

	return 0;
}

/* Vérifié. */
int parseSyntaxChamp(champ ** ch, const char separatorChamp, const char * str) {
	int a;
	int length;
	int separatorChampPosition = -1;

	int tableNumber;
	int rowNumber;

	length = (int) strlen(str);
	for (a = 0; a < length; a++) {
		if (str[a] == separatorChamp) {
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

		rowNumber = 0;

		/* To comment */
		tokenLength = separatorChampPosition;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, str, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = base26to10(&tableNumber, token, tokenLength);
		libererSimple((void **) &token);

		if (a != 0) {
			return SYNTAX_CHAMP_EXCEPTION;
		}

		/* To comment */
		tokenLength = length - (separatorChampPosition + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, str + separatorChampPosition + 1, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = isNumeric(token, tokenLength);
		rowNumber = atoi(token);
		libererSimple((void **) &token);

		if ((a != 0) || (rowNumber < 1)) {
			return SYNTAX_CHAMP_EXCEPTION;
		} else {
			rowNumber--;
		}
	}

	(* ch) = (champ *) malloc(sizeof (champ));
	(* ch)->table = tableNumber;
	(* ch)->row = rowNumber;

	return 0;
}

/* Vérifié. */
int parseSyntaxCondition(condition ** co, const char separatorChamp, const char * str) {
	int a;
	int length;
	int separatorConditionPositionStart = -1;
	int separatorConditionPositionEnd = -1;

	champ * champ1 = NULL;
	char * comparisonOperator = NULL;
	champ * champ2 = NULL;

	length = (int) strlen(str);
	{
		char lastChar = '\0';
		int cOTempI = 0;
		char cOTemp[] = {'\0', '\0', '\0', '\0'};

		for (a = 0; a < length; a++) {
			if (!(separatorConditionPositionStart < 0)) {
				if	(
					(((lastChar == '<') || (lastChar == '>')) && (isInVAList(str[a], 2, '=', '.') == 0))
					||
					((lastChar == '!') && (str[a] == '='))
					||
					((lastChar == '=') && (str[a] == '.'))
					)
				{
					cOTemp[cOTempI] = str[a];
					cOTempI++;
				} else {
					separatorConditionPositionEnd = a - 1;
					break;
				}
			} else {
				if (isInVAList(str[a], 4, '<', '>', '!', '=') == 0) {
					cOTemp[cOTempI] = str[a];
					cOTempI++;
					separatorConditionPositionStart = a;
				}
			}
			lastChar = str[a];
		}
		if (!(((separatorConditionPositionStart > 0) && (separatorConditionPositionEnd > 0)) &&
			((separatorConditionPositionStart < (length - 1)) && (separatorConditionPositionEnd < (length - 1))))) {
			return SYNTAX_CONDITION_EXCEPTION;
		}
		comparisonOperator = (char *) malloc((sizeof (char)) * (cOTempI + 1));
		memcpy(comparisonOperator, cOTemp, (sizeof (char)) * cOTempI);
		comparisonOperator[cOTempI] = '\0';
	}

	{
		int tokenLength;
		char * token = NULL;

		/* To comment */
		tokenLength = separatorConditionPositionStart;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, str, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = parseSyntaxChamp(&champ1, separatorChamp, token);
		libererSimple((void **) &token);

		if (a != 0) {
			libererSimple((void **) &comparisonOperator);
			libererSimple((void **) &champ1);
			return a;
		}

		tokenLength = length - (separatorConditionPositionEnd + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, str + separatorConditionPositionEnd + 1, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = parseSyntaxChamp(&champ2, separatorChamp, token);
		libererSimple((void **) &token);

		if (a != 0) {
			libererSimple((void **) &comparisonOperator);
			libererSimple((void **) &champ1);
			libererSimple((void **) &champ2);
			return a;
		}
	}

	(* co) = (condition *) malloc(sizeof (condition));
	(* co)->champ1 = champ1;
	(* co)->comparisonOperator = comparisonOperator;
	(* co)->champ2 = champ2;

	return 0;
}

/* Vérifié. */
int initRequete(requete ** req, const int argc, const char * argv[]) {
	/* rappel: argv[0] = #commande d'appel# */
	const char separatorChamp = '.';

	int i = 1;
	int ofPosition = -1;
	int withPosition = -1;
	int optionPosition = -1;
	int maxNbTableAccess = -1;
	int coherentSort = 0; /* 0 incohérent ou 1 cohérent */
	int returnValue = 0;

    newRequete(req);

	TRY {
		/* Test du nombre d'arguments. */
		if (argc < 7) { /* 6 paramètres au minimum. */
			THROW(MISSING_ARGS_EXCEPTION);
		}

		/* Vérification des mots-clés. */
		{
			while (i < argc) {
				/* On cherche un unique mot clé "de". */
				if (strcmp(argv[i], S_OF) == 0) {
					if (ofPosition < 0) {
						ofPosition = i;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				/* On cherche un unique mot clé "avec". */
				if (strcmp(argv[i], S_WITH) == 0) {
					if (withPosition < 0) {
						withPosition = i;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				/* On cherche une unique option (facultative) :
					"ordre" ou "[unique]".
				*/
				if (strcmp(argv[i], S_ORDER) == 0) { /* "ordre" */
					if(optionPosition < 0) {
						optionPosition = i;
						(* req)->option = C_ORDER;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				if (strcmp(argv[i], S_SINGLE) == 0) { /* "[unique]" */
					if (optionPosition < 0) {
						optionPosition = i;
						(* req)->option = C_SINGLE;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				i++;
			}
			if (optionPosition < 0) { /* Si aucune option n'a été trouvée. */
				optionPosition = argc;
			}
			if 	( /* Test final */
				!( /* !( */
					((ofPosition > 1) && /* Au minimum un champ à extraire. ET */
					(withPosition < optionPosition - 1) && /* Au minimum une condition. ET ( */
					((((* req)->option == C_ORDER) && (optionPosition = argc - 2)) /* Si "ordre" un champ de tri. */
					||	/* OU */
					(((* req)->option == C_SINGLE) && (optionPosition = argc - 1)) /* Si "unique" dernier argument. */
					|| /* OU */
					((* req)->option == C_NO_OPTION))) && /* Si aucune option, rien à tester. ) ET */
					(withPosition - ofPosition > 2) /* Au minimum deux fichiers entre les mots-clés. */
				) /* ) */
				)
			{
				THROW(KEYWORD_EXCEPTION);
			} /* else { Tout va bien } */
		}

		/* Vérification syntaxique et ajout des champs à extraire.
			+ champ de tri (si nécessaire).
		*/
		{
			int pSCResult;
			champ * tempChamp = NULL;

			if ((* req)->option == C_ORDER) {
				pSCResult = parseSyntaxChamp(&tempChamp, separatorChamp, argv[optionPosition + 1]);
				if (pSCResult == 0) {
					(* req)->champOrdre = tempChamp;
					if (tempChamp->table > maxNbTableAccess) {
						maxNbTableAccess = tempChamp->table;
					}
				} else {
					THROW(pSCResult);
				}
			}
			tempChamp = NULL;
			for (i = 1; (i < ofPosition); i++) {
				pSCResult = parseSyntaxChamp(&tempChamp, separatorChamp, argv[i]);
				if (pSCResult == 0) {
					file_ajouter((* req)->champsSortie, (void *) tempChamp);
					if (tempChamp->table > maxNbTableAccess) {
						maxNbTableAccess = tempChamp->table;
					}
					if (((* req)->option == C_ORDER) && (coherentSort == 0)) {
						if (comparerChamp((* req)->champOrdre, tempChamp) == 0) {
							coherentSort = 1;
						}
					}
					libererSimple((void **) &tempChamp);
				} else {
					THROW(pSCResult);
				}
			}
		}

		/* Vérification syntaxique et ajout des conditions d'extraction. */
		{
			int pSCResult;
			condition * tempCondition = NULL;

			for (i = withPosition + 1; i < optionPosition; i++) {
				pSCResult = parseSyntaxCondition(&tempCondition, separatorChamp, argv[i]);
				if (pSCResult == 0) {
					file_ajouter((* req)->conditions, (void *) tempCondition);
					if (tempCondition->champ1->table > maxNbTableAccess) {
						maxNbTableAccess = tempCondition->champ1->table;
					}
					if (tempCondition->champ2->table > maxNbTableAccess) {
						maxNbTableAccess = tempCondition->champ2->table;
					}
					libererCondition((void **) &tempCondition);
				} else {
					THROW(pSCResult);
				}
			}
		}

		/* Enregistrement des chemins des fichiers. */
		{
			for (i = ofPosition + 1; i < withPosition; i++) {
				file_ajouter((* req)->nomsTables, (void *) argv[i]);
			}
		}

		/* Vérification des cohérences :
			- entre la première partie de chaque champ et le nombre de fichiers
			- égalité entre le champ de tri et au moins un champ à extraire
		*/
		{
			if	((maxNbTableAccess > (withPosition - ofPosition - 2)) ||
				(((* req)->option == C_ORDER) && (coherentSort != 1))
				)
			{
				THROW(INCOHERENT_REQUETE_EXCEPTION);
			}
		}
	} CATCH (MISSING_ARGS_EXCEPTION) {
		returnValue = MISSING_ARGS_EXCEPTION;
		printf("Trop peu d'arguments : (%d / 6 minimum).", argc - 1);
	} CATCH (KEYWORD_EXCEPTION) {
		returnValue = KEYWORD_EXCEPTION;
		printf("Erreur sur un ou plusieurs mots-clés.");
	} CATCH (SYNTAX_CHAMP_EXCEPTION) {
		returnValue = SYNTAX_CHAMP_EXCEPTION;
		printf("Erreur de syntaxe sur le champ (ou un des deux champs d'une condition) : \"%s\" - argv[%d]", argv[i], i);
	} CATCH (SYNTAX_CONDITION_EXCEPTION) {
		returnValue = SYNTAX_CONDITION_EXCEPTION;
		printf("Erreur de syntaxe sur la condition : \"%s\" - argv[%d]", argv[i], i);
	} CATCH (INCOHERENT_REQUETE_EXCEPTION) {
		returnValue = INCOHERENT_REQUETE_EXCEPTION;
		printf("Incohérence parmis au moins deux champs.");
	} FINALLY {
		/* On nettoie tout si une exception a été levée. */
		if (returnValue > 0) {
			destroyRequete(req);
		}
	} ETRY;

	return returnValue;
}

/* Vérifié. */
int searchRowInFile(file f, int row) {
    int i = 1;
    int * value;
    file_parcours parcours = NULL;

    parcours = file_parcours_creer(f);
    while (!file_parcours_est_fini(parcours)) {
        file_parcours_suivant(parcours, (void **) &value);
        if (row == (* value)) {
            libererSimple((void **) &value);
            file_parcours_detruire(&parcours);
            return i;
        }
        i++;
        libererSimple((void **) &value);
    }
    file_parcours_detruire(&parcours);

    /* Non trouvé. */
    return -1;
}

/* Vérifié. */
void completeChamps(file * champs, champ * ch) {
    int a;
    int result;

    a = (ch->row) - 1;
    result = searchRowInFile(champs[ch->table], a);
    if (result < 0) {
        file_ajouter(champs[ch->table], &a);
        ch->row = file_taille(champs[ch->table]);
    } else {
        ch->row = result;
    }
}

/* Vérifié */
void optimizeRequete(const requete * reqInit, requete ** reqOpt) {
	int i;
	int nbFichiers;
	file * champs = NULL;
	file_parcours parcours = NULL;

    nbFichiers = file_taille(reqInit->nomsTables);

	champs = (file *) malloc((sizeof (file_struct)) * nbFichiers);
	for (i = 0; i < nbFichiers; i++) {
		file_creer(&(champs[i]), &copierIntE, &libererSimple);
	}

    newRequete(reqOpt);

    {
        champ * ch = NULL;

        parcours = file_parcours_creer(reqInit->champsSortie);
        while (!file_parcours_est_fini(parcours)) {
            file_parcours_suivant(parcours, (void **) &ch);
            completeChamps(champs, ch);
            file_ajouter((* reqOpt)->champsSortie, ch);
            libererSimple((void **) &ch);
        }
        file_parcours_detruire(&parcours);
    }

    {
        if (reqInit->champOrdre != NULL) {
            copierChamp(reqInit->champOrdre, (void **) &((* reqOpt)->champOrdre));
            completeChamps(champs, (* reqOpt)->champOrdre);
        } else {
            (* reqOpt)->champOrdre = NULL;
        }
    }

    {
        condition * co = NULL;

        parcours = file_parcours_creer(reqInit->conditions);
        while(!file_parcours_est_fini(parcours)) {
            file_parcours_suivant(parcours, (void **) &co);
            completeChamps(champs, co->champ1);
            completeChamps(champs, co->champ2);
            file_ajouter((* reqOpt)->conditions, co);
            libererCondition((void **) &co);
        }
        file_parcours_detruire(&parcours);
    }

    {
        char * str = NULL;

        parcours = file_parcours_creer(reqInit->nomsTables);
        while(!file_parcours_est_fini(parcours)) {
            file_parcours_suivant(parcours, (void **) &str);
            file_ajouter((* reqOpt)->nomsTables, str);
            libererSimple((void **) &str);
        }
        file_parcours_detruire(&parcours);
    }

    (* reqOpt)->option = reqInit->option;

    (* reqOpt)->tabFChamps = champs;
}

/* Vérifié. */
int createRequete(requete ** req, const int argc, const char * argv[]) {
    requete * reqInit = NULL;

    if (initRequete(&reqInit, argc, argv) == 0) {
        optimizeRequete(reqInit, req);
    }
    destroyRequete(&reqInit);

    return 0;
}
