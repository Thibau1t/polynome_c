#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include "linkedList.h"

/**
 * @brief compute 'in place' the derive of a polynomial
 * @param [in, out] polynome address of a polynomial's head pointer
 */
void poly_derive(cell_t ** polynome);

/**
 * @brief compute P1 = P1 + P2, P2 become empty
 * @param poly1 [in, out] address of the 1st polynomial's head pointer
 * @param poly2 [in, out] address of the 2nd polynomial's head pointer
 */
void poly_add(cell_t ** poly1, cell_t ** poly2 );

/** TO DO
 * @brief P1 * P2
 * @param poly1 [in, out] head pointer of the 1st polynomial
 * @param poly2 [in, out] head pointer of the 2nd polynomial
 * @return P1*P2
 */
cell_t * poly_prod(cell_t ** poly1, cell_t ** poly2);

#endif
