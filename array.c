#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "array.h"

array_t *make_array(){
    array_t *arr = malloc(sizeof(*arr));
    int temp_size = ORI_SIZE;
    arr->size = temp_size;
    arr->multi_datapoint= malloc(temp_size * sizeof(*(arr->multi_datapoint)));
    arr->n = 0;
    return arr;
};

void arrayAppend(array_t *arr, datapoint_t* data){
	if(arr->size == arr->n){
		arr->size *= 2;
		arr->multi_datapoint = realloc(arr->multi_datapoint, arr->size * sizeof(*(arr->multi_datapoint)));
	}
    if (arr->n < arr->size){
		arr->multi_datapoint[arr->n] = data;
		(arr->n)++;
	}
}

void array_shrink(array_t *arr){
    if (arr->size != arr->n) {
		arr->size = (arr->n > ORI_SIZE) ? arr->n : ORI_SIZE;
		arr->multi_datapoint = realloc(arr->multi_datapoint, arr->size * sizeof(*(arr->multi_datapoint)));
		assert(arr->multi_datapoint);
	}
}

void array_sort(array_t* arr){
    for(int i = 0; i < arr->n; i++){
        for(int j = i + 1; j < arr->n; j++){
            if(arr->multi_datapoint[i]->record->footpath_id > arr->multi_datapoint[j]->record->footpath_id){
                datapoint_t* temp = arr->multi_datapoint[i];
                arr->multi_datapoint[i] = arr->multi_datapoint[j];
                arr->multi_datapoint[j] = temp;
            }
        }
    }
}

void array_free(array_t* arr){
    for(int i = 0; i < arr->n; i++){
        printf("Run2\n");
        free(arr->multi_datapoint[i]->point);
        free(arr->multi_datapoint[i]->record);
    }
    free(arr->multi_datapoint);
    free(arr);
}