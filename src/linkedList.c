/**
 * @file linkedList.c
 * @brief Implementation file of linked list module
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/**
 * @fn void LL_init_list(cell_t **adrHeadPt)
 * @brief Initialize a void list
 * @param [in, out] adrHeadPt address of head pointer of the list 
 */
void LL_init_list(cell_t **adrHeadPt)
{
    // initialise la liste a NULL
    *adrHeadPt = NULL;
}


/** 
 * @brief create a new cell for linked list from its data 
 * @param [in] pdata address of the data
 * @return address of the new cell
 */
cell_t * LL_create_cell(monom_t * pmonome)
{
    cell_t * new = NULL;
    // alloue dynamiquement le monome
    new = (cell_t *) malloc(sizeof(cell_t));
    // si l'alloc a fonctionnee 
    if (NULL != new)
    {
	new->val.coef    = pmonome->coef;   //init le coef
	new->val.degree  = pmonome->degree; //init le degre
	new->next        = NULL;            //init le suivant
    }

    return new; // retrun NULLsi erreur malloc
}


/**
 * @brief Insert a cell into a linked list at the given position
 * @param [in, out]  prec address of previous pointer of the cell
 * @param [in]  a address of the cell to be added to the linked list
 */
void LL_add_cell(cell_t ** prec, cell_t * a)
{
    a->next = *prec; // modifie le suivant du maillon qu'on insert
    *prec   = a;     // realise le chainge avec prec
}


/**
 * @brief Create a linked list from a file
 * @param [in, out] a0 address of head pointer of a linked list
 * @param [in] fname name of a file containing the data for a linked list
 * @param pfcmp fonction pointer for comparison of two cell's value
 * @return head pointer of the linked list
 */
cell_t ** LL_create_list_fromFileName(cell_t ** a0,
				      char * fname,
				      int (*pfcmp) (monom_t *, monom_t *))
{
    FILE   *  fp    = fopen(fname, "r"); // si fname = NULL alors fp := NULL 
    cell_t ** prec  = NULL;
    cell_t *  curr  = NULL;
    int  error      = 0 ;                // = 1 si erreur malloc
    monom_t   mread;

    LL_init_list(a0);

    if (NULL != fp) { // ouverture fichier OK
	//lire une ligne
	while ( 2 ==  fscanf(fp, "%lg %d\n", &mread.coef, &mread.degree )
		&& !error)
	{
	    //créer cellule
	    curr = LL_create_cell(&mread);
	   
	    if (NULL != curr) // si malloc reussi
	    {
		//insérer cellule
		prec = LL_search_prev(a0, &mread, pfcmp);
		LL_add_cell(prec, curr);
	    }
	    else //message erreur malloc + free liste
	    {
		error = 1;
		fprintf(stderr,
			"%s : Erreur problème lors de l'allocation : %s\n",
			__FUNCTION__,
			fname);
		// libere toute la liste en cas d'erreur
		LL_free_list(a0);
	    }	
	}
	fclose(fp);
    }
    else { // message erreur ouverture
	fprintf(stderr,
		"%s : 'Erreur lors de l'ouverture du fichier: %s'\n",
		__FUNCTION__,
		fname);
    }
    return a0; // retourne la liste
}
  
/**
 * @brief Write the linked list to an output stream
 * @param [in] file file pointer of an output stream
 * @param [in] head pointer of a linked list
 * @param pfprint fonction pointer for printing the data of a cell on an output stream
 */
void LL_save_list_toFile(FILE * file, cell_t * head,
			 void (*pfprint) (FILE *, monom_t *))
{
    cell_t * curr = head;
    if (NULL != file) // verifie que FILE a ete correctement ouvert
    {
	while (NULL != curr) // parcours la liste : tq on est pas a la fin
	{
	    (*pfprint)(file, &curr->val); // ecrit dans le fichier
 	    curr = curr->next;            // passage au maillon suivant
	}	
    }
}


/**
 * @brief Save a linked list into a file
 * @param [in, out] head : head pointer of a linked list
 * @param [in] fname name of the backup file
 * @param pfwrite fonction pointer for writing the data of a cell to a output stream
 */
