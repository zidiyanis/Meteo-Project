#include<math.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include<stdbool.h>

#include <time.h>

#include <limits.h>

#include<math.h>

typedef struct {

    int id;
    char * date;
    double data;

    double x;
    double y;

    double d_x;
    double d_y;

    double min;
    double max;

    int nmb_data;
    double sum;
    double average;

    int nmb_data_1;
    double sum_1;
    double average_1;

    int tab_incr;
    int pass;

}
Data_Set;

typedef struct Tree Tree;
struct Tree {
    Data_Set * element;
    Tree * left_son;
    Tree * right_son;
    int equilibre;
    int height;
};

typedef Tree * p_tree;

Data_Set * create_set();

Data_Set * create_data_set(int id, char * date, double data, double d_x, double d_y, double min, double max, double x, double y);

void print_data_set_tab(Data_Set ** tab_data_set, int size, int r, FILE * output_file);

int compare_max(const void * a,
    const void * b);

int compare_id(const void * a,
    const void * b);

p_tree create_tree(Data_Set * element);

bool is_leaf(p_tree t);

bool has_left_son(p_tree t);

bool has_right_son(p_tree t);

Data_Set * get_element(p_tree t);

int get_height(p_tree t);

int nmb_nodes(p_tree t);

void in_fixe_search(p_tree t, FILE * file, int mode, int * c);

void reverse_in_fixe_search(p_tree t, FILE * file, int mode);

bool search_by_id(p_tree t, Data_Set * element);

bool search_by_date(p_tree t, Data_Set * element, int * nmb_knots);

bool binary_search_tree_date(p_tree array[], int size, char * value, int * index);

//********************************************************************/
//***********************   BINARY SEARCH TREE   *********************//
//********************************************************************//

p_tree insert_ABR_by_ID(p_tree t, Data_Set * element, int mode);

p_tree insert_ABR_by_date(p_tree t, Data_Set * element);

p_tree insert_ABR_by_max(p_tree t, Data_Set * element);

void insert_ABR_by_max_from_id(p_tree t1, p_tree * t2);

void print_ABR(p_tree t, int space);

//********************************************************************/
//*******************************   AVL   ****************************//
//********************************************************************//

int get_balance_factor(p_tree t);

p_tree left_rotation(p_tree t);

p_tree right_rotation(p_tree t);

p_tree balance_AVL(p_tree t);

p_tree double_left_rotation(p_tree t);

p_tree double_right_rotation(p_tree t);

// p_tree add_AVL(p_tree t , Data_Set* x, int *h);

p_tree insert_AVL_by_ID(p_tree t, Data_Set * element, int * h, int mode);

p_tree insert_AVL_by_date(p_tree t, Data_Set * element, int * h);

p_tree insert_AVL_by_max(p_tree t, Data_Set * element);

void insert_AVL_by_max_from_id(p_tree t1, p_tree * t2);

void print_AVL(p_tree t, int space);

int is_csv(char * s);

int compare_double(const void * a,
    const void * b);

int compare_int(const void * a,
    const void * b);

bool binary_search_date(char * array[], int size, char * value, int * index);

bool binary_search_data_set_ID(Data_Set * array[], int size, int value, int * index);

bool binary_search_data_set_date(Data_Set * array[], int size, char * value, int * index);

bool binary_search_tab_date(Data_Set ** array[], int size, char * value, int * index);
