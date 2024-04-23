/**
 * @file linkedList.h
 * @brief Header file of linked list module
 */

#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "valCell.h"

/**
 * @struct cell_t
 * @brief Data structure of the linked list's cell
 * 2 fields :
 *      - 'val' : value of a cell
 *      - 'next' : pointer to the next cell
 */
typedef struct cell_s {
    monom_t val;
    struct cell_s * next;
} cell_t;

/** 
 * @fn void LL_init_list(cell_t **adrHeadPt)
 * @brief Initialize a void list
 * @param [in, out] adrHeadPt address of head pointer of the list 
 */
void LL_init_list(cell_t ** adrHeadPt);

/** 
 * @brief create a new cell for linked list from its data 
 * @param [in] pdata address of the data
 * @return address of the new cell
 */
cell_t * LL_create_cell(monom_t * pmonome);

/** 
 * @brief Insert a cell into a linked list at the given positione
 * @param [in, out]  prec address of previous pointer of the cell
 * @param [in]  a address of the cell to be added to the linked list
 */
void LL_add_cell(cell_t ** prec, cell_t * a);

/** 
 * @brief Create a linked list from a file
 * @param [in, out] a0 address of head pointer of a linked list
 * @param [in] fname name of a file containing the data for a linked list
 * @param pfcmp fonction pointer for comparison of two cell's value
 * @return head pointer of the linked list
 */
cell_t ** LL_create_list_fromFileName(cell_t ** a0, char * fname,
				      int (*pfcmp) (monom_t *, monom_t *));

/**
 * @brief Write the linked list to an output stream
 * @param [in] file file pointer of an output stream
 * @param [in] a0 head pointeur of a linked list
 * @param pfprint fonction pointer for printing the data of a cell on an output stream
 */
void LL_save_list_toFile(FILE * file, cell_t * head, void (*pfprint) (FILE *, monom_t *));

/**
 * @brief Save a linked list into a file
 * @param [in, out] head pointer of a linked list
 * @param [in] fname name of the backup file
 * @param  fonction pointer for writing the data of a cell to a output stream
 */
void LL_save_list_toFileName(cell_t * head, char * fname, void (*pfwrite) (FILE *, monom_t *));

/**
 * @brief Search a value in a linked list, and return the address of the previous pointer
 * @param [in] a0 address of the head pointer
 * @param [in] value address of the value to search
 * @param  pfcmp fonction pointer for comparison of two values
 * @return the address of the previous pointer
 */
cell_t ** LL_search_prev(cell_t ** a0, monom_t * value, int (*pfcmp) (monom_t *, monom_t *));

/**
 * @brief Delete a cell from a linked list
 * @param [in, out] prec address of the previous pointer of the cell to delete
 */
void LL_del_cell(cell_t ** prec);


/**
 * @brief Free the memory location occupied by the cells of a linked list
 * @param [in, out] a0 address of head pointer of a linked list
 */
void LL_free_list(cell_t ** a0);


/**
 * @brief Multiply a list by a monome
 * @param [in, out] a0 address of head pointer of a linked list
 * @param [in, out] monome address of the monome used for multiplication
 * @return result of the multiplication between the list and the monome in a new list allocated in the function
 */
cell_t * LL_multication_monome_to_list(monom_t * monome, cell_t **  a0);

#endif
