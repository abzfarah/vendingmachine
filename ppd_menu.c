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

#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
static char * menu_labels[MENU_OPTIONS] = {
	"Display Items",
	"Purchase Items",
	"Save and Exit",
	"Add Item",
	"Remove Item",
	"Display Coins",
	"Reset Stock",
	"Reset Coins",
	"Abort Program"
};

static BOOLEAN (*menu_funcs[MENU_OPTIONS])(struct ppd_system*) = {
	display_items,
	purchase_item,
	save_system,
	add_item,
	remove_item,
	display_coins,
	reset_stock,
	reset_coins,
	abort_program
};

void init_menu( struct menu_item* menu)
{
	int i;
	for(i=0; i < MENU_OPTIONS; i++){
		strncpy(menu[i].name, menu_labels[i], MENU_NAME_LEN);
		menu[i].function = menu_funcs[i];
	}
}

int display_menu(struct menu_item* menu){
	int i,len;
	char line[MENU_NAME_LEN];

	printf("Main menu:\n");
	for(i=0; i < MENU_OPTIONS; i++){
		printf("%i. %s\n", i+1, menu[i].name);
		if(i == 2)
			printf("Administrator­Only Menu:\n");
	}
	printf("Select your option (1-%i):", MENU_OPTIONS);
	if((fgets(line, sizeof(line), stdin) == 0) || (line[0] == '\n')){
		return -1; /* Ctrl-D or Enter*/
	}
	len = strlen(line);
	if(line[len-1] != '\n'){	/* If we have more data to read*/
		read_rest_of_line();
	}else{
		line[len-1] = '\0';
	}
	return atoi(line) - 1;
}
