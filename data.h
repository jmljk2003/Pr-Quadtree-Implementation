#define MAX_INPUT 512
#define MAXLEN 128
#define MAXCHILD 4
#define ORI_SIZE 2

int empty_field(int count, char* input);

int case_quotation(int count, char* input);

int str_scan(char* src, char* location, int count);

int double_scan(char* src, double* location, int count);

int int_scan(char* src, int* location, int count);