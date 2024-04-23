#include <stdio.h>
#include <stdlib.h>
#include <float.h>  // FLT_EPSILON DBL_EPSILON LDBL_EPSILON
#include <math.h>

#include "linkedList.h"

/**
 * @brief compute 'in place' the derive of a polynomial 
 * @param [in, out] xxx address of a polynomial's head pointer
 */
void poly_derive(cell_t ** polynome)
// hypothese : fonctione pour tous les polynomes trié par degré croissant
{
    cell_t ** prec = polynome; // parcours avec prec pour supprimer si deg == 0

    if ((*prec) != NULL && (*prec)->val.degree == 0) // deg maillonTete == 0
    // meilleure complexité que de testere dans le TQ a chaque fois
    // hyp: triee oredre croissant donc supprime tete si deg == 0
    {
	LL_del_cell(prec);  // supprime tete
    }
    while ((*prec) != NULL) // parcours polynome avec prec
    {
	(*prec)->val.coef  *= (*prec)->val.degree;  // change coef (coef* deg)
	(*prec)->val.degree = (*prec)->val.degree - 1; // decremente deg
	prec = &((*prec)->next); // passage maillon suivant
    }
}

/**
 * @brief compute P1 = P1 + P2, P2 become empty
 * @param poly1 [in, out] address of the 1st polynomial's head pointer
 * @param poly2 [in, out] address of the 2nd polynomial's head pointer
 */
void poly_add(cell_t ** poly1, cell_t ** poly2)
// hypothese : P1 et P2 triés par ordre croissant
{
    cell_t ** precP1 = poly1;    // precedent poly1, avance avec prec pour
				 // insertion et suppression
    cell_t ** precP2 = poly2;    // precedent poly2, avance avec prec pour
				 // insertion et suppression
                                 // currP1 = (*precP1)->val
    cell_t * tmp = NULL; // var temporaire
    
    while (NULL != (*precP1) && NULL != (*precP2))
    // on parcours jusqu'a ce qu'on arrive au dernier maillon de P1 ou P2
    {
	if ((*precP1)->val.degree == (*precP2)->val.degree) //même degre
	{
	    if (fabs ((*precP1)->val.coef + (*precP2)->val.coef)
		                      < DBL_EPSILON) // <==> == 0 pour double 
		//si ajout provoque coef null, on supprime le currP1
	    {
		LL_del_cell(precP1);
	    }
	    else // sinon on modifie la valeur du coef de p1
	    {
		(*precP1)->val.coef += (*precP2)->val.coef; // somme coef
		precP1 = &(*precP1)->next; // avance P1 d'un maillon
	    }
	    LL_del_cell(precP2); // libere la memoire le prec de P2 (+ avance P2)
	}
	else if (((*precP1)->val.degree > (*precP2)->val.degree))
            // si deg currP1 > deg currP2,
	    //   alors on insère le maillon currP2 entre precP1 et currP1
	{
	    tmp = (*precP2)->next; // sauvegarde suivant P2
	    LL_add_cell(precP1, *precP2); // ajout du maillon
	    precP2 = &tmp; 
	    
	}
	else // sinon deg currP1 < deg currP2
	{
	    precP1 = &(*precP1)->next; // alors on avance precP1 à son suivant
	}
    }
    if (NULL != (*precP2))
     // cas où la dimension de P1 est inférieure à celle de P2
     // il faut ajouter la fin du polynome de P2 à P1
    {
	*precP1 = *precP2; // chainage fin P2 a fin P1
	*precP2 = NULL; // P2 pointe sur NULL, plusde reference au maillon de P1 
    }
}


/** 
 * @brief compute P1 * P2
 * @param poly1 [in, out] head pointer of the 1st polynomial
 * @param poly2 [in, out] head pointer of the 2nd polynomial
 * @return P3 = P1*P2
 */
cell_t * poly_prod(cell_t ** poly1, cell_t ** poly2)
{  
    cell_t * poly_tmp = NULL; // polynome temporaire
    cell_t * poly_res = NULL; // polynome qui va contenir le resultat
    cell_t * currP2 = (*poly2); // maillon courant de P2
    
    while(currP2 != NULL) // pour chaque maillon de P2
    {
	// multication maillon de P2 avec P1, forme poly temporaire
	poly_tmp = LL_multication_monome_to_list(&currP2->val, poly1);

	// addition entre poly_tmp et pol_res, occurence 1 : poly_res <-poly_tmp + NULL 
	poly_add(&poly_res, &poly_tmp);

	// avance currP2
	currP2 = currP2->next;
    }
    // retourne P3, construit avec la somme des poly_tmp
    return poly_res; 
}
