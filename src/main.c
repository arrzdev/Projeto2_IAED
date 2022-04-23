#include "constants.h"
#include "outputs.h"
#include "prototypes.h"
#include "structures.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*
Andre Santos
ist1103597
*/

int main(){
	database db = {0};
	
	db.date.day = 1;
	db.date.month = 1;
	db.date.year = 2022;
	
	db.n_airports = 0;
	db.n_flights = 0;
	
	/* init linked lists */
	db.flights = init_list(&db);

	/* init hashtable */
	db.reservations_hashtable = init_hashtable(&db, 20011);
	db.flights_hashtable = init_hashtable(&db, 20011);

	/* execute till 'q' command is inserted */  
	while (handle_command(&db)){
	}

	return 0;
}

int handle_command(database *db){
	char c = getchar();	
	switch (c){
		case 'q':
			free_all(db);
			return 0;  
		
		case 'a':
			handle_add_airport_command(db);
			return 1;
			
		case 'l':
			handle_list_airports_command(db);
			return 1;
		
		case 'v':
			handle_add_list_fligth_command(db);
			return 1;

		case 'p':
			handle_list_origin_destination_command(db, 0);
			return 1;

		case 'c':
			handle_list_origin_destination_command(db, 1);
			return 1;

		case 't':
			handle_date_forward_command(db);
			return 1;
		
		case 'r':
			handle_add_list_reservation_command(db);
			return 1;
		
		case 'e':
			handle_delete_flights_reservations_command(db);
			return 1;
			
		default:
			return 1;
	}
}

void handle_add_airport_command(database *db){
	int n_airports = db->n_airports;
	
	airport new_airport;
	scanf("%s %s %[^\n]", new_airport.id, new_airport.country, new_airport.city); 
	
	new_airport.n_flights = 0;

	/* invalid airport ID */                                                   
	if (!is_valid_airport_id(new_airport.id)){
		printf(INVALID_AIRPORT_ID);
		return;
	}
	
	/* to many airports */
	if (n_airports == MAX_AIRPORTS){
		printf(TOO_MANY_AIRPORTS);
		return;
	}

	/* duplicate airport*/
	if (!is_new_airport(db, new_airport.id)){
		printf(DUPLICATE_AIRPORTS);
		return;	
	}
	
	/* success */
	insert_airport_sorted(db, &new_airport);
	printf(ADDED_AIRPORT, new_airport.id);
	
	return;
}

void handle_list_airports_command(database *db){
	int i, airport_index;
	int n_airports = db->n_airports;
	
	char id[MAX_AIRPORT_ID_SIZE+1];

	/* no arguments */
	if (getchar() == '\n'){
		for (i=0; i < n_airports; i++){
			print_airport(&db->airports[i]);
		}
	} 
	else {
		do{
			scanf("%s", id);
			airport_index = found_airport_id(db, id);
			
			if (airport_index == -1)
				printf(NO_SUCH_AIRPORT, id);
			else
				print_airport(&db->airports[airport_index]);	
		
		} while(getchar() != '\n');		
	}
}

