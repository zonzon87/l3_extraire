# Les modules
MODULE := file requete table
# Fichiers en-tête
HEADERS := $(MODULE:%=%.h) 

# Compilateur
CC  :=  gcc
# Options de compilation
CFLAGS :=  -ansi -Wall -Wextra -pedantic -ggdb

.PHONY : all file_test compilation test memoire archive 

# Construction par défaut : fait construire extraire
all : 
	@echo "Disponible à la construction :"
	@echo "file_test => petit test du module file avec un test mémoire"
	@echo "compilation => compilation (ne doit produire ni erreur ni alerte)"
	@echo "test => test simple sur une requete"
	@echo "memoire => test les fuites mémoire (il ne doit pas y en avoir)"
	@echo "archive => produit la fichier pour le rendu"
	@echp "clean => nettoie le répertoire"

# Compilation : faire engendrer le programme
compilation : ./extraire 


# Règle de compilation
%.o : %.c $(HEADERS) 
	$(CC) -c $(CFLAGS) -o $@ $<

# Compilation de l'exécutable extraire
./extraire : $(MODULE:%=%.o) $(HEADERS) extraire.c 
	$(CC) $(CFLAGS) -o $@  $(MODULE:%=%.o) extraire.c 

# Répertoire de données pour les tests
DONNES_DIR := Donnees

# Répertoire où mettre les résultats
RESULTATS_DIR := Resultats
# Répertoire où sont les résultats attendus
RESULTATS_ATTENDUS_DIR := Resultats_Attendus

# Requêtes pour les tests sur extraire
TEST_REQUETE_1 := a.1 b.1 a.2 b.3 de $(DONNES_DIR)/sport.table $(DONNES_DIR)/repas.table avec a.1=b.1
TEST_REQUETE_2 := a.3 b.5 a.4 b.2 b.9 de $(DONNES_DIR)/r2-1.table $(DONNES_DIR)/r2-2.table avec a.3=b.5


# FAIRE un TEST SIMPLE
test : extraire
	@mkdir -p $(RESULTATS_DIR)
	@./extraire $(TEST_REQUETE_1) > $(RESULTATS_DIR)/requete_1.table
	@if ! diff $(RESULTATS_DIR)/requete_1.table $(RESULTATS_ATTENDUS_DIR)/requete_1.table ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo "test 1 OK" ; fi
# décommenter pour avoir un test de plus grande taille
#	@./extraire $(TEST_REQUETE_2) > $(RESULTATS_DIR)/requete_2.table
#	@if ! diff $(RESULTATS_DIR)/requete_2.table $(RESULTATS_ATTENDUS_DIR)/requete_2.table ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo "test 2 OK" ; fi


# TESTE de GESTION de la MÉMOIRE
memoire : extraire
	@valgrind --leak-check=full ./extraire $(TEST_REQUETE_1) >/dev/null
# décommenter pour avoir un test de plus grande taille
#	@valgrind --leak-check=full ./extraire $(TEST_REQUETE_2) >/dev/null


# TESTE du modle FILE avec un TEST MÉMOIRE
file_test : file_test.c file.o
	@$(CC) $(CFLAGS) -o $@ file.o file_test.c 
	@valgrind --leak-check=full ./file_test > $(RESULTATS_DIR)/file_test.output
	@if ! diff $(RESULTATS_DIR)/file_test.output $(RESULTATS_ATTENDUS_DIR)/file_test.output ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo OK ; fi


# PRODUIT l'ARCHIVE à RENDRE par MAIL
archive :
	@tar czf PASD_mini-projet.tgz Makefile *.c *.h compte-rendu.pdf

clean :
	rm *.o
