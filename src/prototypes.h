#ifndef __PROTOTYPES_H__
#define __PROTOTYPES_H__

#include "structures.h"
/*
Andre Santos
ist1103597
*/

/**
 * @brief Main function
 * @return 0
 */
int main();

/**
 * @brief Function that handles the input commands 
 * @param db system database pointer
 * @return 0 if 'q' command, 1 otherwise
 */
int handle_command(database *db);

/**
 * @brief Function that handles add airport command (a)
 * @param db system database pointer
 * @return void
 */
void handle_add_airport_command(database *db);

/**
 * @brief Function that handles list airports command (l)
 * @param db system database pointer
 * @return void
 */
void handle_list_airports_command(database *db);

/**
 * @brief Function that handles both add and list command (v)
 * @param db system database pointer
 * @return void
 */
void handle_add_list_fligth_command(database *db);

/**
 * @brief Function that handles list of both origin and destination 
 * flights (c, p)
 * @param db system database pointer
 * @param type 0 to display origin airports, 1 to display destination airports.
 * @return void
 */
void handle_list_origin_destination_command(database *db, int type);
/**
 * @brief Function that handles date forward command (t)
 * @param db system database pointer
 * @return void
 */
void handle_date_forward_command(database *db);

/**
 * @brief This function handles the command that prints or add a reservation (r)
 * @param db system database pointer
 * @return void
 */
void handle_add_list_reservation_command(database *db);

/**
 * @brief This function handles the command that deletes a reservation (e)
 * @param db system database pointer
 */
void handle_delete_flights_reservations_command(database *db);

/**
 * @brief Function that receives an intenger between 1 and 12 and return the 
 correspondent month days
 * @param month month index
 * @return amount of days corresponding to the given month
 */
int get_month_days(int month);

/**
 * @brief Function that convert a date (structure) to days (int)
 * @param date date to be converted
 * @return days corresponding to the given date
 */
int date_to_days(date *date);
/**
 * @brief Function that convert date and time (both structures) to days (int)
 * @param date date to be converted
 * @param time time to be converted
 * @return days corresponding to the given date and time
 */
double date_time_to_days(date *date, time *time);

/**
 * @brief Function that checks if a date is valid or not
 * @param db_date date of current system database
 * @param new_date date to be checked
 * @return 1 if the date is valid, 0 otherwise
 */
int is_valid_date(date *db_date, date *date);

/**
 * @brief Function that updates the flight info to the destination info
 * @param flight flight to be updated
 * @return flight with updated destination info
 */
void update_destination_info(flight *flight);

/**
 * @brief Function that checks if a given airport_id is valid or not
 * @param id id to be checked
 * @return 1 if airport_id is valid, 0 otherwise
 */
int is_valid_airport_id(char id[]);
/**
 * @brief Function that checks if a given airport is a new airport or not
 * @param db system database pointer
 * @param id id to be checked
 * @return 1 if the given airport is new on the database, 0 otherwise
 */
int is_new_airport(database *db, char id[]);

/**
 * @brief Function that receives a new airport and insert it in the right index
 on the database airports
 * @param db system database pointer
 * @param new_airport airport to be inserted
 * @return void
 */
void insert_airport_sorted(database *db, airport *new_airport);

/**
 * @brief Function that receives an id and search for it in the database aiports
 ,returns -1 if the id was not found, the index of that id in the database 
 otherwise
 * @param db system database pointer
 * @param id id to be searched
 * @return -1 if the airport_id was not found, the index in the database 
 * airports otherwise
 */
int found_airport_id(database *db, char id[MAX_AIRPORT_ID_SIZE+1]);

/**
 * @brief Function to print the output of a successfully added airport
 * @param airport airport to be printed
 * @return void
 */
void print_airport(airport *airport);
/**
 * @brief Function that checks if a flight_id is valid or not
 * @param id id to be checked
 * @return 1 if the the given flight_id is valid, 0 otherwise
 */
int is_valid_flight_id(char id[]);
/**
 * @brief Function that checks if a duration is valid or not
 * @param duration duration to be checked
 * @return 1 if duration is valid, 0 otherwise
 */
int is_valid_duration(time *duration);

/**
 * @brief Function that checks if a capacity is valid or not
 * @param db system database pointer
 * @param capacity capacity to be checked
 * @return 1 if capacity is valid, 0 otherwise
 */
int is_valid_capacity(int capacity);

/**
 * @brief Function to insert a flight in the system database 
 * @param db system database pointer
 * @param new_flight flight to be inserted
 * @param origin_airport_index index of the system database where the flight 
 should be inserted in
 * @return void
 */
void insert_flight(database *db, node *flight_node, int origin_airport_index);

/**
 * @brief Function to print the output of a successfully added flight
 * @param flight flight to be printed
 * @return void
 */