void handle_add_list_fligth_command(database *db){
	/* no arguments */
	if (getchar() == '\n'){
		node * current_node = db->flights->tail;

		for (; current_node; current_node = current_node->list_previous){
			flight* current_node_data = (flight*)current_node->data;
			print_flight(current_node_data);
		}
	}
	else {
		flight* new_flight;
		int origin_airport_index;
		node* new_node;

		new_flight = (flight*) controlled_malloc(db, sizeof(flight));

		scanf("%s %s %s %02d-%02d-%d %02d:%02d %02d:%02d %d", new_flight->id, 
		new_flight->origin_id, new_flight->destination_id, &new_flight->date.day, 
		&new_flight->date.month, &new_flight->date.year, &new_flight->time.hours, 
		&new_flight->time.minutes, &new_flight->duration.hours, 
		&new_flight->duration.minutes, &new_flight->capacity);

		/* invalid flight code*/
		if (!is_valid_flight_id(new_flight->id)){
			printf(INVALID_FLIGHT_CODE);
			free(new_flight);
			return;
		}

		/* flight already exists */
		if (found_flight_id(db, new_flight->id, new_flight->date)){
			printf(FLIGHT_ALREADY_EXISTS);
			free(new_flight);
			return;
		}

		/* no such airport ID*/
		origin_airport_index = found_airport_id(db, new_flight->origin_id);
		if (origin_airport_index == -1){
			printf(NO_SUCH_AIRPORT, new_flight->origin_id);
			free(new_flight);
			return;
		}
		
		else if (found_airport_id(db, new_flight->destination_id) == -1){
			printf(NO_SUCH_AIRPORT, new_flight->destination_id);
			free(new_flight);
			return;
		}

		/* too many flights */
		if (db->n_flights == MAX_FLIGHTS){
			printf(TOO_MANY_FLIGHTS);
			free(new_flight);
			return;
		}

		/* invalid date */
		if (!is_valid_date(&db->date, &new_flight->date)){
			printf(INVALID_DATE);
			free(new_flight);
			return;
		}

		/* invalid duration */
		if (!is_valid_duration(&new_flight->duration)){
			printf(INVALID_DURATION);
			free(new_flight);
			return;
		}
		
		/* invalid capacity */
		if (!is_valid_capacity(new_flight->capacity)){
			printf(INVALID_CAPACITY);
			free(new_flight);
			return;
		}
		
		/* init left_capacity reservations */
		new_flight->left_capacity = new_flight->capacity;
		new_flight->n_reservations = 0;
		
		/* init linked list */
		new_flight->reservations = init_list(db);

		new_node = (node*) controlled_malloc(db, sizeof(node));
		new_node->hash_next = NULL;
		new_node->hash_previous = NULL;
		new_node->list_next = NULL;
		new_node->list_previous = NULL;
		new_node->data = (void*) new_flight;

		insert_in_table(db->flights_hashtable, new_node, new_flight->id);
		insert_flight(db, new_node, origin_airport_index);
	}
} 

void handle_list_origin_destination_command(database *db, int type){
	int n_flights = db->n_flights;
	char airport_id[MAX_AIRPORT_ID_SIZE+1];
	
	scanf("%s", airport_id);
	
	/* no such aiport */
	if (found_airport_id(db ,airport_id) == -1){
		printf(NO_SUCH_AIRPORT, airport_id);
		return;
	}

	print_sorted_flights(db, n_flights, airport_id, type);
}

void handle_date_forward_command(database *db){
	date new_date;
	
	scanf("%02d-%02d-%d", &new_date.day, &new_date.month, &new_date.year);
	
	/* invalid date */
	if (!is_valid_date(&db->date, &new_date)){
		printf(INVALID_DATE);
		return;
	}

	db->date = new_date;
	printf(PRINT_DATE, new_date.day, new_date.month, new_date.year);
}

void handle_add_list_reservation_command(database *db){
	/* create new reservation */
	char flight_id[MAX_FLIGHT_ID_SIZE+1];
	date reservation_date;
	
	scanf("%s %02d-%02d-%d", flight_id, &reservation_date.day, 
	&reservation_date.month, &reservation_date.year);

	/* if we have a reservation id*/
	if (getchar() != '\n'){
		char reservation_buffer[MAX_INPUT];
		int reservation_passengers;

		node* reservation_flight_node;
		flight* reservation_flight;
		
		/* read the other 2 arguments */
		scanf("%s %d", reservation_buffer, &reservation_passengers);
		
		/* valid reservation id */
		if (!is_valid_reservation_id(reservation_buffer)){
			printf(INVALID_RESERVATION_CODE);
			return;
		}
	
		/* flight exists */
		reservation_flight_node = found_flight_id(db, flight_id, reservation_date);

		if (!reservation_flight_node){
			printf(FLIGHT_DOESNT_EXIST, flight_id);
			return;
		}

		/* flight reservation already used */
		if (search_table(db->reservations_hashtable, reservation_buffer)){
			printf(FLIGHT_RESERVATION_ALREADY_IN_USE, reservation_buffer);
			return;
		}
		
		reservation_flight = (flight*) reservation_flight_node->data;
		
		/* valid passengers amount */
		if (reservation_passengers > reservation_flight->left_capacity){
			printf(TOO_MANY_RESERVATIONS);
			return;
		}
		
		/* valid date */
		if (!is_valid_date(&db->date, &reservation_date)){
			printf(INVALID_DATE);	
			return;
		}
		
		if (reservation_passengers <= 0){
			printf(INVALID_PASSENGER_NUMBER);
			return;
		}
		
		/* allocate reservation_id */
		make_reservation(db, reservation_buffer, reservation_date, reservation_flight, reservation_passengers);
	}
	/* print */
	else{
		node* reservation_flight_node;
		flight* reservation_flight;

		/* flight exists */
		reservation_flight_node = found_flight_id(db, flight_id, reservation_date);
		if (!reservation_flight_node){
			printf(FLIGHT_DOESNT_EXIST, flight_id);
			return;
		}
	
		/* valid date */
		if (!is_valid_date(&db->date, &reservation_date)){
			printf(INVALID_DATE);	
			return;
		}
		
		reservation_flight = (flight*) reservation_flight_node->data;

		print_reservations(db, reservation_flight);
	}
}

