#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "qtree.h"

qtree_t* create_tree(){
    qtree_t* tree;
    tree = (qtree_t*)malloc(sizeof(qtree_t));
    assert(tree != NULL);
    tree->color = 'g';
    tree->child[0] = NULL;
    tree->child[1] = NULL;
    tree->child[2] = NULL;
    tree->child[3] = NULL;
    return tree;
}

qtree_t* create_node(datapoint_t* point){
    qtree_t* branch;
    branch = malloc(sizeof(qtree_t));
    branch->datapoint = point;
    branch->more_datapoint = make_array();
    branch->color = 'b';
    branch->child[0] = NULL;
    branch->child[1] = NULL;
    branch->child[2] = NULL;
    branch->child[3] = NULL;
    return branch;
}

int find_quad(point_t* point, rect_t rect){
    /*if(point->lon == rect.center.lon && point->lat == rect.center.lat){
        return 1;
    }*/
    if(point->lon <= rect.center.lon){
        if(point->lat < rect.center.lat){
            return 0; //SW
        }
        else if(point->lat >= rect.center.lat){
        return 1; //NW
        }
    }
    else if(point->lon > rect.center.lon){
        if(point->lat >= rect.center.lat){
            return 2; //NE
        }
        else if(point->lat < rect.center.lat){
            return 3; //SE
        }
    }
    return 4; //Not in rect.
}

rect_t find_rect(int quad, rect_t rect_initial){
    rect_t temp;
    if (quad == 0){ //sw CORRECT
        temp.top_right = rect_initial.center;
        temp.bot_left = rect_initial.bot_left;
    }
    else if(quad == 1){ //nw
        temp.top_right.lat = rect_initial.top_right.lat;
        temp.top_right.lon = rect_initial.center.lon;
        temp.bot_left.lat = rect_initial.center.lat;
        temp.bot_left.lon = rect_initial.bot_left.lon;
    }
    else if(quad == 2){ //ne
        temp.top_right = rect_initial.top_right;
        temp.bot_left = rect_initial.center;
    }
    else if(quad == 3){ //se
        temp.top_right.lat = rect_initial.center.lat;
        temp.top_right.lon = rect_initial.top_right.lon;
        temp.bot_left.lat = rect_initial.bot_left.lat;
        temp.bot_left.lon = rect_initial.center.lon;
    }
    temp.center.lat = (temp.top_right.lat + temp.bot_left.lat)/2;
    temp.center.lon = (temp.top_right.lon + temp.bot_left.lon)/2;
    return temp;
}

qtree_t* add_point(qtree_t* tree, datapoint_t* point, rect_t rect2D){
    int quad;
    rect_t temp_rect;
    quad = find_quad(point->point, rect2D);
    temp_rect = find_rect(quad, rect2D);
    if(tree->color == 'b'){
        if((point->point->lat == tree->datapoint->point->lat) && (point->point->lon == tree->datapoint->point->lon)){
            arrayAppend(tree->more_datapoint, point);
            //printf("Point Inserted!\n");
        }
        else{
            quad = find_quad(tree->datapoint->point, rect2D);
            tree->child[quad] = create_node(tree->datapoint);
            tree->child[quad]->more_datapoint = tree->more_datapoint;
            tree->color = 'g';
        }
    }
    if (tree->color == 'g'){
        quad = find_quad(point->point, rect2D);
        if(tree->child[quad] == NULL){
            tree->child[quad] = create_node(point);
            arrayAppend(tree->child[quad]->more_datapoint, point);
        }
        else{
            tree->child[quad] = add_point(tree->child[quad], point, temp_rect);
        }
    }

    return tree;
}

qtree_t* query_search(qtree_t* tree, point_t query_point, rect_t rect2D, FILE* out){
    int quad;
    rect_t temp_rect;
    if(tree->color == 'b'){
        if(tree->more_datapoint->n > 1){
            array_sort(tree->more_datapoint);
            for(int i = 0; i < tree->more_datapoint->n; i++){
                print_record(tree->more_datapoint->multi_datapoint[i]->record, out);
            }
        }
        else {
            print_record(tree->datapoint->record, out);
        }
        return tree;
    }
    else if(tree->color == 'g'){
        quad = find_quad(&query_point, rect2D);
        if (quad == 0){
            printf("SW ");
        }
        else if(quad == 1){
            printf("NW ");
        }
        else if(quad == 2){
            printf("NE ");
        }
        else if(quad == 3){
            printf("SE ");
        }
        temp_rect = find_rect(quad, rect2D);
        return query_search(tree->child[quad], query_point, temp_rect, out);
    }
    printf("UH OH!\n");
    return NULL;
}


