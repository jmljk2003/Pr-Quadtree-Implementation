#include "data.h"
#include "array.h"

#define MAX_INPUT 512
#define MAXLEN 128
#define MAXCHILD 4
#define ORI_SIZE 2

typedef struct rect rect_t;
typedef struct node qtree_t;

struct node{
    datapoint_t* datapoint;
    qtree_t* child[4];
    char color; //w, b, g
    array_t* more_datapoint;
};

struct rect{
    point_t bot_left;
    point_t top_right;
    point_t center;
};

void make_tree(qtree_t* tree, rect_t initial_rect, FILE* inFile, FILE* outFile);

qtree_t* create_tree();

qtree_t* creatNode(datapoint_t* point);

int find_quad(point_t* point, rect_t rect);

rect_t find_rect(int quad, rect_t rect_initial);

qtree_t* add_point(qtree_t* tree, datapoint_t* point, rect_t rect2D);

qtree_t* query_search(qtree_t* tree, point_t query_point, rect_t rect2D, FILE* out);

void print_record(data_t* data, FILE* out);

void query_range(rect_t rect, qtree_t* tree, FILE* out);

void free_tree(qtree_t* tree);