void handle_delete_flights_reservations_command(database *db){
	char code[65500];
	scanf("%s", code);
	
	/* delete flight and his reservations */
	if (strlen(code) < 10){
		node* flight_node;

		/* flight exists */
		flight_node = search_table(db->flights_hashtable, code);
		if (!flight_node){
			printf(NOT_FOUND);
			return;
		}

		delete_flight(db, code, flight_node);
	}
	else {
		node* reservation_node;

		reservation_node = search_table(db->reservations_hashtable, code);
		if (!reservation_node){
			printf(NOT_FOUND);
			return;
		}

		delete_reservation(db, code, reservation_node);
	}
}

int get_month_days(int month){
	int year_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return year_days[month-1];
}

int date_to_days(date *date){
	int i, days;
	
	/* add days corresponding to past years and days of current month*/
	days = date->day + (date->year - 1) * 365;

	/* add the days corresponding to the months that passed in the current year */
	for (i=1; i<date->month; i++){
		days += get_month_days(i);
	}
	
	return days;
}

double date_time_to_days(date *date, time *time){
	double days;
	
	days = date_to_days(date);
	
	/* add the time converted to days */
	days += (double)(time->hours + ((double)time->minutes / 60)) / 24;
	
	return days;
}

int is_valid_date(date *db_date, date *date){
	int db_days = date_to_days(db_date);
	int new_date_days = date_to_days(date);

	/* future +1 year */
	if (new_date_days - db_days > 365)
		return 0;
	
	/* past */
	else if (db_days > new_date_days)
		return 0;

	return 1;
}

void update_destination_info(flight *flight){
	int max_month_days;
	
	/* add minutes */
	flight->time.minutes += flight->duration.minutes;
	if (flight->time.minutes >= 60){
		flight->time.minutes -= 60;
		flight->time.hours += 1;
	}
	
	/* add hours */
	flight->time.hours += flight->duration.hours;
	if (flight->time.hours >= 24){
		flight->time.hours -= 24;
		flight->date.day += 1;
	}
	
	max_month_days = get_month_days(flight->date.month);
	if (flight->date.day > max_month_days){
		flight->date.day -= max_month_days;
		flight->date.month += 1;
	}
	
	if (flight->date.month > 12){
		flight->date.month -= 12;
		flight->date.year += 1;
	}
}

int is_valid_airport_id(char id[]){
	int i;
	
	for (i=0; i < (int)strlen(id); i++){
		if (!isupper(id[i]))
			return 0;
	}
	
	return 1;
}

int is_new_airport(database *db, char id[]){
	int i;
	
	for (i=0; i < db->n_airports; i++){
		if (!strcmp(db->airports[i].id, id))
			return 0;
	}
	
	return 1;
}

void insert_airport_sorted(database *db, airport *new_airport){
	int j;

	/* find the insertion index */
	j = db->n_airports-1;
	while (j>=0 && strcmp(db->airports[j].id, new_airport->id) > 0){
		db->airports[j+1] = db->airports[j];
		j--;
	}
	
	/* insert it */
	db->airports[j+1] = *new_airport;
	db->n_airports++;
}

int found_airport_id(database *db, char id[MAX_AIRPORT_ID_SIZE+1]){
	int i;

	for (i=0; i<db->n_airports; i++){
			airport iteration_airport = db->airports[i];

			if (!strcmp(iteration_airport.id, id))
				return i;
	}
	
	return -1;
}

void print_airport(airport *airport){
	printf(LIST_AIRPORT, airport->id, airport->city, airport->country, 
	airport->n_flights);
}

