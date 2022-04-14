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

void insert_flight(database *db, flight *new_flight, int origin_airport_index);

void print_flight(flight *flight);

int filter_sort_flights(flight flights[], int n_flights, char airport_id[], flight sorted_flights[], int type);
void print_sorted_flights(flight sorted_flights[], int n_print_flights, int type);