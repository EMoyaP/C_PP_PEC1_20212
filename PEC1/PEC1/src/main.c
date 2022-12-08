#include <stdio.h>
#include "csv.h"
#include "vaccine.h"


void vaccineData_print(tVaccineLotData data) {
    char buffer[1024];
    int i;
    if (vaccineData_len(data) > 0) {
        for(i = 0 ; i < vaccineData_len(data) ; i++) {
            vaccineData_get(data, i, buffer);
            printf("%d;%s\n", i, buffer);
        }
    }    
}

int main(int argc, char **argv)
{    
    tCSVData test_data;      
    tVaccineLotData vaccineLotData;
    tVaccineLot vaccineLot;
    tDateTime dateTime;   
    int i;
    
    // Define test data
    const char* test_data_str = "01/01/2022;13:45;08000;PFIZER;2;21;300\n" \
                                "01/02/2022;11:00;08018;MODERNA;1;0;150\n" \
                                "01/03/2022;17:15;25001;PFIZER;2;21;100\n";
    const char* test_data2_str = "01/01/2022;13:45;08000;PFIZER;2;21;50\n" \
                                 "01/03/2022;17:15;25001;PFIZER;2;21;250\n" \
                                 "01/02/2022;11:00;08018;MODERNA;1;0;200\n";
                               
    // Read the data in CSV format
    csv_init(&test_data);
    csv_parse(&test_data, test_data_str, "VACCINE_LOT");
   
    // Initialize the geopositions data
    vaccineData_init(&vaccineLotData);
    
    // Add data
    for(i=0; i < csv_numEntries(test_data); i++) {
        // Initialize the position
        vaccine_parse(&vaccineLot, csv_getEntry(test_data, i)[0]);
        
        // Add new vaccine batch the list of vaccines
        vaccineData_add(&vaccineLotData, vaccineLot);
    }
    
    // List current lots
    vaccineData_print(vaccineLotData);
    printf("\n");
    
    // Remove old data
    csv_free(&test_data);
    
    // Load new data
    csv_init(&test_data);
    csv_parse(&test_data, test_data2_str, "VACCINE_LOT");
   
    // Add new data
    for(i=0; i < csv_numEntries(test_data); i++) {
        // Initialize the position
        vaccine_parse(&vaccineLot, csv_getEntry(test_data, i)[0]);
        
        // Add new vaccine batch the list of vaccines
        vaccineData_add(&vaccineLotData, vaccineLot);
    }
    
    // List current lots
    vaccineData_print(vaccineLotData);
    printf("\n");
    
    // Remove some doses
    dateTime_parse(&dateTime, "01/02/2022", "11:00");
    vaccineData_del(&vaccineLotData, "08018", "MODERNA", dateTime, 50);
    
    // List current lots
    vaccineData_print(vaccineLotData);
    printf("\n");
    
    // Remove remaining doses    
    vaccineData_del(&vaccineLotData, "08018", "MODERNA", dateTime, 300);
    
    // List current lots
    vaccineData_print(vaccineLotData);
    printf("\n");
    
    // Remove not existing registry (the same, as now it should not exist)
    vaccineData_del(&vaccineLotData, "08018", "MODERNA", dateTime, 1000);
    
    // List current lots
    vaccineData_print(vaccineLotData);
    printf("\n");
    
    // Remove CSV data
    csv_free(&test_data);   
    
    printf("\n\nPress key to end...\n");
    getchar();
	return 0;
}
