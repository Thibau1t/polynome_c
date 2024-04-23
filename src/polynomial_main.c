/**
 * @file polynomial_main.c
 * @brief Programme pour les tests des operations sur les polynomes
 * ATTENTION : Il faut creer autant de tests que les cas d'execution pour chaque fonction a tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "polynomial.h"
#include "teZZt.h"

BEGIN_TEST_GROUP(polynomial)

TEST(LL_init_list) { // initialise polynome a NULL
    cell_t *list;

    printf("\nInitialization of the linked list : \n");
    LL_init_list(&list);

    REQUIRE ( list == NULL );
}

TEST(Poly_derive1) {  // avec un polynome quelconque 
    cell_t *poly = NULL;
    FILE   *file = NULL;
    char   buffer[1024];

    printf("\nDerive of polynomial 1 : \n");

    // création polynome
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly, monom_save2fileForPoly);
    fclose(file);
	
    //affichage polynome avec derivation : debug
    /* LL_save_list_toFile(stdout, poly, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test bonne recupération
    CHECK( 0 == strcmp(buffer, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // derivation
    poly_derive(&poly);

    // enregistrement dans un fichier
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly, monom_save2fileForPoly);
    fclose(file);

    // affichage du polynome dans le terminale : debug 
    /* LL_save_list_toFile(stdout, poly, monom_save2fileForPoly); */
    /* printf("\n");  */

    // verifiction du resultat
    CHECK( 0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) ") );

    // liberre mémoire
    LL_free_list(&poly);
}

TEST(Poly_derive2) {  // avec un maillon de deg 0
    cell_t *poly = NULL;
    FILE   *file = NULL;
    char   buffer[1024];

    printf("\nDerive twice a polynomial : \n");

    // création polynome
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly, "data/poly10.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly, monom_save2fileForPoly);
    fclose(file);
	
    //affichage polynome avec derivation : debug
    /* LL_save_list_toFile(stdout, poly, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test bonne recupération
    CHECK( 0 == strcmp(buffer, "(10.00, 0) (5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // derivation deux fois de suite
    poly_derive(&poly);
    poly_derive(&poly);
    CHECK( NULL != poly );

    // enregistrement dans un fichier
    file = fmemopen(buffer, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly, monom_save2fileForPoly);
    fclose(file);

    // affichage du polynome dans le terminale : debug 
    /* LL_save_list_toFile(stdout, poly, monom_save2fileForPoly); */
    /* printf("\n"); */

    // verifiction du resultat
    CHECK( 0 == strcmp(buffer, "(8.00, 0) (30.00, 1) (72.00, 2) (60.00, 3) ") );

    // liberre mémoire
    LL_free_list(&poly);
}

TEST(Poly_derive3) { // test sur la derivation d'un polynome NULL
    cell_t *poly = NULL;

    printf("\nDerive of NULL polynomial : \n");

    // derivation deux fois de suite
    CHECK( NULL == poly );
    poly_derive(&poly);
    CHECK( NULL == poly );

    // liberre mémoire
    LL_free_list(&poly);
    CHECK ( NULL == poly );
}

