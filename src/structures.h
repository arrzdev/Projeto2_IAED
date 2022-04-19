#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "constants.h"

/* structures */
typedef struct{
  int day;
  int month;
  int year;
} date;

typedef struct{
  int hours;
  int minutes;
} time;

typedef struct{
  char id[MAX_AIRPORT_ID_SIZE+1];
  char country[MAX_COUNTRY_SIZE+1];
  char city[MAX_CITY_SIZE+1];
  int n_flights;
} airport;

typedef struct {
  struct Flight *reservation_flight;
  date date;
  char *reservation_id;
  int n_passengers;
} reservation;

typedef struct Node{
  struct Node *next;
  struct Node *previous;
  struct Node *next_hash;
  struct Node *previous_hash;
  reservation *reservation;
} node;

typedef struct{
  node *head;
  node *tail;
} list;

typedef struct Flight{
  char id[MAX_FLIGHT_ID_SIZE+1];
  char destination_id[MAX_AIRPORT_ID_SIZE+1];
  char origin_id[MAX_AIRPORT_ID_SIZE+1];
  date date;
  time time;
  time duration;
  int capacity;
  int left_capacity;
  list *reservations;
  int n_reservations;
} flight;

/* hash table */
typedef struct {
  node **table;
  int size;
} hashtable;

/* 
Global structure
*/
typedef struct{
  int n_airports;
  int n_flights;
  airport airports[MAX_AIRPORTS];
  flight flights[MAX_FLIGHTS];
  date date;
  hashtable *reservations_hashtable;
  hashtable *flights_hashtable;
} database;

#endif