/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : EDIT HERE
 * Student Number   : EDIT HERE
 * Course Code      : EDIT HERE
 * Program Code     : EDIT HERE
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/

int main(int argc, char **argv)
{
	/* represents the data structures to manage the system */
    struct ppd_system system;
    struct menu_item ppd_menu[MENU_OPTIONS];
    int stop;

    /* validate command line arguments */
    if(argc != 3){
		fprintf(stderr, "Usage: ./ppd <itemsfile> <coinsfile\n");
		return 1;
	}

    /* init the system */
    if(!system_init(&system))
		return 1;


    /* load data */
    if(!load_data(&system, argv[2], argv[1]))
		return 1;

    /* test if everything has been initialised correctly */

    /* initialise the menu system */
    init_menu(ppd_menu);

    /* loop, asking for options from the menu */
    stop = 0;
	do{
		int user_selected = display_menu(ppd_menu);

		/* run each option selected */
		if((user_selected >= 0) && (user_selected < MENU_OPTIONS))
			ppd_menu[user_selected].function(&system);

		switch(user_selected){
			case MENU_OPTIONS-1: stop = 1; break;	/*quit*/
			case 3-1:			 stop = 1; break;	/*save and quit*/
			default:					   break;
		}

		/* until the user quits */
	}while(stop == 0);

    /* make sure you always free all memory and close all files
     * before you exit the program
     */
    system_free(&system);
    return EXIT_SUCCESS;
}
