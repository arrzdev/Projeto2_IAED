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
	
	/* init hashtable */
	db.reservations_hashtable = init_hashtable(&db, 20011);

	/* execute till 'q' command is inserted */  
	while (handle_command(&db)){
	}

	return 0;
}

/**
 * \brief Function that handles the input commands 
 * \param *db system database pointer
 * \return 0 if 'q' command, 1 otherwise
 */
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
			handle_date_forward(db);
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

/**
 * \brief Function that handles add airport command (a)
 * \param *db system database pointer
 * \return void
 */
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
	insert_airport_ordenated(db, &new_airport);
	printf(ADDED_AIRPORT, new_airport.id);
	
	return;
}

/**
 * \brief Function that handles list airports command (l)
 * \param *db system database pointer
 * \return void
 */
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

/**
 * \brief Function that handles both add and list command (v)
 * \param *db system database pointer
 * \return void
 */
void handle_add_list_fligth_command(database *db){
	/* no arguments */
	if (getchar() == '\n'){
		int i, n_flights = db->n_flights;
	
		for (i=0; i < n_flights; i++){
			print_flight(&db->flights[i]);
		}
	}
	else {
		flight new_flight;
		int origin_airport_index;

		scanf("%s %s %s %02d-%02d-%d %02d:%02d %02d:%02d %02d", new_flight.id, 
		new_flight.origin_id, new_flight.destination_id, &new_flight.date.day, 
		&new_flight.date.month, &new_flight.date.year, &new_flight.time.hours, 
		&new_flight.time.minutes, &new_flight.duration.hours, 
		&new_flight.duration.minutes, &new_flight.capacity);

		/* invalid flight code*/
		if (!is_valid_flight_id(new_flight.id)){
			printf(INVALID_FLIGHT_CODE);
			return;
		}

		/* flight already exists */
		if (!is_new_flight(db, &new_flight)){
			printf(FLIGHT_ALREADY_EXISTS);
			return;
		}

		/* no such airport ID*/
		origin_airport_index = found_airport_id(db, new_flight.origin_id);
		if (origin_airport_index == -1){
			printf(NO_SUCH_AIRPORT, new_flight.origin_id);
			return;
		}
		
		else if (found_airport_id(db, new_flight.destination_id) == -1){
			printf(NO_SUCH_AIRPORT, new_flight.destination_id);
			return;
		}

		/* too many flights */
		if (db->n_flights == MAX_FLIGHTS){
			printf(TOO_MANY_FLIGHTS);
			return;
		}

		/* invalid date */
		if (!is_valid_date(&db->date, &new_flight.date)){
			printf(INVALID_DATE);
			return;
		}

		/* invalid duration */
		if (!is_valid_duration(&new_flight.duration)){
			printf(INVALID_DURATION);
			return;
		}
		
		/* init left_capacity reservations */
		new_flight.left_capacity = new_flight.capacity;
		
		/* init linked list */
		new_flight.reservations = init_list(db);
	
		insert_flight(db, &new_flight, origin_airport_index);
	}
}

/**
 * \brief Function that handles list of both origin and destination flights (c, p)
 * \param *db system database pointer
 * \param type 0 to display origin airports, 1 to display destination airports.
 * \return void
 */
void handle_list_origin_destination_command(database *db, int type){
	int n_print_flights, n_flights = db->n_flights;
	char airport_id[MAX_AIRPORT_ID_SIZE+1];
	flight sorted_flights[MAX_FLIGHTS];
	
	scanf("%s", airport_id);
	
	/* no such aiport */
	if (found_airport_id(db ,airport_id) == -1){
		printf(NO_SUCH_AIRPORT, airport_id);
		return;
	}

	n_print_flights = filter_sort_flights(db->flights, n_flights, airport_id, 
	sorted_flights, type);

	print_sorted_flights(sorted_flights, n_print_flights, type);
}