int is_valid_flight_id(char id[]){
	int i;
	
	for (i=0; i < (int)strlen(id); i++){
		if (i == 0 || i == 1){
			if (!isupper(id[i]))
				return 0;
		}
		else if (!isdigit(id[i]))
			return 0;
	}
	
	return 1;
}

int is_valid_duration(time *duration){
	if (duration->hours > 12 || (duration->hours == 12 && duration->minutes > 0))
		return 0;
	
	return 1;
}

int is_valid_capacity(int capacity){
	if (capacity < 10)
		return 0;
		
	return 1;
}

void insert_flight(database *db, node *flight_node, int origin_airport_index){
	insert_node(db->flights, flight_node);
	db->n_flights++;
	
	db->airports[origin_airport_index].n_flights++;
}

void print_flight(flight *flight){
	printf(LIST_FLIGHT, flight->id, flight->origin_id, 
	flight->destination_id, flight->date.day, 
	flight->date.month, flight->date.year, 
	flight->time.hours, flight->time.minutes);
}

void print_sorted_flights(database* db, int n_flights, char airport_id[], 
int type){
	int i, j, passed_filter, n_filtered_flights;
	
	flight* sorted_flights;	
	node* current_node;

	flight flight_data;

	/* sort */
	sorted_flights = (flight*) controlled_malloc(db, sizeof(flight) * n_flights);

	/* copy and filter data */
	n_filtered_flights = 0;
	for (current_node = db->flights->head; current_node; 
	current_node = current_node->list_next){
		flight_data = *(flight*)current_node->data;

		passed_filter = 0;

		/* for destination flights */
    if (type){
      /* update the destination date */
      update_destination_info(&flight_data);
      
      passed_filter = !strcmp(flight_data.destination_id, airport_id);
    }
    else{
      passed_filter = !strcmp(flight_data.origin_id, airport_id);
    }
		
		if (passed_filter){
			/* find the insertion index */
			j = n_filtered_flights-1;
			while (j>=0 && date_time_to_days(&sorted_flights[j].date, 
			&sorted_flights[j].time) > date_time_to_days(&flight_data.date, 
			&flight_data.time)){
				sorted_flights[j+1] = sorted_flights[j];
				j--;
			}

			/* insert it */
			sorted_flights[j+1] = flight_data;
			n_filtered_flights++;
		}
	}

	/* print */
	for (i=0; i<n_filtered_flights; i++){;
		if (type)
			printf(LIST_FLIGHT_AIRPORT, sorted_flights[i].id, 
			sorted_flights[i].origin_id, sorted_flights[i].date.day, 
			sorted_flights[i].date.month, sorted_flights[i].date.year, 
			sorted_flights[i].time.hours, sorted_flights[i].time.minutes);
		else
			printf(LIST_FLIGHT_AIRPORT, sorted_flights[i].id, 
			sorted_flights[i].destination_id, sorted_flights[i].date.day, 
			sorted_flights[i].date.month, sorted_flights[i].date.year, 
			sorted_flights[i].time.hours, sorted_flights[i].time.minutes);
	}

	free(sorted_flights);
}

void* controlled_malloc(database *db, unsigned int size){
	void* p = malloc(size);
	
	/* if we had memory */
	if (p)
		return p;

	/* I need to free everything */ 
	free_all(db);
	printf(NO_MEMORY);
	exit(NO_MEMORY_EXIT_CODE);
	return NULL;
}

int is_valid_reservation_id(char id[]){
	int i;
	
	int id_len = strlen(id);
	
	if (id_len < 10)
	  return 0;
	
	for (i=0; i < id_len; i++){
		if (!(isupper(id[i]) || isdigit(id[i]))){
			return 0;
		}
	}
	
	return 1;
}

node* found_flight_id(database *db, char id[MAX_FLIGHT_ID_SIZE+1], 
date flight_date){
	node* flight_node = search_table(db->flights_hashtable, id);

	for (; flight_node; flight_node = flight_node->hash_next){
		flight * flight_node_content = flight_node->data;
		if (date_to_days(&flight_node_content->date) == date_to_days(&flight_date)){
			return flight_node;
		}
	}

	return NULL;
}

