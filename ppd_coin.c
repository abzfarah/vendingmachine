/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Abdi Farah
 * Student Number   : 543707
 * Course Code      : MEC20004
 * Program Code     : MSc
 ***********************************************************************/
#include "ppd_main.h"
#include "ppd_coin.h"
#include <strings.h>

static int denom_vals[NUM_DENOMS] = {5, 10, 20, 50, 100, 200, 500, 1000};

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/

void ppd_cash_register_init(struct ppd_system * system){
	system->cash_register[FIVE_CENTS].denom = FIVE_CENTS;
	system->cash_register[FIVE_CENTS].count = 0;

	system->cash_register[TEN_CENTS].denom = TEN_CENTS;
	system->cash_register[TEN_CENTS].count = 0;

	system->cash_register[TWENTY_CENTS].denom = TWENTY_CENTS;
	system->cash_register[TWENTY_CENTS].count = 0;

	system->cash_register[FIFTY_CENTS].denom = FIFTY_CENTS;
	system->cash_register[FIFTY_CENTS].count = 0;

	system->cash_register[ONE_DOLLAR].denom = ONE_DOLLAR;
	system->cash_register[ONE_DOLLAR].count = 0;

	system->cash_register[TWO_DOLLARS].denom = TWO_DOLLARS;
	system->cash_register[TWO_DOLLARS].count = 0;

	system->cash_register[FIVE_DOLLARS].denom = FIVE_DOLLARS;
	system->cash_register[FIVE_DOLLARS].count = 0;

	system->cash_register[TEN_DOLLARS].denom = TEN_DOLLARS;
	system->cash_register[TEN_DOLLARS].count = 0;
}

int ppd_load_coins(struct ppd_system * system){
	char line[100]={0};
	FILE * fin = fopen(system->coin_file_name, "r");
	if(fin == NULL){
		perror("fopen");
		return 1;
	}

	while(fgets(line, sizeof(line), fin) != NULL){
		char *denom, *quantity;
		int quantity_val, denom_val, i;

		denom = strtok(line, COIN_DELIM);
		if(denom == NULL){
			fprintf(stderr, "Error: Invalid coin line\n");
			return 1;
		}

		quantity = strtok(NULL, COIN_DELIM);
		if(quantity == NULL){
			fprintf(stderr, "Error: Invalid coin line\n");
			return 1;
		}
		quantity_val = atoi(quantity);
		if(quantity_val < 0){
			fprintf(stderr, "Error: Invalid coin quantity\n");
			return 1;
		}

		denom_val = atoi(denom);
		for(i=0; i < quantity_val; i++)
			ppd_add_coins(system, denom_val);
		bzero(line, sizeof(line));
	}
	fclose(fin);
	return 0;
}

int ppd_return_change(struct ppd_system * system, struct price *p){
	int i, last_change;
	int cents = (p->dollars * 100) + p->cents;
	if(cents == 0)
		return 0;

	while(cents > 0){
		last_change = 0;
		for(i=NUM_DENOMS-1; i >0; i--){
			if(system->cash_register[system->cash_register[i].denom].count > 0){
				int val = denom_vals[system->cash_register[i].denom];
				if(val <= cents){
					if(val >= 100){
						printf("$%i ", val/100);
					}else{
						printf("%ic ", val);
					}
					cents -= val;
					system->cash_register[system->cash_register[i].denom].count--;
					last_change = val;
				}
			}
		}
		if(last_change == 0){
			printf("Sorry, but we can't return change\n");
			return 1;
		}
	}
	return 0;
}

int ppd_add_coins(struct ppd_system * system, const int val){
	int i=0;
	switch(val){
			case 1000:	i = TEN_DOLLARS; 	break;
			case 500:	i = FIVE_DOLLARS; 	break;
			case 200:	i = TWO_DOLLARS; 	break;
			case 100:	i = ONE_DOLLAR;	 	break;
			case 50:	i = FIFTY_CENTS;	break;
			case 20:	i = TWENTY_CENTS;	break;
			case 10:	i = TEN_CENTS;		break;
			case 5:		i = FIVE_CENTS;		break;
			default:
				fprintf(stderr, "Error: $%i.%02i is not a valid denomination of money.\n",
								val/100, val%100);
				return 1;
		}
	system->cash_register[i].count++;
	return 0;
}

int ppd_save_coins(struct ppd_system * system){
	FILE * fout = fopen(system->coin_file_name, "w");
	if(fout == NULL){
		perror("fopen");
		return 1;
	}

	fprintf(fout, "%i,%i\n", 1000, system->cash_register[TEN_DOLLARS].count);
	fprintf(fout, "%i,%i\n", 500,  system->cash_register[FIVE_DOLLARS].count);
	fprintf(fout, "%i,%i\n", 200,  system->cash_register[TWO_DOLLARS].count);
	fprintf(fout, "%i,%i\n", 100,  system->cash_register[ONE_DOLLAR].count);
	fprintf(fout, "%i,%i\n", 50,   system->cash_register[FIFTY_CENTS].count);
	fprintf(fout, "%i,%i\n", 20,   system->cash_register[TWENTY_CENTS].count);
	fprintf(fout, "%i,%i\n", 10,   system->cash_register[TEN_CENTS].count);
	fprintf(fout, "%i,%i\n", 5,    system->cash_register[FIVE_CENTS].count);

	fclose(fout);
	return 0;
}

int ppd_is_denom(int value){
	int i;
	for(i=0; i < NUM_DENOMS; i++){
		if(value == denom_vals[i])
			return 0;
	}
	return 1;
}

int ppd_list_coins(struct ppd_system * system){
	int i,li;
	char * label[2] = {"dollars", "cents"};
	for(i=0; i < NUM_DENOMS; i++){
		int denom = denom_vals[system->cash_register[i].denom];
		int count = system->cash_register[system->cash_register[i].denom].count;
		if(denom >= 100){
			denom /= 100;
			li = 0;
		}else{
			li = 1;
		}
		printf("%-2i %-8s|%6i\n", denom, label[li], count);
	}
	return 0;
}