/**
 * \brief Function that handles date forward command (t)
 * \param *db system database pointer
 * \return void
 */
void handle_date_forward(database *db){
	date NewDate;
	
	scanf("%02d-%02d-%d", &NewDate.day, &NewDate.month, &NewDate.year);
	
	/* invalid date */
	if (!is_valid_date(&db->date, &NewDate)){
		printf(INVALID_DATE);
		return;
	}
	
	db->date = NewDate;
	printf(PRINT_DATE, NewDate.day, NewDate.month, NewDate.year);
}

/**
 * @brief 
 * 
 */
void handle_add_list_reservation_command(database *db){
	/* create new reservation */
	char flight_id[MAX_FLIGHT_ID_SIZE+1];
	date date;
	flight* reservation_flight;
	
	scanf("%s %02d-%02d-%d", flight_id, &date.day, &date.month, &date.year);

	/* if we have a reservation id*/
	if (getchar() != '\n'){
		char reservation_buffer[65500];
		int n_passengers;
		int flight_index;
		reservation* new_reservation;
		
		node* new_node;
		char * reservation_id;
		
		/* read the other 2 arguments */
		scanf("%s %d", reservation_buffer, &n_passengers);
		
		/* valid reservation id */
		if (!is_valid_reservation_id(reservation_buffer)){
			printf(INVALID_RESERVATION_CODE);
			return;
		}
	
		/* flight exists */
		flight_index = found_flight_id(db, flight_id, date);
		
		if (flight_index == -1 &&
		date_to_days(&db->flights[flight_index].date) != date_to_days(&date)){
			printf(FLIGHT_DOESNT_EXIST, flight_id);
			return;
		}

		/* get the flight */
		reservation_flight = &db->flights[flight_index];
		
		/* flight reservation already used */
		if (search_table(db->reservations_hashtable, reservation_buffer)){
			printf(FLIGHT_RESERVATION_ALREADY_IN_USE, reservation_buffer);
			return;
		}
		
		/* valid passengers amount */
		if (n_passengers > reservation_flight->left_capacity){
			printf(TOO_MANY_RESERVATIONS);
			return;
		}
		
		/* valid date */
		if (!is_valid_date(&db->date, &date)){
			printf(INVALID_DATE);	
			return;
		}
		
		if (n_passengers <= 0){
			printf(INVALID_PASSENGER_NUMBER);
			return;
		}
		
		/* reserve passengers */
		reservation_flight->left_capacity -= n_passengers;
		
		/* allocate reservation_id */
		reservation_id = (char *) controlled_malloc(db, strlen(reservation_buffer) + 1);
		strcpy(reservation_id, reservation_buffer);
		
		/* allocate reservation structure */
		new_reservation = (reservation *) controlled_malloc(db, sizeof(reservation));
		
		/* insert the data into the reservation structure */
		new_reservation->date = date;
		new_reservation->reservation_flight = reservation_flight;
		new_reservation->n_passengers = n_passengers;
		new_reservation->reservation_id = reservation_id;
		
		/* create new node */
		new_node = (node *) controlled_malloc(db, sizeof(node));
		
		/* insert data into node */
		new_node->reservation = new_reservation;
		
		/* i might get naughty here and insert it ordenated :D */
		/* i did it :) */
		insert_node(reservation_flight->reservations, new_node);
		
		/* insert in hashtable */
		insert_table(db->reservations_hashtable, new_node);
	}
	/* print */
	else{
		node* current_node;
		int flight_index;
		
		/* flight exists */
		flight_index = found_flight_id(db, flight_id, date);
		if (flight_index == -1 &&
		date_to_days(&db->flights[flight_index].date) != date_to_days(&date)){
			printf(FLIGHT_DOESNT_EXIST, flight_id);
			return;
		}
	
		/* get the flight_index of the correct day */
		reservation_flight = &db->flights[flight_index];
		
		/* valid date */
		if (!is_valid_date(&db->date, &date)){
			printf(INVALID_DATE);	
			return;
		}

		/*  print them */
		for (current_node = reservation_flight->reservations->head; current_node;
		current_node = current_node->next){
			printf(PRINT_RESERVATIONS, current_node->reservation->reservation_id, 
			current_node->reservation->n_passengers);
		}
	}
}

