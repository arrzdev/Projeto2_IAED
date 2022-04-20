#ifndef __PROTOTYPES_H__
#define __PROTOTYPES_H__

#include "structures.h"

int handle_command(database *db);

void handle_add_airport_command(database *db);
void handle_list_airports_command(database *db);
void handle_add_list_fligth_command(database *db);
void handle_list_origin_destination_command(database *db, int type);
void handle_date_forward(database *db);

int get_month_days(int month);
int date_to_days(date *date);
double date_time_to_days(date *date, time *time);
int is_valid_date(date *db_date, date *date);

void update_destination_info(flight *flight);

int is_valid_airport_id(char id[]);
int is_new_airport(database *db, char id[]);

void insert_airport_ordenated(database *db, airport *new_airport);

int found_airport_id(database *db, char id[MAX_AIRPORT_ID_SIZE+1]);

void print_airport(airport *airport);

int is_valid_flight_id(char id[]);

int is_new_flight(database *db, flight *new_flight);

int is_valid_duration(time *duration);

int is_valid_capacity(int capacity);

void insert_flight(database *db, node *flight_node, int origin_airport_index);

void print_flight(flight *flight);

int filter_sort_flights(flight flights[], int n_flights, char airport_id[], flight sorted_flights[], int type);
void print_sorted_flights(database* db, int n_flights, char airport_id[], int type);

/* 
2nd projeto
*/

void* controlled_malloc(database *db, unsigned int size);

void handle_add_list_reservation_command(database *db);
void handle_delete_flights_reservations_command(database *db);

int is_valid_reservation_id(char id[]);
node* found_flight_id(database *db, char id[MAX_FLIGHT_ID_SIZE+1], date flight_date);

/* LINKED LISTS */
list* init_list(database *db);
void insert_node(list *reservations, node *new_node);
void delete_list(list *reservations);
void free_node(node* node_to_free);
void delete_node(list *linked_list, node *current_node);

/* HASH TABLES */
hashtable* init_hashtable(database *db, int size);

int hash_string(char* v, int size);

void insert_in_table(hashtable* _hashtable, node* node_to_insert, char key[]);

void remove_from_table(hashtable* _hashtable, node* node_to_remove, char key[]);

void delete_table(hashtable* hashtable_to_delete);
void* search_table(hashtable* table, char key[]);

void free_all(database *db);

#endif