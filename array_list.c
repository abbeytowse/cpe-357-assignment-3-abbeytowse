#include "array_list.h" 
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <dirent.h> 
#include <unistd.h>
#define PATH_SIZE 256 
#define CAPACITY 5

void *checked_malloc(size_t size) {
    int *p; 

    p = malloc(sizeof(char*) * size); 
    if (p == NULL) {
	perror("malloc"); 
	exit(1); 
    } 

    return p; 
}

struct array_list array_list_new() {
    struct array_list al; 

    al.length = 0; 
    al.capacity = CAPACITY;
    al.array = checked_malloc(CAPACITY); 

    return al;     
}

struct array_list array_list_add_to_end(struct array_list al, char *string) {
    if (al.capacity == al.length) { 
        al.array = (char**)realloc(al.array, sizeof(char**) * (al.length+CAPACITY)); 
        al.capacity += CAPACITY; 
    }

    strcat(string, "\0"); 
    al.array[al.length] = string;   
    al.length += 1; 
    return al; 
}

void sort_alpha(int length, struct array_list al) {
    int i;
    int j;
    char temp[PATH_SIZE];

    for (i = 0; i < length; i++) {
       for (j = i + 1; j < length; j++) {
	    if (strcasecmp(al.array[i], al.array[j]) > 0) {
                 strcpy(temp, al.array[i]);
		 strcpy(al.array[i], al.array[j]);
		 strcpy(al.array[j], temp);
            }
        }
    }
}

struct array_list put_argv_in_al(int argc, char *argv[], int val, int sw) {
    struct array_list al_argv = array_list_new();
    struct dirent *contents; 
    DIR *dir; 
    char *name;

    if (val == 1) {
        for (int i = 0; i < argc; i++) {
            al_argv = array_list_add_to_end(al_argv, argv[i]); 
        }

    } else {
        if ((dir = opendir(".")) == NULL) {
           fprintf(stderr, "ERROR: couldn't open directory"); 
	   exit(1);
        
	} 

	while ((contents = readdir(dir)) != NULL) { 
	    name = contents->d_name;
	    if ((strcmp(name, ".")) & (strcmp(name, ".."))) { 
                if ((sw == 1) | (sw == 3)) { 
		    al_argv = array_list_add_to_end(al_argv, name); 
	        } else {
		    if (strncmp(name, ".", 1)) {
	                al_argv = array_list_add_to_end(al_argv, name); 
		    }
	        }
	    } 
        }

	sort_alpha(al_argv.length, al_argv);

        closedir(dir); 
        free(contents); 	
    }
 
    return al_argv; 
}

void free_array_list(struct array_list al) {
    free(al.array); 
} 