void delete_reservation(database *db, char code[], node* reservation_node){
	reservation * reservation_data;

	reservation_data = (reservation *) reservation_node->data;

	/* increment flight capacity left */
	reservation_data->reservation_flight->left_capacity += 
	reservation_data->n_passengers ;

	/* decrement n_reservations */
	reservation_data->reservation_flight->n_reservations--;
	
	remove_from_table(db->reservations_hashtable, reservation_node, code);
	
	free(reservation_data->reservation_id);

	delete_node(reservation_data->reservation_flight->reservations, 
	reservation_node);
}

void delete_flight(database *db, char code[], node* found_flight_node){
	node *flight_node = found_flight_node;
	node *next_flight_node;
	flight flight_data;

	node *reservation_node;
	node *next_reservation_node;
	reservation *reservation_data;


	/* iterate trought the flights*/
	while (flight_node){
		/* :) */
		flight_data = *(flight*) flight_node->data;
		next_flight_node = flight_node->hash_next;

		db->n_flights--;

		/* we need to delete every reservation linked to this flight*/
		reservation_node = flight_data.reservations->head;
		
		/* delete and free nodes */
		while (reservation_node){
			next_reservation_node = reservation_node->list_next;
			
			reservation_data = (reservation*) reservation_node->data;

			/* delete reservation from hashtable */
			remove_from_table(db->reservations_hashtable, reservation_node, 
			reservation_data->reservation_id);

			/* free reservation code */
			free(reservation_data->reservation_id);
			free_node(reservation_node);

			reservation_node = next_reservation_node;
		}
	
		/* free the memory allocated for the flight linked list */
		free(flight_data.reservations);
		
		/* delete reservation from hashtable */
		remove_from_table(db->flights_hashtable, flight_node, code);

		/* delete / free flight node */
		delete_node(db->flights, flight_node);

		flight_node = next_flight_node;
	}
}


/* LINKED LISTS */

list* init_list(database *db){
	list* new_list = (list*)  controlled_malloc(db, sizeof(list));
	new_list->head = NULL;
	new_list->tail = NULL;
	
	return new_list;
}

void insert_node(list *linked_list, node *new_node){
	if (!linked_list->head){
		linked_list->head = new_node;
		linked_list->tail = new_node;
	}
	else{
		 new_node->list_next = linked_list->head;
		 linked_list->head->list_previous = new_node;
		 linked_list->head = new_node;
	}
}

void delete_node(list *linked_list, node *node_to_delete){
	if (node_to_delete->list_previous)
		node_to_delete->list_previous->list_next = node_to_delete->list_next;
	else
		linked_list->head = node_to_delete->list_next;
	
	if (node_to_delete->list_next)
		node_to_delete->list_next->list_previous = node_to_delete->list_previous;
	else
		linked_list->tail = node_to_delete->list_previous;

	free_node(node_to_delete);
}

void free_node(node* node_to_free){
	free(node_to_free->data);
	free(node_to_free);
}

/* HASH TABLE */

hashtable* init_hashtable(database *db, int size){
	int i;
	hashtable* new_hashtable = (hashtable*) controlled_malloc(db, 
	sizeof(hashtable));
	
	node** table = (node**) controlled_malloc(db, sizeof(node*) * size);
	
	new_hashtable->table = table;
	new_hashtable->size = size;
	
	/* set every hash as NULL */
	for (i=0; i<size; i++){
		table[i] = NULL;
	}
	
	return new_hashtable;
}

int hash_string(char* string, int size){
	int hash, a = 31415, b = 27183;

	for (hash = 0; *string != '\0'; string++, a = a*b % (size-1))

	hash = (a*hash + *string) % size;
	return hash;
}

void insert_in_table(hashtable* _hashtable, node* node_to_insert, char key[]){
	int i = hash_string(key, _hashtable->size);

	if (!_hashtable->table[i]){
		_hashtable->table[i] = node_to_insert;
	}
	else{
		node_to_insert->hash_next = _hashtable->table[i];
		_hashtable->table[i]->hash_previous = node_to_insert;
		_hashtable->table[i] = node_to_insert;
	}
}

void remove_from_table(hashtable* _hashtable, node* node_to_remove, char key[]){
	int i = hash_string(key, _hashtable->size);
	
	if (node_to_remove->hash_previous)
		node_to_remove->hash_previous->hash_next = node_to_remove->hash_next;
	else
		_hashtable->table[i] = node_to_remove->hash_next;
	
	if (node_to_remove->hash_next)
		node_to_remove->hash_next->hash_previous = node_to_remove->hash_previous;
}

