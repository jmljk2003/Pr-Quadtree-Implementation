#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

//Function used to identify if the input is empty
int empty_field(int count, char* input){
    if (input[count] == ','){
        return 1;
    }
    return 0;
}

//Function used to identify if the input is delimited by a quotation mark
int case_quotation(int count, char* input){
    if(input[count] == '\"'){
        return 1;
    }
    return 0;
}

//Function is used to scan in fields of string type
int str_scan(char* src, char* location, int count){
    int char_q;
    if (empty_field(count, src)){       //Case: When field is empty
            strcpy(location, "");
            char_q = 0;
            char_q++;
        }
        //Case: When field is delimited by quotation marks
        else if (case_quotation(count, src)){   
            count += 1;
            sscanf(src + count, "%[^\"]%n,", location, &char_q);
            char_q += 3;
        }
        else{       //Case: When the field input is normal
            sscanf(src + count, "%[^,],%n", location, &char_q);
        }
    return char_q;  //return number of characters scanned
}

//Function is used to scan in fields of type double
int double_scan(char* src, double* location, int count){
    int char_q;
    sscanf(src + count, "%128lf,%n", location, &char_q);
    return char_q;
}

//Function is used to scan in fields of type int
int int_scan(char* src, int* location, int count){
    int char_q;
    double temp_store;
    sscanf(src + count, "%lf,%n", &temp_store, &char_q);
    *location = (int) temp_store;
    return char_q;
}