void LL_save_list_toFileName(cell_t * head, char * fname, void (*pfwrite) (FILE *, monom_t *))
{
    cell_t * curr = head;
    FILE * fp;
   
    fp   = fopen(fname, "w+"); // ecrase le fichier s'il existe
    if (NULL != fp)   // si on a reussi a ouvrir le fichier
    {
	
	while (NULL != curr)  // parcours la liste : tq on est pas a la fin
	{
	    (*pfwrite)(fp, &(curr->val)); // ecrit dans le fichier
 	    curr = curr->next;            // passage au maillon suivant
	}
	fclose(fp);  // ferme le fichier
    }
    else // message d'erreur si l'ouverture ne fonctionne pas
    {
	fprintf(stderr, "%s : 'Erreur lors de l'ouverture du fichier %s'",
		__FUNCTION__,
		fname);
    }
}

/**
 * @brief Search a value in a linked list, and return the address of the previous pointer
 * @param [in] a0 address of the head pointer
 * @param [in] value address of the value to search
 * @param pfcmp fonction pointer for comparison of two values
 * @return the address of the previous pointer
 */
cell_t ** LL_search_prev(cell_t ** a0, monom_t * value, int (*pfcmp) (monom_t *, monom_t *))
{
    cell_t ** prec = a0; // maillon precedent
    // tq prec!= NULL et que le maillon courant est pluspetit quecelui quel'on cherche
    while (*prec != NULL && (*pfcmp)(value, &(*prec)->val) > 0)
    {
	prec = &((*prec)->next); // on avance d'un maillon
    }
    return prec; // retourne le precedent
}

/**
 * @brief Delete a cell from a linked list
 * @param [in, out] prec address of the previous pointer of the cell to delete
 */
void LL_del_cell(cell_t ** prec)
{
    cell_t * tmp = *prec;
    (*prec) = (*prec)->next;
    (*tmp).val.coef   = 0;   // mets a 0 dans la RAM (nettoyage de memoire)
    (*tmp).val.degree = 0;   // permet de rendre confidentiel les donnees
    (*tmp).next = NULL;      // manipulees après fin du programme
                             // limite le reverse engineering sur les poly :)
    
    free(tmp);               // liberation du maillon
}


/**
 * @brief Free the memory location occupied by a linked list
 * @param [in, out] a0 address of head pointer of a linked list
 */
void LL_free_list(cell_t ** a0)
{
    cell_t * curr = *a0; // maillon courant
 
    while (curr != NULL) // parcours de la liste
    {
	LL_del_cell(&curr); // supprime le maillon en cours
    }
    (*a0) = NULL; // mets a0 a NULL car la liste est vide
}

/**
 * @brief Multiply a list by a monome
 * @param [in, out] a0 address of head pointer of a linked list
 * @param [in, out] monome address of the monome used for multiplication
 * @return result of the multiplication between the list and the monome in a new list allocated in the function
 */
cell_t * LL_multication_monome_to_list(monom_t * monome, cell_t **  a0)
{
    cell_t * list_res = NULL; // liste contenant le resultat
    cell_t ** prec_a0 = a0;   // precedent du maillon de la liste en parametre
    cell_t ** prec_list_res = &list_res; // precedent maillon en cours de la
					 // liste resultat
    monom_t monom_tmp; // variable temporaire pour plus de lisibilitee
    cell_t * cell_tmp; // variable temporaire pour plus de lisibilitee

    if (NULL != monome) // si le monome n'est pas NULL
	                // car sinon a0 * NULL= NULL
    {
	while((*prec_a0) != NULL) // parcours a0
	{
	    // multiplication entre le monome un maillon de a0
	    monom_tmp = monom_multiplication(&(*prec_a0)->val, monome);

	    // alloue la memoire pour creer le poly resultat
	    cell_tmp = LL_create_cell(&monom_tmp);

	    // ajoute le produit dans le poly resultat, respecte l'ordre
	    // croissant, pas besoin de s'en occupe  
	    LL_add_cell(prec_list_res, cell_tmp);

	    // avance le prec de a0 et prec de poly res
	    prec_a0       = &(*prec_a0)->next;
	    prec_list_res = &(*prec_list_res)->next;
	}
    }
    return list_res; // retourne la liste resultat, nouvelle liste
}
