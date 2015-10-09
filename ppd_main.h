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

#include <stdio.h>
#include <stdlib.h>
#include "ppd_stock.h"
#include "ppd_coin.h"
#ifndef PPD_MAIN
#define PPD_MAIN
/**
 * @file ppd_main.h this file holds the rest of the application together.
 * From these header files, main() can call all functions refered to 
 * in the header files included. You might also want to insert here
 * any functions for handling and validation of command line arguments
 **/

/**
 * datatype to represent a boolean value within the system
 **/
typedef enum truefalse
{
    /**
     * the constant for false
     **/
    FALSE, 
    /**
     * the constant for true
     **/
    TRUE
} BOOLEAN;

/**
 * this is the header structure for all the datatypes that is 
 * passed around and manipulated
 **/
struct ppd_system
{
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer - how does that
     * change what we need to do for initialization of the list?
     **/
    struct ppd_list * item_list;

    /**
     * the name of the coin file - we need this for saving as all menu
     * items only take the one parameter of the ppd_system
     **/
    const char * coin_file_name;
    /**
     * the name of the stock file
     **/
    const char * stock_file_name;
};

#endif
