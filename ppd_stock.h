/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Abdi Farah
 * Student Number   : 543707
 * Course Code      : MEC20004
 * Program Code     : MSc
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#ifndef PPD_STOCK
#define PPD_STOCK

struct ppd_system;
/**
 * @file ppd_stock.h this file defines the data structures required to
 * manage the stock list. You should add here the function prototypes for
 * managing the list here and implement them in ppd_stock.c
 **/

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20

/**
 * The number of denominations of currency available in the system
 **/
#define NUM_DENOMS 8

/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price
{
    /**
     * the dollar value for some price
     **/
    unsigned dollars,
             /**
              * the cents value for some price
              **/
             cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand?
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
    struct ppd_stock * data;
    struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
    /**
     * the beginning of the list
     **/
    struct ppd_node * head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

int ppd_load_stock(struct ppd_system * system);
void ppd_free_stock(struct ppd_system * system);
struct ppd_stock * ppd_find_stock_id(struct ppd_list*, const char * item_id);
int ppd_save_stock(struct ppd_system * system);
int ppd_stock_generate_id();

int ppd_stock_list_insert(struct ppd_system * system, struct ppd_stock*);
int ppd_stock_list_remove(struct ppd_system * system, struct ppd_stock*);
void ppd_stock_list_sort_id(struct ppd_system * system);
#endif


