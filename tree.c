#include "tree.h"
#include "array_list.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h> 
#define PATH_SIZE 256 // how can I not have a limiting size?  

int validate_args (int argc, char *argv[]) {
    if (argc < 2) {
	return 0;  	
    } else if (argc == 2) {
        if ((!strcmp(argv[1], "-a") | !strcmp(argv[1], "-s"))) {
            return 0; 
	}
    } else if (argc == 3) {
        if  ((!strcmp(argv[1], "-a") | !strcmp(argv[1], "-s")) & (!strcmp(argv[2], "-a") | !strcmp(argv[2], "-s"))) {
            return 0;
	} 
    } 

    return 1; 
}

int get_switches (int argc, char *argv[]) { 
     int sw = 0;  
     char *p1;
     char *cmpa = "-a";
     char *cmps = "-s";  

    for (int i = 1; i < argc; i++) { 
        p1 = argv[i]; 

       	if (!strcmp(p1, cmpa)) { 
	    if (sw == 2) { 
	        sw = 3; 
	    } else {
                sw = 1;
	    }

	} else if (!strcmp(p1, cmps)){ 
            if (sw == 1) {
	        sw = 3; 
	    } else {
                sw = 2; 
	    }
	} 
    }

    return sw; 
}

void list_dir(char *arg, struct stat stats, int iteration, struct array_list al, int sw, char file_path[]) { 
    struct dirent *contents;
    DIR *dir;
    char *name;   

    if (iteration > 1) { 
        for (int i = 1; i < iteration/2; i++) {
            fprintf(stdout, "|   ");     
        } 
	if ((sw == 2) | (sw == 3)) {
	    fprintf(stdout, "|-- %s [size: %li]\n", arg, stats.st_size); 
	} else { 
	    fprintf(stdout, "|-- %s\n", arg);
	} 
    } else {
        if ((sw == 2) |	(sw == 3)) {
            fprintf(stdout, "%s [size: %li]\n", arg, stats.st_size); 
        } else { 
            fprintf(stdout, "%s\n", arg); 
        }
    } 

   if (chdir(file_path)) {
       fprintf(stderr, "ERROR: could not change directory\n"); 
       return; 
   }  

   if ((dir = opendir(".")) == NULL) {
       fprintf(stderr, "ERROR: couldn't open directory"); 
       return; 
   }

   int i = 0; 
   while ((contents = readdir(dir)) != NULL) {
	name = contents->d_name; 	
	if ((strcmp(name, ".")) & (strcmp(name, ".."))) { 
	    if ((sw == 1) | (sw == 3)) {	
                al = array_list_add_to_end(al, name);
	    } else {
		if (strncmp(name, ".", 1)) {
	            al = array_list_add_to_end(al, name);
	        } 
	    }		
            i++; 	    
	} 
   }

   free(contents); 
   sort_alpha(al.length, al); 
   iteration += 1; 
   get_files_dirs(al.length, al, sw, iteration, 1); 
   closedir(dir);
}

void list_file(char *arg, struct stat stats, int iteration, int sw) {
    if (iteration > 1) {
	for (int i = 1; i < iteration/2; i++) {
	    fprintf(stdout, "|   "); 
        }
        if ((sw == 2) | (sw == 3)) {
	   fprintf(stdout, "|-- %s [size: %li]\n", arg, stats.st_size); 
	} else {	
            fprintf(stdout, "|-- %s\n", arg); 
	}
    } else {
	if ((sw == 2) | (sw == 3)) {
	   fprintf(stdout, "%s [size: %li]\n", arg, stats.st_size); 
	} else {
	    fprintf(stdout, "%s\n", arg);	
	}
    } 
} 

void get_files_dirs(int length, struct array_list al_argv, int sw, int iteration, int val) {
    int idx = 0; 
    struct array_list al = array_list_new(); 
    struct stat stats; 
    int file = 0; 
    int dir = 0;   
    char path[PATH_SIZE];
    char file_path[PATH_SIZE]; 

    if (iteration == 0) {
        if (sw == 0) { // no flags given 
            idx = 1; 
        } else if ((sw == 1) | (sw == 2)) { // 1 flag given 
	    idx = 2; 
        } else { // 2 flags given 
            idx = 3; 
        }
    } 	

    getcwd(path, sizeof(path));  
    for (int i = idx; i < length; i++) { 
	strcpy(file_path, path);
        strcat(file_path, "/");  	
	strcat(file_path, al_argv.array[i]);	
        stat(file_path, &stats);
	file = S_ISREG(stats.st_mode);	
        dir = S_ISDIR(stats.st_mode);	
        if (dir == 1) {
	    iteration += 1; 
	    list_dir(al_argv.array[i], stats, iteration, al, sw, file_path);
	    iteration -= 1; 
        } else if (file == 1) {	
            list_file(al_argv.array[i], stats, iteration, sw);  
        } else {
            fprintf(stderr, "Invalid arg given on command line"); 
	} 
    } 

    free_array_list(al);   
}

int main (int argc, char *argv[]) { 
    int sw = 0;
    struct array_list arg_al; 
    int iteration = 0; 
    int val = 0;

    val = validate_args(argc, argv);  
    sw = get_switches(argc, argv); // sw=1: -a, sw=2: -s, sw=3: -a -s, sw=0: no flags  
    arg_al = put_argv_in_al(argc, argv, val, sw); 
    get_files_dirs(arg_al.length, arg_al, sw, iteration, val);
    free_array_list(arg_al);  

    return 0; 
}
