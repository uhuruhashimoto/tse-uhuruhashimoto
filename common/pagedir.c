/*
* pagedir - checks directory existence/permissions by opening file to write
*
* contains other modules related to file-reading/writing/identification
* Uhuru Hashimoto CS50 21X
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// opens a file "crawler" in the specified directory 
// to check its existence and write permissions
bool checkDir(char *dirname) {
    //create a string "dirname/crawler"
    char *cr = "crawler";
    char *filename = filenameCreator(dirname, cr);

    //open file and return result
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        free(filename);
        return false;
    }
    else {
        free(filename);
        return true;
    }
}

// allocates and creates a string "dirname/filename"
// from a specified dirname and filename (assumed null-terminated) 
// It is left to the user to later free the returned result
char *filenameCreator(char *dirname, char *filename) {
    //allocation size is strlen(dirname) + strlen(filename) + / + null terminator
    char *ret = NULL;
    ret = malloc(sizeof(char)*(strlen(dirname)+strlen(filename)+2));
    if (ret == NULL) return NULL;
    strcpy(ret, dirname);
    strcat(ret, '/');
    strcat(ret, filename); //strcat adds null terminator by default
    return ret;
}

// allocates a string for a specified int (including null terminator) without using math lib
// Ex. 123 -> "123\0" 
// It is left to the user to later free the returned result
// Grateful credit to https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
// for advice on using snprintf to convert ints
char *intToString(int num) {
    //length of string is log10(num) + null terminator
    int strlen = snprintf(NULL, 0, "%d", num);
    strlen++; //null terminator

    char *strnum = NULL;
    strnum = malloc(sizeof(char)*strlen);
    if (strnum == NULL) return NULL;
    snprintf(strnum, strlen, "%d", num);
    return strnum;
}