void print_record(data_t* data, FILE* out){
    fprintf(out,"--> footpath_id: %d || address: %s || "
            "clue_sa: %s || asset_type: %s || deltaz: %.2lf || "
            "distance: %.2lf || grade1in: %.1lf || mcc_id: %d || "
            "mccid_int: %d || rlmax: %.2lf || rlmin: %.2lf || "
            "segside: %s || statusid: %d || streetid: %d || "
            "street_group: %d || start_lat: %lf || start_lon: %lf || "
            "end_lat: %lf || end_lon: %lf || \n", data->footpath_id,
            data->address, data->clue_sa, data->asset_type, data->deltaz,
            data->distance,data->gradelin,data->mcc_id,data->mccid_int,
            data->rlmax,data->rlmin,data->segside,data->statusid,data->streetid,
            data->street_grp,data->start_lat,data->start_lon,data->end_lat,
            data->end_lon);
}

void make_tree(qtree_t* tree, rect_t initial_rect, FILE* inFile, FILE* outFile){

    char redundant[MAX_INPUT + 1];

    char input[MAX_INPUT + 1];

    fscanf(inFile, "%s\n", redundant);
    while(fgets(input, MAX_INPUT + 1, inFile)){
        data_t *temp = (data_t*)malloc(sizeof(data_t));
        assert(temp != NULL);
        datapoint_t* temp_datapoint = malloc(sizeof(datapoint_t));
        temp_datapoint->point = malloc(sizeof(point_t));
        datapoint_t* temp_datapoint2 = malloc(sizeof(datapoint_t));
        temp_datapoint2->point = malloc(sizeof(point_t));

        int true_count = 0;
        true_count += int_scan(input, &temp->footpath_id, true_count);
        true_count += str_scan(input, temp->address, true_count);
        true_count += str_scan(input, temp->clue_sa, true_count);
        true_count += str_scan(input, temp->asset_type, true_count);
        true_count += double_scan(input, &temp->deltaz, true_count);
        true_count += double_scan(input, &temp->distance, true_count);
        true_count += double_scan(input, &temp->gradelin, true_count);
        true_count += int_scan(input, &temp->mcc_id, true_count);
        true_count += int_scan(input, &temp->mccid_int, true_count);
        true_count += double_scan(input, &temp->rlmax, true_count);
        true_count += double_scan(input, &temp->rlmin, true_count);
        true_count += str_scan(input, temp->segside, true_count);
        true_count += int_scan(input, &temp->statusid, true_count);
        true_count += int_scan(input, &temp->streetid, true_count);
        true_count += int_scan(input, &temp->street_grp, true_count);
        true_count += double_scan(input, &temp->start_lat, true_count);
        true_count += double_scan(input, &temp->start_lon, true_count);
        true_count += double_scan(input, &temp->end_lat, true_count);
        true_count += double_scan(input, &temp->end_lon, true_count);
        
        temp_datapoint->record = temp;
        temp_datapoint->point->lat = temp_datapoint->record->start_lat;
        temp_datapoint->point->lon = temp_datapoint->record->start_lon;
        tree = add_point(tree, temp_datapoint, initial_rect);
        temp_datapoint2->record = temp;
        temp_datapoint2->point->lat = temp_datapoint->record->end_lat;
        temp_datapoint2->point->lon = temp_datapoint->record->end_lon;
        tree = add_point(tree, temp_datapoint2, initial_rect);
    }
}

void query_range(rect_t rect, qtree_t* tree, FILE* out){
    point_t query_point;
    char temp_query[MAXLEN + 1];
    while(fgets(temp_query, MAXLEN, stdin)){
        strtok(temp_query, "\n");
        fprintf(out,"%s\n",temp_query);
        printf("%s --> ", temp_query);
        sscanf(temp_query, "%LF %LF", &query_point.lon, &query_point.lat);
        query_search(tree, query_point, rect, out);
        printf("\n");
    }
}

void free_tree(qtree_t* tree){
    if (tree == NULL){
        return;
    }
    for(int i = 0; i<4; i++){
        printf("Run\n");
        free_tree(tree->child[i]);
        tree->child[i] = NULL;
    }
    array_free(tree->more_datapoint);
    free(tree);
}