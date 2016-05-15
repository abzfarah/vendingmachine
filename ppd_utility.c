/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Abdi Farah
 * Student Number   : 543707
 * Course Code      : MEC20004
 * Program Code     : MSc
 ***********************************************************************/

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function,
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name,
const char * stock_name)
{
    system->stock_file_name = stock_name;
    system->coin_file_name  = coins_name;

	if(	(ppd_load_stock(system) == 1) ||
		(ppd_load_coins(system) == 1)		){
		return FALSE;
	}

    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    ppd_cash_register_init(system);
    system->item_list = (struct ppd_list *) malloc(sizeof(struct ppd_list));
    if(system->item_list == NULL){
		perror("malloc");
		return FALSE;
	}

    system->item_list->head = NULL;
    system->item_list->count = 0;
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	ppd_free_stock(system);
    free(system->item_list);
}
