#ifndef TREE_H
#define TRR_H
#include "array_list.h" 
#include <sys/stat.h> 

int validate_args (int agrc, char *argv[]); 

int get_switches (int argc, char *argv[]); 

void get_files_dirs(int argc, struct array_list al, int sw, int iteration, int val);

void list_file(char *arg, struct stat stats, int iteration, int sw);

void list_dir(char *arg, struct stat stats, int iteration, struct array_list al, int sw, char file_path[]); 

#endif
