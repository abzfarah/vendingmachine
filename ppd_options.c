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

#include "ppd_options.h"
#include "ppd_utility.h"
#include <stdio.h>
#include <strings.h>

/**
 * @file ppd_options.c this is where you need to implement the main
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere.
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
	struct ppd_node * iter;

	printf("Items Menu­­\n");
	printf("----------\n");
	printf("ID    |Name                |Available |Price\n");
	printf("--------------------------------------------\n");
	ppd_stock_list_sort_id(system);
	iter = system->item_list->head;
	while(iter){
		struct ppd_stock * d = iter->data;
		printf("%-6s|%-20s|%-10i|$ %i.%i\n", d->id, d->name, d->on_hand,
										 d->price.dollars, d->price.cents);
		iter = iter->next;

	}
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
	char buf[IDLEN+1]={0};
	struct price left, give;
	struct ppd_stock * item;
	int len, refund;

    printf("Purchase Item:\n");
    printf("-------------\n");
    printf("Please enter the id of the item you wish to purchase:");
    if((fgets(buf, sizeof(buf), stdin) == NULL) || (buf[0] =='\n')){
		return FALSE;
	}
	len = strlen(buf);
	if(buf[len-1] != '\n'){	/* If we have more data to read*/
		read_rest_of_line();
	}else{
		buf[len-1] = '\0';
	}

	item = ppd_find_stock_id(system->item_list, buf);
	if(item == NULL){
		fprintf(stderr, "Error: Item with id %s not found.\n", buf);
		return FALSE;
	}

	if(item->on_hand <= 0){
		printf("You have selected \"%s\", but we are out of stock.\n", item->name);
		return FALSE;
	}

	printf("You have selected \"%s\". This will cost you $%i.%02i\n",
			item->desc, item->price.dollars, item->price.cents);
	printf("Please hand over the money – type in the value of each note/coin in cents.\n");
	printf("Press enter or ctrl­d on a new line to cancel this purchase:\n");

	left = item->price;
	refund = 0;
	while((left.dollars > 0) || (left.cents > 0)){
		int coins;
		printf("You still need to give us $%i.%i:", left.dollars, left.cents);
		if((fgets(buf, sizeof(buf), stdin) == NULL) || (buf[0] =='\n')){	/*Ctrl-D or Enter*/
			refund = 1; break;
		}
		len = strlen(buf);
		if(buf[len-1] != '\n'){	/* If we have more data to read*/
			read_rest_of_line();
		}else{
			buf[len-1] = '\0';
		}

		coins = 0;
		if(sscanf(buf, "%i", &coins) != 1){
			perror("scanf");
			continue;
		}

		if(ppd_is_denom(coins) == 1){
			printf("Error: $%i.%02i is not a valid denomination of money.\n",
								coins/100, coins%100);
			continue;
		}
		ppd_add_coins(system, coins);

		give.dollars = coins / 100;
		give.cents    = coins % 100;

		if(give.dollars > 0){
			if(give.dollars >= left.dollars){
				give.dollars -= left.dollars;
				left.dollars = 0;
			}else{
				left.dollars -= give.dollars;
			}
		}

		if(give.cents > 0){
			if(give.cents >= left.cents){
				give.cents -= left.cents;
				left.cents = 0;
			}else{
				left.cents -= give.cents;
			}
		}
	}

	if(refund){
		struct price coins_back;
		coins_back.dollars = item->price.dollars - left.dollars;
		coins_back.cents   = item->price.cents   - left.cents;
		printf("You are being refunded with $%i,%i: ", coins_back.dollars, coins_back.cents);
		ppd_return_change(system, &coins_back);	/* refund */
		printf("\n");
		return FALSE;	/* return to main menu */
	}

	item->on_hand--;

	printf("Thank you. Here is your %s", item->name);
	if(give.dollars > 0 || give.cents > 0){
		printf(", and your change of $ %i.%i: ", give.dollars, give.cents);
		ppd_return_change(system, &give);
	}else{
		printf(".");
	}
	printf("\nPlease come again soon.\n");

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    if( (ppd_save_stock(system) == 1) ||
		(ppd_save_coins(system) == 1)	)
		return FALSE;
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
	struct ppd_stock * item;
    int len, rc, gen_id = ppd_stock_generate_id();

	item = (struct ppd_stock*) malloc(sizeof(struct ppd_stock));
	if(item == NULL){
		perror("malloc");
		return FALSE;
	}
	bzero(item, sizeof(struct ppd_stock));
	sprintf(item->id, "I%04i", gen_id);


	printf("This new meal item will have the Item id of I%i.\n", gen_id);
	printf("Enter the item name:");
	if((fgets(item->name, NAMELEN, stdin) == NULL) || (item->name[0] == '\n')){
		return FALSE;
	}
	len = strlen(item->name);
	if(item->name[len-1] != '\n'){	/* If we have more data to read*/
		read_rest_of_line();
	}else{
		item->name[len-1] = '\0';
	}

	printf("Enter the item description:");
	if((fgets(item->desc, DESCLEN, stdin) == NULL) || (item->desc[0] == '\n')){
		return FALSE;
	}
	len = strlen(item->desc);
	if(item->desc[len-1] != '\n'){	/* If we have more data to read*/
		read_rest_of_line();
	}else{
		item->desc[len-1] = '\0';
	}

	printf("Enter the price for this item:");
	rc = fscanf(stdin, "%u.%u", &item->price.dollars, &item->price.cents);
	if((rc == EOF) || (rc != 2)){
		return FALSE;
	}
	read_rest_of_line();	/* read the newline and everything else*/

	item->on_hand = DEFAULT_STOCK_LEVEL;

	if(ppd_stock_list_insert(system, item) == 0){
		printf("This item \"%s-%s\" has now been added to the menu.\n", item->name, item->desc);
		return TRUE;
	}
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
    struct ppd_stock * item;
	int len;
	char item_id[IDLEN+1]={0};

    printf("Please enter the id of the item to remove from the menu:");
    if((fgets(item_id, sizeof(item_id), stdin) == NULL) || (item_id[0] == '\n')){
		return FALSE;
	}
	len = strlen(item_id);
	if(item_id[len-1] != '\n'){	/* If we have more data to read*/
		read_rest_of_line();
	}else{
		item_id[len-1] = '\0';
	}

	item = ppd_find_stock_id(system->item_list, item_id);
	if(item == NULL){
		fprintf(stderr, "Error: Item with id %s not found.\n", item_id);
		return FALSE;
	}

	if(item->on_hand <= 0){
		printf("You have selected \"%s\", but there are no items of that kind on hand.\n", item->name);
		return FALSE;
	}

	printf("\"%s - %s\" has been removed from the system.\n", item->id, item->desc);
	ppd_stock_list_remove(system, item);

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
	struct ppd_node * iter = system->item_list->head;
	while(iter){
		struct ppd_stock * item = iter->data;
		item->on_hand = DEFAULT_STOCK_LEVEL;
		iter = iter->next;
	}
	printf("All stock has been reset to the default value of %i\n", DEFAULT_STOCK_LEVEL);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
	int i;
	for(i=0; i < NUM_DENOMS; i++)
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	printf("All coins have been reset to the default value of %i\n", DEFAULT_COIN_COUNT);
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
	printf("Coins Summary\n");
	printf("-------------\n");
	printf("Denomination	| Count\n");
	printf("-----------------------\n");
	ppd_list_coins(system);
    return FALSE;
}

BOOLEAN abort_program(struct ppd_system * system){
	printf("Bye.\n");
	return TRUE;
}