void print_flight(flight *flight);
/**
 * @brief Function to both filter and sort the flights that have the requested 
 airport id origin (type = 0) or destination (type = 1)
 * @param flights list of flights to be filtered and sorted 
 * @param n_flights size of the the flights list 
 * @param airport_id requested airport id that need to be in the (destination 
 or origin depending on the type)
 * @param sorted_flights list where the sorted and filtered flights will be 
 stored
 * @param type 0 if we want the flights with the requested airport as origin, 
 1 if want the destination otherwise
 * @return number of flights found
 */
void print_sorted_flights(database* db, int n_flights, char airport_id[], 
int type);

/**
 * @brief This function allocates memory in a controlled way
 * 
 * @param db system database pointer
 * @param size size of the memory to be allocated
 * @return void
 */
void* controlled_malloc(database *db, unsigned int size);

/**
 * @brief This function checks if a reservation id is valid or not 
 * @param id reservation id to be checked
 * @return int 
 */
int is_valid_reservation_id(char id[]);
/**
 * @brief This function checks if a flight already exists in the system database
 * @param db system database pointer
 * @param id flight id to be compared with
 * @param flight_date flight date to be compared with
 * @return node* 
 */
node* found_flight_id(database *db, char id[MAX_FLIGHT_ID_SIZE+1], 
date flight_date);

/**
 * @brief This function deletes a reservation from the system
 * @param db system database pointer
 * @param code reservation code
 * @param reservation_node reservation node
 */
void delete_reservation(database *db, char code[], node* reservation_node);

/**
 * @brief This function deletes a flight from the system
 * @param db system database pointer
 * @param code flight code
 * @param found_flight_node flight node
 */ 
void delete_flight(database *db, char code[], node* found_flight_node);

/* LINKED LISTS */

/**
 * @brief This function initializes a linked list
 * @param db system database pointer
 * @return list* 
 */
list* init_list(database *db);

/**
 * @brief This function inserts a node in a linked list
 * @param linked_list linked list pointer
 * @param new_node node to be inserted
 */
void insert_node(list *linked_list, node *new_node);
/**
 * @brief This function deletes a node from a linked list
 * 
 * @param linked_list linked list pointer
 * @param node_to_delete node to be deleted
 */
void delete_node(list *linked_list, node *node_to_delete);

/**
 * @brief This function frees a node
 * @param node_to_free node to be freed
 */
void free_node(node* node_to_free);

/* HASH TABLE */

/**
 * @brief This function initializes a hash table
 * @param db system database pointer
 * @param size size of the hash table
 * @return hashtable* 
 */
hashtable* init_hashtable(database *db, int size);

/**
 * @brief This function hashes a string
 * @param string string to be hashed
 * @param size size of the hash table
 * @return int 
 */
int hash_string(char* string, int size);

/**
 * @brief This function inserts a node in a hash table
 * @param _hashtable hashtable pointer
 * @param node_to_insert node to be inserted
 * @param key key to be hashed
 */

void insert_in_table(hashtable* _hashtable, node* node_to_insert, char key[]);
/**
 * @brief This function removes a node from a hash table
 * @param _hashtable hashtable pointer
 * @param node_to_remove node to be removed
 * @param key key to be hashed
 */
void remove_from_table(hashtable* _hashtable, node* node_to_remove, char key[]);

/**
 * @brief This function searches for a node in a hash table
 * @param _hashtable hashtable pointer
 * @param key key to be hashed
 * @return void* 
 */
void* search_table(hashtable* _hashtable, char key[]);

/**
 * @brief This function deletes a node from a hash table
 * @param _hashtable hashtable pointer
 */
void delete_table(hashtable* _hashtable);

/**
 * @brief This function frees everything that is still allocated
 * @param db system database pointer
 */
void free_all(database *db);

/**
 * @brief This function sorts a vector of flights
 * @param arr vector of flights
 * @param low starting index
 * @param high ending index
 */

void quickSort(node* arr, int low, int high);
/**
 * @brief This function partitions a vector of flights
 * @param arr vector of flights
 * @param low starting index
 * @param high ending index
 * @return int 
 */

int partition (node *arr, int low, int high);

/**
 * @brief This function swaps two nodes  
 * @param a node pointer
 * @param b node pointer
 */
void swap(node *a, node *b);
 
/**
 * @brief This function handles the creation of a reservation.
 * @param db system database pointer
 * @param reservation_buffer reservation buffer
 * @param reservation_date reservation date
 * @param reservation_flight reservation flight
 * @param reservation_passengers reservation passengers
 */
void make_reservation(database *db, char reservation_buffer[MAX_INPUT], 
date reservation_date, flight *reservation_flight, int reservation_passengers);

/**
 * @brief This function print the reservations of a flight
 * @param db system database pointer
 * @param reservation_flight reservation flight
 */
void print_reservations(database *db, flight *reservation_flight);

#endif