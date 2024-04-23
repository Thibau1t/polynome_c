/**
 * @file valCell.c
 * Implementation file for comparison and display of linked list's cells
 */

#include <stdio.h>
#include "valCell.h"

/**
 * @brief Compare the degree of two monomials
 * @param [in] m1 address of the first monomial
 * @param [in] m2 address of the second monomial
 * @return <0 if m1.degree<m2.degree; =0 if m1.degree=m2.degree;  >0 if  m1.degree>m2.degree
 * 0 if pointeur null
 */
int monom_degree_cmp(monom_t * m1, monom_t * m2)
{
    int res = 0; // 0 si au minimum un des monomes est NULL
    if(m1 != NULL && m2 != NULL) // si les deuxmonomes ne sont pas NULL
    { 
	res = (m1->degree - m2->degree); // difference entre les deux degres
    }
    return res;
}


/**
 * @brief write the information of a monomial to the given output stream
 * @param [in] file file pointer of an output stream
 * @param [in] m address of a monomial
 */
void monom_save2file(FILE * file, monom_t * monome)
{
    if (file != NULL && monome != NULL) // si le fichier est ouvert et le
					// pointeur n'est pas a NULL
    {
	fprintf(file, "%.3f %d\n", monome->coef, monome->degree);  // ecrit dans file 
    }
}


/**
 * @brief write the information of a monomial to the given output stream for
 * polynome test
 * @param [in] file file pointer of an output stream
 * @param [in] m address of a monomial
 */
void monom_save2fileForPoly(FILE * file, monom_t * monome)
{
    if (file != NULL && monome != NULL) // si le fichier est ouvert et le
					// pointeur n'est pas a NULL
    {
	fprintf(file, "(%.2f, %d) ", monome->coef, monome->degree); // ecrit dans file 
    }
}

/**
 * @brief Multiply two monomes together. The first receives the result
 * @param [in, out] monome1 address of the monome which receives the
 * multiplication between itself and the monome2
 * @param [in, out] monome2 address of the second monome participating in the
 * multiplication
 * @return result of the multiplication between the 2 monomes passed in parameter
 */
monom_t monom_multiplication(monom_t * monome1, monom_t * monome2)
{
    monom_t res; // monome resultat
    res.coef = monome1->coef * monome2->coef; // multiplication des coef
    res.degree = monome1->degree + monome2->degree; // somme des degres
    return res;
}