/**
 * @brief 
 * 
 */
void handle_delete_flights_reservations_command(database *db){
	char code[65500];
	node* current_node;
	scanf("%s", code);
	
	/* flight code */
	if (strlen(code) < 10){
		node* next_node;
		flight* iteration_flight;
		int i, j, found=0;
		
		/* iterate trought the flights*/
		for (i=0; i<(db->n_flights); i++){
			/* get the flight */
			iteration_flight = &db->flights[i];
		
			if (!strcmp(iteration_flight->id, code)){
				found++;
			
				/* we need to delete every reservation linked to this flight*/
				current_node = iteration_flight->reservations->head;
				
				/* delete and free nodes */
				while (current_node){
					next_node = current_node->next;
					
					delete_table(db->reservations_hashtable, current_node);
					
					current_node = next_node;
				}
		
				/* free the memory allocated for the flight linked list */
				delete_list(iteration_flight->reservations);
		
				/* now we need to delete the flight */
				for (j=i; j<db->n_flights; j++){
					/* shift elements to the left*/
					if (j+1)
					db->flights[j] = db->flights[j+1];
				}
				
				db->n_flights--;
				
				/* since we shifted the list to the left we need to check index i again */
				i--;
			}
		}
		
		if (!found){
			printf(NOT_FOUND);
			return;
		}
		
	}
	
	else {
		node* reservation_node;
		
		/* reservation code */
		reservation_node = search_table(db->reservations_hashtable, code);
		
		if (!reservation_node){
			printf(NOT_FOUND);
			return;
		}
		
		/* increment flight capacity left */
		reservation_node->reservation->reservation_flight->left_capacity += 
		reservation_node->reservation->n_passengers;
		
		delete_table(db->reservations_hashtable, reservation_node);
		delete_node(reservation_node->reservation->reservation_flight->reservations, reservation_node);
		
	}
}

/**
 * \brief Function that receives an intenger between 1 and 12 and return the 
 correspondent month days
 * \param month month index
 * \return amount of days corresponding to the given month
 */
int get_month_days(int month){
	int year_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return year_days[month-1];
}

/**
 * \brief Function that convert a date (structure) to days (int)
 * \param date date to be converted
 * \return days corresponding to the given date
 */
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

/**
 * \brief Function that convert date and time (both structures) to days (int)
 * \param date date to be converted
 * \param time time to be converted
 * \return days corresponding to the given date and time
 */
double date_time_to_days(date *date, time *time){
	double days;
	
	days = date_to_days(date);
	
	/* add the time converted to days */
	days += (double)(time->hours + ((double)time->minutes / 60)) / 24;
	
	return days;
}

/**
 * \brief Function that checks if a date is valid or not
 * \param db_date date of current system database
 * \param new_date date to be checked
 * \return 1 if the date is valid, 0 otherwise
 */
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

/**
 * \brief Function that updates the flight info to the destination info
 * \param flight flight to be updated
 * \return flight with updated destination info
 */
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

/**
 * \brief Function that checks if a given airport_id is valid or not
 * \param id id to be checked
 * \return 1 if airport_id is valid, 0 otherwise
 */
int is_valid_airport_id(char id[]){
	int i;
	
	for (i=0; i < (int)strlen(id); i++){
		if (!isupper(id[i]))
			return 0;
	}
	
	return 1;
}

/**
 * \brief Function that checks if a given airport is a new airport or not
 * \param *db system database pointer
 * \param id id to be checked
 * \return 1 if the given airport is new on the database, 0 otherwise
 */
