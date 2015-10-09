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

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN
#define COIN_DELIM ","
struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system.
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR,
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

void ppd_cash_register_init(struct ppd_system * ppd);
int ppd_load_coins(struct ppd_system * ppd);
int ppd_return_change(struct ppd_system * ppd, struct price *p);
int ppd_add_coins(struct ppd_system * system, const int val);
int ppd_save_coins(struct ppd_system * ppd);
int ppd_is_denom(int value);
int ppd_list_coins(struct ppd_system * ppd);
#endif
