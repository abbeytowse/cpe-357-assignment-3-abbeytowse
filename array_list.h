#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stddef.h> 

struct array_list {
    int length; 
    int capacity; 
    char **array; 
};

void *checked_malloc(size_t size); 

struct array_list array_list_new(); 

struct array_list array_list_add_to_end(struct array_list al, char *string); 

struct array_list put_argv_in_al(int argc, char *argv[], int val, int sw);

void sort_alpha(int length, struct array_list al); 

void free_array_list(struct array_list al); 

#endif