int is_new_airport(database *db, char id[]){
	int i;
	
	for (i=0; i < db->n_airports; i++){
		if (!strcmp(db->airports[i].id, id))
			return 0;
	}
	
	return 1;
}

/**
 * \brief Function that receives a new airport and insert it in the right index
 on the database airports
 * \param *db system database pointer
 * \param new_airport airport to be inserted
 * \return void
 */
void insert_airport_ordenated(database *db, airport *new_airport){
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

/**
 * \brief Function that receives an id and search for it in the database aiports,
 returns -1 if the id was not found, the index of that id in the database otherwise
 * \param *db system database pointer
 * \param id id to be searched
 * \return -1 if the airport_id was not found, the index in the database airports
 otherwise
 */
int found_airport_id(database *db, char id[MAX_AIRPORT_ID_SIZE+1]){
	int i;

	for (i=0; i<db->n_airports; i++){
			airport iteration_airport = db->airports[i];

			if (!strcmp(iteration_airport.id, id))
				return i;
	}
	
	return -1;
}

/**
 * \brief Function to print the output of a successfully added airport
 * \param airport airport to be printed
 * \return void
 */
void print_airport(airport *airport){
	printf(LIST_AIRPORT, airport->id, airport->city, airport->country, 
	airport->n_flights);
}

/**
 * \brief Function that checks if a flight_id is valid or not
 * \param id id to be checked
 * \return 1 if the the given flight_id is valid, 0 otherwise
 */
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

/**
 * \brief Function that checks if a given flight is a new flight or not
 * \param *db system database pointer
 * \param new_flight flight to be checked
 * \return 1 if the flight_id is valid, 0 otherwise 
 */
int is_new_flight(database *db, flight *new_flight){
	int i;
	
	for (i=0; i<db->n_flights; i++){
		flight iteration_flight = db->flights[i];

		if (!strcmp(iteration_flight.id, new_flight->id) && 
		iteration_flight.date.day == new_flight->date.day && 
		iteration_flight.date.month == new_flight->date.month && 
		iteration_flight.date.year == new_flight->date.year)
			return 0;
	}
	
	return 1;
}

/**
 * \brief Function that checks if a duration is valid or not
 * \param duration duration to be checked
 * \return 1 if duration is valid, 0 otherwise
 */
int is_valid_duration(time *duration){
	if (duration->hours > 12 || (duration->hours == 12 && duration->minutes > 0))
		return 0;
	
	return 1;
}

/**
 * \brief Function to insert a flight in the system database 
 * \param *db system database pointer
 * \param new_flight flight to be inserted
 * \param origin_airport_index index of the system database where the flight 
 should be inserted in
 * \return void
 */
void insert_flight(database *db, flight *new_flight, int origin_airport_index){
	db->flights[db->n_flights] = *new_flight;
	db->n_flights++;
	
	db->airports[origin_airport_index].n_flights++;
}

/**
 * \brief Function to print the output of a successfully added flight
 * \param flight flight to be printed
 * \return void
 */
void print_flight(flight *flight){
	printf(LIST_FLIGHT, flight->id, flight->origin_id, 
	flight->destination_id, flight->date.day, 
	flight->date.month, flight->date.year, 
	flight->time.hours, flight->time.minutes);
}

/**
 * \brief Function to both filter and sort the flights that have the requested 
 airport id origin (type = 0) or destination (type = 1)
 * \param flights list of flights to be filtered and sorted 
 * \param n_flights size of the the flights list 
 * \param airport_id requested airport id that need to be in the (destination 
 or origin depending on the type)
 * \param sorted_flights list where the sorted and filtered flights will be stored
 * \param type 0 if we want the flights with the requested airport as origin, 
 1 if want the destination otherwise
 * \return number of flights found
 */
int filter_sort_flights(flight flights[], int n_flights, char airport_id[], flight sorted_flights[], int type){
	int i, j, passed_filter, found_flights = 0;
	
	for (i = 0; i < n_flights; i++){    
    flight iteration_flight = flights[i];
    passed_filter = 0;
  
    /* for destination flights */
    if (type == 1){
      /* update the destination date */
      update_destination_info(&iteration_flight);
      
      passed_filter = !strcmp(iteration_flight.destination_id, airport_id);
    }
    else{
      passed_filter = !strcmp(iteration_flight.origin_id, airport_id);
    }
    
    if (passed_filter){
      /* insert ordenated */
      j = found_flights - 1;
      
      while (j>=0 && 
      date_time_to_days(&sorted_flights[j].date, &sorted_flights[j].time) > 
      date_time_to_days(&iteration_flight.date, &iteration_flight.time)){
        sorted_flights[j+1] = sorted_flights[j];
        j--;
      }
      
      sorted_flights[j+1] = iteration_flight;
      found_flights++;
		}
  }
  
  return found_flights;
}

/**
 * \brief Function to print the sorted flights output one by one
 * \param sorted_flights flights to be printed
 * \param n_print_flights size of the sorted_flights list
 * \return void
 */
void print_sorted_flights(flight sorted_flights[], int n_print_flights, int type){
	int i;
	
	for (i = 0; i < n_print_flights; i++){
		flight iteration_flight = sorted_flights[i];
	
		/* for destination */
		if (type)
			printf(LIST_FLIGHT_AIRPORT, iteration_flight.id, 
			iteration_flight.origin_id, iteration_flight.date.day, 
			iteration_flight.date.month, iteration_flight.date.year, 
			iteration_flight.time.hours, iteration_flight.time.minutes);
		else
			printf(LIST_FLIGHT_AIRPORT, iteration_flight.id, 
			iteration_flight.destination_id, iteration_flight.date.day, 
			iteration_flight.date.month, iteration_flight.date.year, 
			iteration_flight.time.hours, iteration_flight.time.minutes);
	}
}






















/* ========================================= */
/*                2 PROJETO                  */
/* ========================================= */

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

/**
 * \brief Function that checks if a reservation_id is valid or not
 * \param *db system database pointer
 * \param id id to be checked
 * \return 1 if the the given flight_id is valid, 0 otherwise
 */
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


/**
 * \brief Function that receives an id and search for it in the database flights,
 returns -1 if the id was not found, the index of that id in the database otherwise
 * \param *db system database pointer
 * \param id id to be searched
 * \return -1 if the airport_id was not found, the index in the database airports
 otherwise
 */
int found_flight_id(database *db, char id[MAX_FLIGHT_ID_SIZE+1], date flight_date){
	int i;

	for (i=0; i<db->n_flights; i++){
			flight iteration_flight = db->flights[i];

			if (!strcmp(iteration_flight.id, id) && 
			date_to_days(&flight_date) == date_to_days(&iteration_flight.date))
				return i;
	}
	return -1;
}


/* LINKED LISTS */

list* init_list(database *db){
	list* new_list = (list*)  controlled_malloc(db, sizeof(list));
	new_list->head = NULL;
	
	return new_list;
}

/**
 * @brief The idea is to insert the node in the linked list in the right position
 * 
 * @param reservations 
 * @param new_node 
 */
void insert_node(list *linked_list, node *new_node){	
	new_node->next = NULL;
	new_node->previous = NULL;

	/* if our list is empty we just insert it*/	
	if (!linked_list->head){
		linked_list->head = new_node;
		return;
	}
	
	/* let's find the sweet spot */
	
	/* check if it is the first elemet */
	if (strcmp(linked_list->head->reservation->reservation_id, new_node->reservation->reservation_id) > 0){
		/* insert in the first position */
		new_node->next = linked_list->head;
		linked_list->head = new_node;
	}
	else{
		node* current_node;
		
		for(current_node = linked_list->head; current_node->next; current_node = current_node->next){
			if (strcmp(current_node->next->reservation->reservation_id, new_node->reservation->reservation_id) > 0){
				break;
			}
		}

		/* otherwise insert it in between */
		new_node->next = current_node->next;
		new_node->previous = current_node;
		current_node->next = new_node;
	}		
}

void delete_node(list *linked_list, node *_node){
	if (!_node->previous && !_node->next)
		linked_list->head = NULL;
	else if (!_node->previous)
		linked_list->head = _node->next;
	else if (!_node->next)
		_node->previous->next = NULL;
	else {
		_node->previous->next = _node->next;
		_node->next->previous = _node->previous;
	}
	
	free_node(_node);
}

void delete_list(list *linked_list){
	node* current_node = linked_list->head;
	node* next_node;
	
	while (current_node){
		/* back it up*/
		next_node = current_node->next;
		
		free_node(current_node);
		current_node = next_node;
	}

	free(linked_list);
}

void free_node(node* node_to_free){
	free(node_to_free->reservation->reservation_id);
	free(node_to_free->reservation);
	free(node_to_free);
}

/* HASH TABLE */
hashtable* init_hashtable(database *db, int size){
	int i;
	hashtable* new_hashtable = (hashtable*) controlled_malloc(db, sizeof(hashtable));
	
	node** table = (node**) controlled_malloc(db, sizeof(node*) * size);
	
	new_hashtable->table = table;
	new_hashtable->count = 0;
	new_hashtable->size = size;
	
	/* set every hash as NULL */
	for (i=0; i<size; i++){
		table[i] = NULL;
	}
	
	return new_hashtable;
}

int hash_string(char* v, int size){
	long int hash, a = 31415, b = 27183;

	for (hash = 0; *v != '\0'; v++) {
		hash = (a * hash + *v) % size;
		a = a * b % (size - 1);
	}
	
	return hash;
}

hashtable* insert_table(hashtable* _hashtable, node* _node){
	int i = hash_string(_node->reservation->reservation_id, _hashtable->size);
	
	while (_hashtable->table[i] != NULL) 
		i = (i + 1) % _hashtable->size; 
		
	_hashtable->table[i] = _node;
	
	return _hashtable;
}

void delete_table(hashtable* _hashtable, node* _node){
	int j, i = hash_string(_node->reservation->reservation_id, _hashtable->size);
	node* tmp_node;
	
	while (_hashtable->table[i]) {
		if (_hashtable->table[i] == _node)
			break;
		else
			i = (i + 1) % _hashtable->size;
	}
	
	if (!_hashtable->table[i]) 
		return;
	
	_hashtable->table[i] = NULL;
	_hashtable->count--;

	for (j = (i + 1) % _hashtable->size; _hashtable->table[j]; 
	j = (j + 1) % _hashtable->size) {
	
		tmp_node = _hashtable->table[j];
		_hashtable->table[j] = NULL;
		
		/* compensate the cout increment on insert table function */
		_hashtable->count--;
		insert_table(_hashtable, tmp_node);
	}
}

void* search_table(hashtable* _hashtable, char reservation_id[]){
	int i = hash_string(reservation_id, _hashtable->size);
	
	while (_hashtable->table[i] != NULL){

		if (!strcmp(_hashtable->table[i]->reservation->reservation_id, reservation_id)){
			return _hashtable->table[i];
		}
		else{
			i = (i + 1) % _hashtable->size;
		}
	}
	
	return NULL;
}


void delete_hashtable(hashtable* hashtable_to_delete){
	/* delete hashtable doesnt free nodes */
	free(hashtable_to_delete->table);
	free(hashtable_to_delete);
}

void free_all(database *db){
	int i;
	flight *iteration_flight;
	
	/* free hashtable */
	delete_hashtable(db->reservations_hashtable);
	
	/* free linked lists of flights */
	for (i=0; i<db->n_flights; i++){
		iteration_flight = &db->flights[i];
	
		/* free list */
		delete_list(iteration_flight->reservations);	
	}
}