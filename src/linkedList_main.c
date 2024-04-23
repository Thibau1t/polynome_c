/**
 * @file linkedList_main.c
 * @brief Programme pour les tests de fonctions de gestion de liste chainee
 * ATTENTION : Il faut creer autant de tests que les cas d'execution pour chaque fonction a tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "valCell.h"
#include "teZZt.h"

BEGIN_TEST_GROUP(linkedList)

TEST(monom_degree_cmp) {
    monom_t v1 = {5.11, 7};
    monom_t v2 = {3., 5};
    monom_t v3 = {5.25, 7};

    printf("\nComparaison des monomes : \n");
    CHECK( monom_degree_cmp(&v1, &v2) > 0 );
    CHECK( monom_degree_cmp(&v2, &v1) < 0 );
    CHECK( monom_degree_cmp(&v1, &v3) == 0 );
}

TEST(monom_save2file) {
    monom_t v = {5., 7};

    // creation du flux de texte => buffer
    char buffer[1024];
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file);
    // ecritdans buffer
    monom_save2file(file, &v);
    // ferme buffer
    fclose(file);
    
    CHECK( 0 == strcmp(buffer, "5.000 7\n") ); 
}



TEST(LL_init_list) {
    cell_t *list;

    printf("\nInitialization of the linked list : \n");
    LL_init_list(&list); // init la liste a NULL

    REQUIRE ( list == NULL );
}


TEST(LL_create_cell) { // test de creation de cellule
    cell_t *new = NULL;
    monom_t m1 = {3.245, 17};

    printf("\nCreate a new cell (3.245 17) : \n");
    new = LL_create_cell(&m1);
    REQUIRE ( NULL != new ); // si allocation
    CHECK ( NULL == new->next );
    // creer un buffer
    char buffer[1024];
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file);
    // sauvegarde dans le buffer
    monom_save2file(file, &(new->val));
    fclose(file);
    // verifie 
    CHECK( 0 == strcmp(buffer, "3.245 17\n") );
    // libere le monome creer
    LL_del_cell(&new);
}

// test d'insertion de cellule - liste a une cellule
TEST(LL_add_cell1) { 
    cell_t *list = NULL;
    cell_t *new = NULL;
    monom_t m1 = {3.45, 17};

    printf("\nAdd a cell to a linked list : \n");
    // creer cellule
    new = LL_create_cell(&m1);
    REQUIRE ( new != NULL );
    // ajoute cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new ); 
    // verification des valeurs
    CHECK( list->val.coef == 3.45 );  // 3.45 est une valeur non approchee
    CHECK( list->val.degree == 17 );  
    CHECK( list->next == NULL );

    free(list); // liberer la liste
    list = NULL;
}

// test d'insertion de cellule - liste a deux cellules
TEST(LL_add_cell2) { 
    cell_t *list = NULL;
    cell_t *new  = NULL;
    monom_t m1 = {3.45, 17};
    monom_t m2 = {25.8,  9};

    printf("\nAdd two cells to a linked list : \n");
    // creer une cellule
    new = LL_create_cell(&m1);
    REQUIRE ( new != NULL );
    // ajoute la cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // creer une nouvelle cellule
    new = LL_create_cell(&m2);
    REQUIRE ( new != NULL );
    // ajoute la cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // tester les valeurs de la liste sans buffer (le prochain test buffer)
    // m1 :
    CHECK( list->next->val.coef == 3.45); 
    CHECK( list->next->val.degree == 17 );  
    CHECK( list->next->next == NULL ); // fin liste 

    // m2 :
    CHECK( list->val.coef == 25.8); 
    CHECK( list->val.degree == 9 );  

    // liberer la liste
    LL_free_list(&list);
}

// test d'insertion de cellule - liste a trois cellules
TEST(LL_add_cell3) { 
    cell_t *list = NULL;
    cell_t *new = NULL;
    monom_t m1 = {3.245, 17};
    monom_t m2 = {25.8, 9};
    monom_t m3 = {12.4, 3};

    printf("\nAdd three cells to a linked list : \n");
    // creer cellule
    new = LL_create_cell(&m1);
    REQUIRE ( new != NULL );
    // ajoute la cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // creer nouvelle cellule
    new = LL_create_cell(&m2);
    REQUIRE ( new != NULL );
    // ajoute la cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // creer nouvelle cellule
    new = LL_create_cell(&m3);
    REQUIRE ( new != NULL );
    // ajoute la cellule en tete de liste
    LL_add_cell(&list, new);
    CHECK( list == new );

    // tester les valeurs de la liste
    // creation du flux de texte => buffer
    char buffer[1024];
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file);

    LL_save_list_toFile(file, list, &monom_save2file);

    fclose(file);

    CHECK( 0 == strcmp(buffer, "12.400 3\n25.800 9\n3.245 17\n") );
    
    // liberer la liste
    LL_free_list(&list);
}


// test pour la creation d'une liste a partir d'un fichier
TEST(LL_create_list_fromFileName0)
{
    cell_t *list;

    printf("\nCreate a linked list from file name0: \n");
    // creation buffer
    char buffer[1024];
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list, "data/listeChaineeTest.txt", monom_degree_cmp);
    CHECK( NULL != list );

    // enregistre dans buffer pour check
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);
	
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // libere liste
    LL_free_list(&list);
}

// test pour la creation d'une liste a partir d'un fichier qui n'existe pas
TEST(LL_create_list_fromFileName1)
{
    cell_t *list;
    ;
    LL_create_list_fromFileName(&list, "data/listeChaineeTestQuiNexistePas.txt", monom_degree_cmp);

    CHECK( NULL == list ); // verifie liste vide
}


TEST(LL_save_list_toFile0) { // test pour l'ecriture d'un polynome sur un flux de
			    // sortie
    // 1. on creer une liste depuis un fichier connu
    // 2. on l'enregistre dans un buffer
    // 3. on utilise strcmp pour vérif la bonne sauvegarde
    
    cell_t *list;

    char buffer[1024];
    FILE * file= NULL;

    // // 1. on creer une liste depuis un fichier connu
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);


    // 2. on l'enregistre dans un buffer
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // 3. on utilise strcmp pour vérif la bonne sauvegarde
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // liberation
    LL_free_list(&list);
}

TEST(LL_save_list_toFile1) { // test pour l'ecriture d'un polynome vide sur un flux de
			    // sortie
    // 1. on creer une liste depuis un fichier connu
    // 2. on l'enregistre dans un buffer
    // 3. on utilise strcmp pour vérif la bonne sauvegarde
    
    cell_t *list;

    char buffer[1024];
    FILE * file= NULL;

    // // 1. on creer une liste depuis un fichier connu
    file = fmemopen(buffer, 1024, "w");
    buffer[0] = 'a'; buffer[1] = '\0'; // init buffer
    
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTestQuinexistepas.txt",
				monom_degree_cmp);

    // 2. on l'enregistre dans un buffer
    LL_save_list_toFile(file, list, monom_save2file); // ne fait rien car la
                                                      //liste est null
    
    CHECK(list == NULL);
    // ne fait rien
    LL_save_list_toFile(file, list, monom_save2file);

    // 3. on utilise strcmp pour vérif qu'il n'y a pas eu de sauvegarde car list=NULL
    CHECK( 0 == strcmp(buffer, "a"));

    // liberation
    LL_free_list(&list);

    fclose(file);
}

TEST(LL_search_prev1) {
    // test pour la fonction de recherche d'une valeur se trouvant au milieu
    cell_t *list, **prec;
    char buffer[1024];
    char bufferPrec[32];
    
    FILE * file= NULL;
    monom_t maillon_recherche = {5.000 , 3};

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // recuperation maillon precedent
    prec = LL_search_prev(&list, &maillon_recherche, monom_degree_cmp);

    // enregistrement du suivant du prec dans le buffer (donc celui que l'on recherche)
    file = fmemopen(bufferPrec, 32, "w");
    REQUIRE ( NULL != file );
    monom_save2file(file, &(*prec)->val);
    fclose(file);
    
    // verification bon maillon
    CHECK (0 == strcmp(bufferPrec, "5.000 3\n"));
    // liberation
    LL_free_list(&list);
}


TEST(LL_search_prev2) {
// test pour la fonction de recherche d'une valeur se trouvant en tete
    cell_t *list, **prec;
    char buffer[1024];
    char bufferPrec[32];
    
    FILE * file= NULL;
    monom_t maillon_recherche = {5.000 , 1};

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // recuperation maillon precedent
    prec = LL_search_prev(&list, &maillon_recherche, monom_degree_cmp);

    // enregistrement du suivant du prec dans le buffer (donc celui que l'on recherche)
    file = fmemopen(bufferPrec, 32, "w");
    REQUIRE ( NULL != file );
    monom_save2file(file, &(*prec)->val);
    fclose(file);
    
    // verification bon maillon
    CHECK (0 == strcmp(bufferPrec, "5.000 1\n"));
    // liberation
    LL_free_list(&list);
}


TEST(LL_search_prev3) {
// test pour la fonction de recherche d'une valeur se trouvant a la fin
    cell_t *list, **prec;
    char buffer[1024];
    char bufferPrec[32];
    
    FILE * file= NULL;
    monom_t maillon_recherche = {6.000 , 4};

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // recuperation maillon precedent
    prec = LL_search_prev(&list, &maillon_recherche, monom_degree_cmp);

    // enregistrement du suivant du prec dans le buffer (donc celui que l'on recherche)
    file = fmemopen(bufferPrec, 32, "w");
    REQUIRE ( NULL != file );
    monom_save2file(file, &(*prec)->val);
    fclose(file);
    
    // verification bon maillon
    CHECK (0 == strcmp(bufferPrec, "6.000 4\n"));
    // liberation
    LL_free_list(&list);
}



TEST(LL_add_celln) {
// test d'insertion de cellule - liste a n-1 cellules
    cell_t *new , *list = NULL ;
    char buffer[1024];
    FILE * file= NULL;
    int i, n = 5;
    monom_t tmp;
    
    for (i = n-1; i > 0; --i)
    {
	tmp.degree = i;
	tmp.coef   = i;
	
        new = LL_create_cell(&tmp);

	LL_add_cell(&list, new);
    }
    
    // verifie bonne recuperation
    memset(buffer, 0, sizeof(buffer));
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bon ajout
    CHECK( 0 == strcmp(buffer, "1.000 1\n2.000 2\n3.000 3\n4.000 4\n"));

    // liberation
    LL_free_list(&list);
    
}


TEST(LL_del_cell1) { // test de la suppression d'un element au milieu

    cell_t *list;

    char buffer[1024];
    char buffer2[1024];
    
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // supprime le deuxieme maillon
    LL_del_cell(&(list->next));


    // enregistrement buffer 2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    
    // comparaison
    CHECK(0 == strcmp(buffer2, "5.000 1\n5.000 3\n6.000 4\n"));
    
    // liberation
    LL_free_list(&list);
    // vrai test avec valgrind
    CHECK(NULL == list);
}


TEST(LL_del_cell2) { // test de la suppression d'un element en tete

    cell_t *list;

    char buffer[1024];
    char buffer2[1024];
    
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // supprime le deuxieme maillon
    LL_del_cell(&(list));


    // enregistrement buffer 2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    
    // comparaison
    CHECK(0 == strcmp(buffer2, "4.000 2\n5.000 3\n6.000 4\n"));
    
    // liberation
    LL_free_list(&list);
    // vrai test avec valgrind
    CHECK(NULL == list);
}



TEST(LL_del_cell3) { // test de la suppression d'un element en fin de liste

    cell_t *list;

    char buffer[1024];
    char buffer2[1024];
    
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // supprime le deuxieme maillon
    LL_del_cell(&(list->next->next->next));


    // enregistrement buffer 2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    
    // comparaison
    CHECK(0 == strcmp(buffer2, "5.000 1\n4.000 2\n5.000 3\n"));
    
    // liberation
    LL_free_list(&list);
    // vrai test avec valgrind
    CHECK(NULL == list);
}

TEST(LL_free_list0) { // test de la liberation de liste
    cell_t *list = NULL;
    char buffer[1024];
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK( 0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // liberation
    LL_free_list(&list);

    // vrai test avec valgrind
    CHECK(NULL == list);
}

TEST(LL_free_list1) { // test liberation sur une liste vide ne provoque pas d'erreur
    cell_t *list = NULL;
    
    // liberation
    LL_free_list(&list);

    // vrai test avec valgrind
    CHECK(NULL == list);
}

TEST(LL_save_list_toFileName0){
    cell_t *list  = NULL;
    cell_t *new = NULL;
    monom_t m1 = {3.245, 17}; 
    monom_t m2 = {25.8, 9};
    monom_t m3 = {12.4, 3};

    FILE *fptr;
    char buffer[1024];
    char * buffer_alloc;
    long file_size;
    char nom_fichier[] = "fichier_test_bonus.txt";

	
    printf("\nAdd three cells to a linked list : \n");
    // ajoute le m1 en tête de la liste (vide)
    new = LL_create_cell(&m1);
    REQUIRE ( new != NULL );
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // ajoute le m2 en tête de la liste
    new = LL_create_cell(&m2);
    REQUIRE ( new != NULL );
    LL_add_cell(&list, new);
    CHECK( list == new ); 

    // ajouter le m3 en tete de la liste
    new = LL_create_cell(&m3);
    REQUIRE ( new != NULL );
    LL_add_cell(&list, new);
    CHECK( list == new );

    // tester les valeurs de la liste
    // creation du flux de texte => buffer
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file);

    LL_save_list_toFileName(list, nom_fichier,
			    &monom_save2file);
    fclose(file);


    // recuperation information du fichier pour test si la commande
    // LL_save_list_toFileName fonctionne
    fptr = fopen(nom_fichier, "r");
    REQUIRE (fptr != NULL);

    // recupération taille pour alloc buffer_alloc
    fseek(fptr, 0, SEEK_END);
    file_size = ftell(fptr);
    rewind(fptr);

    // creation buffer pour comparer
    buffer_alloc = (char*) malloc(sizeof(char) * (file_size + 1));

    REQUIRE(buffer_alloc != NULL);

    fread(buffer_alloc, sizeof(char), file_size, fptr);
    buffer_alloc[file_size] = '\0';

	
    CHECK (strcmp(buffer_alloc, "12.400 3\n25.800 9\n3.245 17\n") == 0);

    free(buffer_alloc);
    fclose(fptr);

    // liberer la liste
    LL_free_list(&list);

    // suprime fichier
    remove(nom_fichier);
}



TEST(LL_multiplication_monome_to_list) {
// test pour la multication d'une liste par un monome
    cell_t *list, *list_res;
    char buffer[1024];
    char buffer_res[1024];
    
    FILE * file= NULL;
    monom_t monome = {6.000 , 4};

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // multiplication
    list_res = LL_multication_monome_to_list(&monome, &list);

    // enregistrement list_resdans buffer_res
    file = fmemopen(buffer_res, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list_res, monom_save2file);
    fclose(file);
    
    // verification de la multiplication
    CHECK (0 == strcmp(buffer_res,  "30.000 5\n24.000 6\n30.000 7\n36.000 8\n"));
    
    // liberation
    LL_free_list(&list);
    LL_free_list(&list_res);
}



TEST(LL_save_list_toFileName1){ // quand liste est vide, ne modifie pasle fichier
    cell_t *list  = NULL;

    FILE *fptr;
    char buffer[1024];
    char * buffer_alloc;
    long file_size;
    char nom_fichier[] = "fichier_test_bonus.txt";


    // tester les valeurs de la liste
    // creation du flux de texte => buffer
    FILE * file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file);

    LL_save_list_toFileName(list, nom_fichier,
			    &monom_save2file);
    fclose(file);

    // recuperation information du fichier pour test si la commande
    // LL_save_list_toFileName fonctionne
    fptr = fopen(nom_fichier, "r");
    REQUIRE (fptr != NULL);

    // recupération taille pour alloc buffer_alloc
    fseek(fptr, 0, SEEK_END);
    file_size = ftell(fptr);
    rewind(fptr);

    // creation buffer pour comparer
    buffer_alloc = (char*) malloc(sizeof(char) * (file_size + 1));

    REQUIRE(buffer_alloc != NULL);

    fread(buffer_alloc, sizeof(char), file_size, fptr);
    buffer_alloc[file_size] = '\0';

	
    CHECK (strcmp(buffer_alloc, "\0") == 0);

    free(buffer_alloc);
    fclose(fptr);

    // liberer la liste
    LL_free_list(&list);

    // suprime fichier
    remove(nom_fichier);
}


TEST(LL_multiplication_monome_to_list) {
// test pour la multication d'une liste par un monome NULL
    cell_t *list, *list_res;
    char buffer[1024];
    
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // multiplication
    list_res = LL_multication_monome_to_list(NULL, &list); // monome == NULL

    // verification
    CHECK(NULL == list_res);
    
    // liberation
    LL_free_list(&list);
    LL_free_list(&list_res);
}

TEST(LL_multiplication_monome_to_list1) {
// test pour la multication d'une liste par un non monome NULL
    cell_t *list, *list_res;
    char buffer[1024];
    char buffer_res[1024];
    
    monom_t monome = {2, 2};
    
    FILE * file= NULL;

    // verifie bonne recuperation
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&list,"data/listeChaineeTest.txt",
				monom_degree_cmp);
    LL_save_list_toFile(file, list, monom_save2file);
    fclose(file);

    // check bonne recuperation
    CHECK (0 == strcmp(buffer, "5.000 1\n4.000 2\n5.000 3\n6.000 4\n"));

    // multiplication
    list_res = LL_multication_monome_to_list(&monome, &list);

    // verifie bonne recuperation
    file = fmemopen(buffer_res, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, list_res, monom_save2file);
    fclose(file);
    
    CHECK(0 == strcmp(buffer_res, "10.000 3\n8.000 4\n10.000 5\n12.000 6\n"));
    
    // liberation
    LL_free_list(&list);
    LL_free_list(&list_res);
}

TEST(LL_multiplication_monome_to_list2) {
// test pour la multication d'une liste NULL par un non monome NULL
    cell_t *list = NULL, *list_res = NULL;
    
    monom_t monome = {2, 2};

    // multiplication
    list_res = LL_multication_monome_to_list(&monome, &list);
    CHECK(NULL == list_res);
    
    // liberation
    LL_free_list(&list);
    LL_free_list(&list_res);
}

TEST(LL_multiplication_monome_to_list3) {
// test pour la multication d'une liste NULL par un monome NULL
    cell_t *list = NULL, *list_res = NULL;

    // multiplication
    list_res = LL_multication_monome_to_list(NULL, &list);
    CHECK(NULL == list_res);
    
    // liberation
    LL_free_list(&list);
    LL_free_list(&list_res);
}


END_TEST_GROUP(linkedList)

    int main(void) {
    RUN_TEST_GROUP(linkedList);
    return EXIT_SUCCESS;
}
