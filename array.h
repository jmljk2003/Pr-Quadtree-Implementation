#include "data.h"

#define MAXLEN 128
#define MAX_INPUT 512

typedef struct info_t data_t;
typedef struct array array_t;
typedef struct datapoint datapoint_t;
typedef struct point point_t;

struct info_t{
    int footpath_id;
    char address[MAXLEN + 1];
    char clue_sa[MAXLEN + 1];
    char asset_type[MAXLEN + 1];
    double deltaz;
    double distance;
    double gradelin;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char segside[MAXLEN + 1];
    int statusid;
    int streetid;
    int street_grp;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};

struct array{
    datapoint_t **multi_datapoint;
    int size;
    int n;
};

struct datapoint{
    point_t* point;
    data_t* record;
};

struct point{
   long double lat;
   long double lon;
};

array_t *make_array();

void arrayAppend(array_t *arr, datapoint_t* data);

void array_shrink(array_t *arr);

void array_sort(array_t* arr);

void array_free(array_t* arr);