void* search_table(hashtable* _hashtable, char key[]){
	int i = hash_string(key, _hashtable->size);
	int key_len = strlen(key);

	node* current_node = _hashtable->table[i];

	while (current_node){
		if (key_len >= 10){ /* reservation search */
			reservation* current_node_data = (reservation*)current_node->data;
			if (!strcmp(current_node_data->reservation_id, key))
				return current_node;
		}
		else{ /* flight search */
			flight* current_node_data = (flight*)current_node->data;
			if (!strcmp(current_node_data->id, key))
				return current_node;
		}
		
		current_node = current_node->hash_next;
	}
	
	return NULL;
}

void delete_table(hashtable* _hashtable){
	/* delete hashtable doesnt free nodes */
	free(_hashtable->table);
	free(_hashtable);
}

void free_all(database *db){
	node* flight_node;
	node* next_flight_node;
	flight* flight_data;

	node* reservation_node;
	node* next_reservation_node;
	reservation* reservation_data;

	/* free hashtable */
	delete_table(db->reservations_hashtable);
	delete_table(db->flights_hashtable);

	/* iterate flights linked list */
	flight_node = db->flights->head;
	while (flight_node){
		next_flight_node = flight_node->list_next;
		flight_data = (flight*) flight_node->data;

		/* free linked list */
		reservation_node = flight_data->reservations->head;
		while (reservation_node){
			next_reservation_node = reservation_node->list_next;
			reservation_data = (reservation*) reservation_node->data;

			free(reservation_data->reservation_id);
			free_node(reservation_node);

			reservation_node = next_reservation_node;
		}

		free(flight_data->reservations);
		free_node(flight_node);

		flight_node = next_flight_node;
	}
	
	free(db->flights);
}

void quickSort(node* arr, int low, int high){
	if (low < high){
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

int partition (node *arr, int low, int high){
	int j;
	char *pivot = ((reservation*) arr[high].data)->reservation_id;

	int i = (low - 1);

	for (j = low; j <= high - 1; j++){
		if (strcmp(((reservation*) arr[j].data)->reservation_id, pivot) < 0){
			i++; 
			
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void swap(node *a, node *b){
    node t = *a;
    *a = *b;
    *b = t;
}

void make_reservation(database *db, char reservation_buffer[MAX_INPUT], 
date reservation_date, flight *reservation_flight, int reservation_passengers){
	char* reservation_id;
	reservation* new_reservation;
	node* new_node;

	reservation_id = (char *) controlled_malloc(db, 
	strlen(reservation_buffer) + 1);
	strcpy(reservation_id, reservation_buffer);
	
	new_reservation = (reservation*) controlled_malloc(db, sizeof(reservation));
	new_reservation->date = reservation_date;
	new_reservation->reservation_flight = reservation_flight;
	new_reservation->n_passengers = reservation_passengers;
	new_reservation->reservation_id = reservation_id;

	new_node = (node *) controlled_malloc(db, sizeof(node));
	new_node->data = new_reservation;
	new_node->list_next = NULL;
	new_node->list_previous = NULL;
	new_node->hash_next = NULL;
	new_node->hash_previous = NULL;
	
	insert_node(reservation_flight->reservations, new_node);
	insert_in_table(db->reservations_hashtable, new_node, reservation_id);
	
	reservation_flight->left_capacity -= reservation_passengers;
	reservation_flight->n_reservations++;
}

void print_reservations(database *db, flight *reservation_flight){
	node* sorted_flights;
	node* reservation_node;
	int i;
	
	/* sort */
	sorted_flights = (node*) controlled_malloc(db, sizeof(node) * 
	reservation_flight->n_reservations);

	/* copy data */
	i = 0;
	for (reservation_node = reservation_flight->reservations->head; 
	reservation_node; reservation_node = reservation_node->list_next){
		sorted_flights[i] = *reservation_node;
		i++;
	}

	/* sort flgiths to be printed */
	quickSort(sorted_flights, 0, reservation_flight->n_reservations - 1);

	/* print */
	for (i=0; i<reservation_flight->n_reservations; i++){
		reservation* current_reservation_data = (reservation*) 
		sorted_flights[i].data;

		printf(PRINT_RESERVATION, current_reservation_data->reservation_id, 
		current_reservation_data->n_passengers);
	}

	free(sorted_flights);	
}

/**/