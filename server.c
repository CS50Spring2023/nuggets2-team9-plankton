//
//
//
//


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
# include "libs/file.h"


int
main(const int argc, char* argv[])
{
    return(0);
}

char*
load_map(FILE* fp)
{
    if (fp == NULL){
        fprintf(stderr, "Error. NULL file pointer passed to load_map.\n");
        exit(1);
    }

    char* map = file_readFile(fp);

    if (map == NULL){
        fprintf(stderr, "Error loading map into string in load_map.\n");
        exit(1);
    }

    return map;

}