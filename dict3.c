#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "qtree.h"

#define MAX_INPUT 512
#define MAXLEN 128
#define MAXCHILD 4
#define ORI_SIZE 2


int main(int argc, char* argv[]){
    
    qtree_t* tree = create_tree();
    
    FILE *inFile = fopen(argv[2], "r");

    FILE *outFile = fopen(argv[3], "w");

    rect_t rect2D;

    rect2D.bot_left.lon = strtold(argv[4], NULL);

    rect2D.bot_left.lat = strtold(argv[5], NULL);

    rect2D.top_right.lon = strtold(argv[6], NULL);

    rect2D.top_right.lat = strtold(argv[7], NULL);

    rect2D.center.lon = (rect2D.top_right.lon + rect2D.bot_left.lon)/2;

    rect2D.center.lat = (rect2D.top_right.lat + rect2D.bot_left.lat)/2;

    make_tree(tree, rect2D, inFile, outFile);

    query_range(rect2D, tree, outFile);

    //free_tree(tree);

    fclose(inFile);

    fclose(outFile);

    return 0;
}