TEST(Poly_addition0) {  // cas ou les 2 polynomes ont tous les mêmes degrés
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly2.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (6.00, 3) (7.00, 4) (4.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (9.00, 2) (11.00, 3) (13.00, 4) (7.00, 5) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition1) {
// cas ou l'addition provoque la suppression d'un maillon au milieu
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly3.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (-5.00, 3) (7.00, 4) (4.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (9.00, 2) (13.00, 4) (7.00, 5) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition2) {
// cas ou l'addition provoque la suppression du premier maillon
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly4.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(-5.00, 1) (5.00, 2) (6.00, 3) (7.00, 4) (4.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(9.00, 2) (11.00, 3) (13.00, 4) (7.00, 5) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition3) {
// cas ou l'addition provoque la suppression du dernier maillon
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly5.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    /* vérifie bonne récupération et sauvegarde du polynome : debug */
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (6.00, 3) (7.00, 4) (-3.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (9.00, 2) (11.00, 3) (13.00, 4) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition4) {
    // cas ou P1 est plus grand que P2
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly8.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) (8.00, 10) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly7.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (6.00, 3) (7.00, 4) (-3.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (4.00, 2) (11.00, 3) (13.00, 4) (8.00, 10) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition5) {
    // cas ou P2 est plus grand que P1
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    /* vérifie bonne récupération et sauvegarde du polynome : debug */
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly9.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    /* vérifie bonne récupération et sauvegarde du polynome : debug */
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (6.00, 3) (7.00, 4) (-3.00, 5) (8.00, 7) (8.78, 9) (14.00, 15) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (4.00, 2) (11.00, 3) (13.00, 4) (8.00, 7) (8.78, 9) (14.00, 15) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition6) {
// cas ou des maillons du poly 2 doivent être inserer entre 2 maillons du poly1
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly6.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly3.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (-5.00, 3) (7.00, 4) (4.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(11.00, 1) (9.00, 2) (7.00, 4) (7.00, 5) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition7) {  // cas l'addition provoque polynome NULL, P1 + (-1*P1)
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly6.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly11.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(-5.00, 1) (-4.00, 2) (-5.00, 3) (-3.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK(poly1 == NULL);

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition8) {  // cas ou les  P1 est NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly2.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (6.00, 3) (7.00, 4) (4.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(6.00, 1) (5.00, 2) (6.00, 3) (7.00, 4) (4.00, 5) ") );

    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition9) {  // cas ou P2 est NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer3[1024];
	
    printf("\nAddition de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);
    CHECK ( NULL == poly2 );

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );
    
    // libere le polynome 1
    // polynome 2 deja libere
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_addition10) {  // cas ou P1 et P2 sont NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
	
    printf("\nAddition de deux polynomes : \n");
    // addition des deux poly : res dans poly1
    poly_add(&poly1, &poly2);

    CHECK ( NULL == poly1 );
    CHECK ( NULL == poly2 );
}

TEST(Poly_produit0) { // test sur le calcul du produit de deux polymones
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    cell_t * poly3 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nProduit de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // creation polynome2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly3.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly2, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(6.00, 1) (5.00, 2) (-5.00, 3) (7.00, 4) (4.00, 5) ") );

    // prod des deux poly : res dans poly3
    poly3 = poly_prod(&poly1, &poly2);

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly3, monom_save2fileForPoly);
    fclose(file);

    // affichage pour debug
    LL_save_list_toFile(stdout, poly3, monom_save2fileForPoly);
    printf("\n");

    // test resultat
    CHECK( 0 == strcmp(buffer3, "(30.00, 2) (49.00, 3) (25.00, 4) (76.00, 5) (71.00, 6) (36.00, 7) (47.00, 8) (45.00, 9) (12.00, 10) ") );

    // libere le polynome 1,2 et 3
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );

    LL_free_list(&poly2);
    CHECK ( NULL == poly2 );

    LL_free_list(&poly3);
    CHECK ( NULL == poly3 );   
}

TEST(Poly_produit1) {
    // test sur le calcul du produit de deux polymones dont Poly1 est NULL
    // donne poly_res == NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    cell_t * poly3 = NULL;
    FILE   * file  = NULL;
    char     buffer1[1024];
    char     buffer3[1024];
	
    printf("\nProduit de deux polynomes : \n");

    // creation polnome 1
    file = fmemopen(buffer1, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly1, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly1, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer1, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // prod des deux poly : res dans poly3
    poly3 = poly_prod(&poly1, &poly2);

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly3, monom_save2fileForPoly);
    fclose(file);

    // test resultat
    CHECK(NULL ==  poly3);

    // libere le polynome 1 (pas besoin pour poly 2 et 3)
    LL_free_list(&poly1);
    CHECK ( NULL == poly1 );
}

TEST(Poly_produit2) {
    // test sur le calcul du produit de deux polymones dont Poly2 est NULL
    // donne poly_res == NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    cell_t * poly3 = NULL;
    FILE   * file  = NULL;
    char     buffer2[1024];
    char     buffer3[1024];
	
    printf("\nProduit de deux polynomes : \n");

    // creation polnome 2
    file = fmemopen(buffer2, 1024, "w");
    REQUIRE ( NULL != file );
    LL_create_list_fromFileName(&poly2, "data/poly1.txt", monom_degree_cmp);
    LL_save_list_toFile(file, poly2, monom_save2fileForPoly);
    fclose(file);

    // vérifie bonne récupération et sauvegarde du polynome : debug
    /* LL_save_list_toFile(stdout, poly1, monom_save2fileForPoly); */
    /* printf("\n"); */
    CHECK( 0 == strcmp(buffer2, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

    // prod des deux poly : res dans poly3
    poly3 = poly_prod(&poly1, &poly2);

    // verification addition
    file = fmemopen(buffer3, 1024, "w");
    REQUIRE ( NULL != file );
    LL_save_list_toFile(file, poly3, monom_save2fileForPoly);
    fclose(file);

    // test resultat
    CHECK(NULL ==  poly3);

    // libere le polynome 2 (pas besoin pour poly 1 et 3)
    LL_free_list(&poly2);
    CHECK ( NULL == poly2 );
}

TEST(Poly_produit3) {
    // test sur le calcul du produit de deux polymones NULL
    // donne poly_res == NULL
    cell_t * poly1 = NULL;
    cell_t * poly2 = NULL;
    cell_t * poly3 = NULL;
	
    printf("\nProduit de deux polynomes NULL : \n");

    // prod des deux poly : res dans poly3
    poly3 = poly_prod(&poly1, &poly2);

    // test resultat
    CHECK(NULL ==  poly3);
}

TEST(LL_save_list_toFileName) { // test pour l'ecriture d'un polynome dans un fichier
    cell_t *list = NULL;
    cell_t *new = NULL;
    monom_t m1 = {3.245, 17}; 
    monom_t m2 = {25.8, 9};
    monom_t m3 = {12.4, 3};

    FILE *fptr;
    char buffer[1024];
    char * buffer_alloc;
    long file_size;
    char nom_fichier[] = "fichier_test_bonus.txt";

	
    printf("\nAdd three monomes to a polynome : \n");
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

    // tester les valeurs du polynome
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

END_TEST_GROUP(polynomial)

int main(void) {
    RUN_TEST_GROUP(polynomial);
    printf("\n");
    return EXIT_SUCCESS;
}
 
