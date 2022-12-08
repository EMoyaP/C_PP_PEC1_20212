#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "vaccine.h"

// Parse a tDateTime from string information
void dateTime_parse(tDateTime* dateTime, const char* date, const char* time){
    // Check output data
    assert(dateTime != NULL);
    
    // Check input date
    assert(date != NULL);
    assert(strlen(date) == 10);
    
    // Check input time
    assert(time != NULL);
    assert(strlen(time) == 5);
    
    // Parse the input date
    sscanf(date, "%d/%d/%d", &(dateTime->date.day), &(dateTime->date.month), &(dateTime->date.year));
    
    // Parse the input time
    sscanf(time, "%d:%d", &(dateTime->time.hour), &(dateTime->time.minutes));
}

// Compare two tDateTime structures and return true if they contain the same value or false otherwise.
bool dateTime_equals(tDateTime dateTime1, tDateTime dateTime2){
    if (dateTime1.date.day != dateTime2.date.day || dateTime1.date.month != dateTime2.date.month || dateTime1.date.year != dateTime2.date.year) {
        return false;
    }
    
    return dateTime1.time.hour == dateTime2.time.hour && dateTime1.time.minutes == dateTime2.time.minutes;
}

// EX2: Implement your methods here....

// Initialize the vaccine lots data
void vaccineData_init(tVaccineLotData* data){
    data->numLots = 0;
}


// Get the number of lots
int vaccineData_len(tVaccineLotData data){
    int lenLots;
	lenLots = 0;	
	
	lenLots = data.numLots;
	
	return lenLots;
}

// Get a lot of vaccines
void vaccineData_get(tVaccineLotData data, int index, char* buffer) {

	sprintf(buffer,"%.2d/%.2d/%d;%.2d:%.2d;%s;%s;%d;%d;%d",
		data.elems[index].dateTime.date.day,
		data.elems[index].dateTime.date.month,
		data.elems[index].dateTime.date.year,
		data.elems[index].dateTime.time.hour,
		data.elems[index].dateTime.time.minutes,
		data.elems[index].CP,
		data.elems[index].vaccine,
		data.elems[index].required,
		data.elems[index].days,
		data.elems[index].doses);
}

// Parse input from CSVEntry
void vaccine_parse(tVaccineLot* data, tCSVEntry entry) {
    
	char date[10+1];
	char time[5+1];
	
	// Pre-conditions
    assert(data != NULL);
    assert(csv_numFields(entry) == 7);
   
    //Parse the input date
	csv_getAsString (entry, 0, date, 11);
	csv_getAsString (entry, 1, time, 6);
	dateTime_parse (&(data->dateTime),date, time);
	
	csv_getAsString (entry, 2, data->CP, MAX_CP_LEN);
	csv_getAsString (entry, 3, data->vaccine, MAX_VACCINE_NAME_LEN);
	
	data->required = csv_getAsInteger(entry, 4);
	data->days = csv_getAsInteger (entry, 5);
	data->doses = csv_getAsInteger (entry, 6);
	
}

// Add a lot to the VaccineLotData
void vaccineData_add(tVaccineLotData* data, tVaccineLot lot) { 

	int i;
	i = 0;
	bool found;
	found = false;
	bool equals;
	equals = true;
	
    // Pre-conditions
    assert(data != NULL);    
    assert(data->numLots <= MAX_LOTES);
	
	// Add a lot into VaccineLotData
    while (i < data->numLots && found == false){
		
		if ((strcmp(data->elems[i].CP, lot.CP) == 0) &&
		(dateTime_equals(data->elems[i].dateTime, lot.dateTime) == equals) &&
		(strcmp(data->elems[i].vaccine, lot.vaccine)) == 0){ 
			//ADD doses to exist lot
			data->elems[i].doses += lot.doses;
			found = true;
		}
		i++;
	}
	
	if ((found == false) &&(data->numLots < MAX_LOTES)){
		//Add new lot
		data->numLots = data->numLots+1;
  		//Paste the new lot to datalotes
		data->elems[data->numLots-1].dateTime = lot.dateTime;
		strcpy(data->elems[data->numLots-1].CP, lot.CP);
		strcpy(data->elems[data->numLots-1].vaccine, lot.vaccine);
		data->elems[data->numLots-1].required = lot.required;
		data->elems[data->numLots-1].days = lot.days;
		data->elems[data->numLots-1].doses = lot.doses;
 	}

 }
	
// Erase an elements from VaccineLotData
void vaccineData_del(tVaccineLotData* data, char* CP, char* vaccine,tDateTime dateTime, int doses){

	int i;
	i = 0;
	int j;
	j = 0;
	bool found;
	found = false;
	int modified;
	modified = 0;
	bool equals;
	equals = true;
 
    while (i < data->numLots && found == false){

		if( (strcmp(data->elems[i].CP, CP) == 0) &&
			(strcmp(data->elems[i].vaccine, vaccine) == 0) &&
			(dateTime_equals(data->elems[i].dateTime, dateTime) == equals) ){
				//Erase doses from vaccine
				data->elems[i].doses=data->elems[i].doses - doses;
				modified = i;
				found = true;		
		}
		i++;
	}
	
	//Erase lot if doses are 0 or less
	if(data->elems[modified].doses <1){	

		for (j = modified; j < data->numLots-1; j++){
				
			data->elems[j].dateTime.date.day   = data->elems[j+1].dateTime.date.day;
			data->elems[j].dateTime.date.month = data->elems[j+1].dateTime.date.month;
			data->elems[j].dateTime.date.year  = data->elems[j+1].dateTime.date.year;
			data->elems[j].dateTime.time.hour  = data->elems[j+1].dateTime.time.hour;
			data->elems[j].dateTime.time.minutes = data->elems[j+1].dateTime.time.minutes;
			strcpy(data->elems[j].CP, data->elems[j+1].CP);
			strcpy(data->elems[j].vaccine, data->elems[j+1].vaccine);
			data->elems[j].required = data->elems[j+1].required;
			data->elems[j].days = data->elems[j+1].days;
			data->elems[j].doses = data->elems[j+1].doses;
		
			data->numLots=data->numLots-1;
		}
	}	
}
