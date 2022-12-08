#ifndef __VACCINE_H__
#define __VACCINE_H__
#include "csv.h"

typedef struct _tDate {    
    int day; 
    int month;
    int year;
} tDate;

typedef struct _tTime {
    int hour; 
    int minutes;
} tTime;

typedef struct _tDateTime {
    tDate date;
    tTime time;    
} tDateTime;


// Parse a tDateTime from string information
void dateTime_parse(tDateTime* dateTime, const char* date, const char* time);

// Compare two tDateTime structures and return true if they contain the same value or false otherwise.
bool dateTime_equals(tDateTime dateTime1, tDateTime dateTime2);

// Ex 1: Define data types here...

// Maximum length of vaccine name
#define MAX_VACCINE_NAME_LEN 15+1
// Maximum length of postal code
#define MAX_CP_LEN 5+1
// Maximum length of vaccine lotes
#define MAX_LOTES 100

typedef struct _tVaccineLot {
    tDateTime dateTime;
	char CP[MAX_CP_LEN];
	char vaccine[MAX_VACCINE_NAME_LEN];
	int required;
	int days;
	int doses;
} tVaccineLot;

typedef struct _tVaccineLotData {
	tVaccineLot elems[MAX_LOTES];
	int numLots;
}tVaccineLotData;

// Ex 2: Define your methods here ....

// Initialize the vaccine lots data
void vaccineData_init(tVaccineLotData* data);

// Get the number of lots
int vaccineData_len(tVaccineLotData data);

// Get a lot of vaccines
void vaccineData_get(tVaccineLotData data, int index, char* buffer);

// Parse input from CSVEntry
void vaccine_parse(tVaccineLot* data, tCSVEntry entry);

// Add a lot to the VaccineLotData
void vaccineData_add(tVaccineLotData* data, tVaccineLot lot);

// Erase an elements from VaccineLotData
void vaccineData_del(tVaccineLotData* data, char* CP, char* vaccine,tDateTime dateTime, int doses);
#endif
