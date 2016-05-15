/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Abdi Farah
 * Student Number   : 543707
 * Course Code      : MEC20004
 * Program Code     : MSc
 ***********************************************************************/
#include "ppd_main.h"
#include "ppd_stock.h"
#include <strings.h>

 /**
  * @file ppd_stock.c this is the file where you will implement the
  * interface functions for managing the stock list.
  **/
static int max_id = 0;

void ppd_stock_list_sort_id(struct ppd_system * system){

	int i,list_size = system->item_list->count;

	/* bubble sort for single linked list */
	for(i=0; i < list_size; i++){
		struct ppd_node* current = system->item_list->head;
        struct ppd_node* next = current->next;
        struct ppd_node* previous = NULL;

        while(next != NULL) {
            if (strncmp(current->data->id, next->data->id, IDLEN) > 0) {
                if (current == system->item_list->head){
                    system->item_list->head = next;
                } else {
                    previous->next = next;
                }
                current->next = next->next;
                next->next = current;

                previous = next;
                next = current->next;
            }else {
                previous = current;
                current = current->next;
                next = current->next;
            }
        }
	}
}

int ppd_load_stock(struct ppd_system * system){
	char line[200]={0};
	FILE * fin = fopen(system->stock_file_name, "r");
	if(fin == NULL){
		perror("fopen");
		return 1;
	}

	while(fgets(line, sizeof(line), fin) != NULL){
		char *item_id, *item_name, *item_desc, *item_price, *item_count,
			 *price_dollars, *price_cents;
		struct ppd_stock * stock;
		int id, item_count_val;

		item_id = strtok(line, "|");
		if(item_id == NULL){	fprintf(stderr, "Error: Invalid stock line\n");	return 1;	}

		item_name = strtok(NULL, "|");
		if(item_name == NULL){	fprintf(stderr, "Error: Invalid stock line\n");	return 1;	}

		item_desc = strtok(NULL, "|");
		if(item_desc == NULL){	fprintf(stderr, "Error: Invalid stock line\n");	return 1;	}

		item_price = strtok(NULL, "|");
		if(item_price == NULL){	fprintf(stderr, "Error: Invalid stock line\n");	return 1;	}

		item_count = strtok(NULL, "|");
		if(item_count == NULL){	fprintf(stderr, "Error: Invalid stock line\n");	return 1;	}

		item_count_val = atoi(item_count);
		if(item_count_val < 0){
			fprintf(stderr, "Error: Invalid item count\n");
			return 1;
		}

		stock = (struct ppd_stock*) malloc(sizeof(struct ppd_stock));
		if(stock == NULL){
			perror("malloc");
			return 1;
		}

		price_dollars = strtok(item_price, ".");
		if(price_dollars == NULL){
			fprintf(stderr, "Error: Invalid stock price\n");
			return 1;
		}
		price_cents = strtok(NULL, ".");
		if(price_cents == NULL){
			fprintf(stderr, "Error: Invalid stock price\n");
			return 1;
		}
		/* Save the highest id from stock*/
		id = atoi(&item_id[1]);
		if(id > max_id)
			max_id = id;

		/* fill in the stock from item */
		strncpy(stock->id, item_id, IDLEN);
		strncpy(stock->name, item_name, NAMELEN);
		strncpy(stock->desc, item_desc, DESCLEN);
		stock->id[IDLEN] = stock->name[NAMELEN] = stock->desc[DESCLEN] = '\0';
		stock->price.dollars = atoi(price_dollars);
		stock->price.cents   = atoi(price_cents);
		stock->on_hand = item_count_val;

		/* insert node in list */
		ppd_stock_list_insert(system, stock);
		bzero(line, sizeof(line));
	}
	fclose(fin);

	return 0;
}

int ppd_stock_list_insert(struct ppd_system * system, struct ppd_stock * item){

	/* create data node */
	struct ppd_node* node = (struct ppd_node*) malloc(sizeof(struct ppd_node));
	if(node == NULL)
		return 1;
	node->data = item;
	node->next = NULL;

	if(system->item_list->head == NULL){
		system->item_list->head = node;
	}else{
		struct ppd_node * iter = system->item_list->head;
		struct ppd_node * prev = NULL;
		while(iter){
			if(strncmp(item->name, iter->data->name, NAMELEN) < 0)
				break;

			prev = iter;
			iter = iter->next;
		}
		if(prev){
			prev->next = node;
			if(iter)
				node->next = iter;
		}else{	/* one node list */
			node->next = system->item_list->head;
			system->item_list->head = node;
		}
	}
	system->item_list->count++;
	return 0;
}

int ppd_stock_list_remove(struct ppd_system * system, struct ppd_stock* item){
	struct ppd_node * iter = system->item_list->head;
	struct ppd_node * prev = NULL;
	while(iter){
		if(item == iter->data){	/*if its our data*/
			if(prev){
				prev->next = iter->next;
			}else{
				system->item_list->head = iter->next;
			}
			free(iter->data);
			free(iter);
			system->item_list->count--;
			return 0;
		}
		prev = iter;
		iter = iter->next;
	}
	return 1;
}

void ppd_free_stock(struct ppd_system * system){
	struct ppd_node * iter = system->item_list->head;
	while(iter){
		struct ppd_stock * data = iter->data;
		struct ppd_node * temp = iter;
		iter = iter->next;
		free(data);
		free(temp);
	}
}

struct ppd_stock * ppd_find_stock_id(struct ppd_list* items, const char * item_id){
	struct ppd_stock * item = NULL;
	struct ppd_node * iter = items->head;
	while(iter){
		if(strcmp(iter->data->id, item_id) == 0){
			item = iter->data;
			break;
		}
		iter = iter->next;
	}
	return item;
}

int ppd_save_stock(struct ppd_system * system){
	struct ppd_node * iter;
	FILE * fout = fopen(system->stock_file_name, "w");
	if(fout == NULL){
		perror("fopen");
		return 1;
	}

	iter = system->item_list->head;
	while(iter){
		struct ppd_stock * item = iter->data;
		fprintf(fout, "%s|%s|%s|%i.%02i|%i\n", item->id, item->name, item->desc,
						item->price.dollars, item->price.cents, item->on_hand);
		iter = iter->next;
	}

	fclose(fout);
	return 0;
}

int ppd_stock_generate_id(){
	return ++max_id;
}
