#ifndef __OUTPUTS_H__
#define __OUTPUTS_H__

/* a command */
#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define TOO_MANY_AIRPORTS "too many airports\n"
#define DUPLICATE_AIRPORTS "duplicate airport\n"
#define ADDED_AIRPORT "airport %s\n"

/* l command*/
#define NO_SUCH_AIRPORT "%s: no such airport ID\n"
#define LIST_AIRPORT "%s %s %s %d\n"

/* v command */
#define LIST_FLIGHT "%s %s %s %02d-%02d-%d %02d:%02d\n"
#define LIST_FLIGHT_AIRPORT "%s %s %02d-%02d-%d %02d:%02d\n"
#define INVALID_FLIGHT_CODE "invalid flight code\n"
#define FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define TOO_MANY_FLIGHTS "too many flights\n"
#define INVALID_DATE "invalid date\n"
#define INVALID_DURATION "invalid duration\n"
#define INVALID_CAPACITY "invalid capacity\n"

/* t command */
#define PRINT_DATE "%02d-%02d-%d\n"
#define INVALID_DATE "invalid date\n"

/* 2 PROJETO */

#define NO_MEMORY "No memory.\n"
#define NO_MEMORY_EXIT_CODE -1

#define INVALID_RESERVATION_CODE "invalid reservation code\n"
#define FLIGHT_DOESNT_EXIST "%s: flight does not exist\n"
#define PRINT_RESERVATIONS "%s %d\n"
#define FLIGHT_RESERVATION_ALREADY_IN_USE "%s: flight reservation already used\n"
#define TOO_MANY_RESERVATIONS "too many reservations\n"
#define INVALID_PASSENGER_NUMBER "invalid passenger number\n"
#define NOT_FOUND "not found\n"

#endif