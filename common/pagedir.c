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
//libcs50 library modules
#include "../libcs50/webpage.h"

bool checkDir(char *dirname);
void pagesaver(webpage_t *webpage, int depth, char *dirname, int doc_id);
char *filenameCreator(char *dirname, char *filename);
char *intToString(int num);
int stringToInt(char *string);
bool isReadableFile(char *filename);
bool isCrawlerDirectory(char *dirname);

// opens a file "crawler" in the specified directory 
// to check its existence and write permissions
bool checkDir(char *dirname) {
    //create a string "dirname/crawler"
    char *cr = ".crawler";
    char *filename = filenameCreator(dirname, cr);

    //open file and return result
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        free(filename);
        return false;
    }
    else {
        fclose(fp);
        free(filename);
        return true;
    }
}

// assumes non-null webpage with fetched html content
// uses pagedir modules to create files and writes webpage data to them
void pagesaver(webpage_t *webpage, int depth, char *dirname, int doc_id) {
	// INIT STRUCTURES
	char *strnum = intToString(doc_id); //allocated
	char *filename = filenameCreator(dirname, strnum); //allocated
    FILE *fp = NULL;

#ifdef NOOVERWRITE
	// OVERWRITE CHECKS
	fp = fopen(filename, "r");
	if (fp != NULL) { // if file exists
		fprintf(stderr, "Error: file %s will be overwritten by save.\n", filename);
		fclose(fp);
		free(strnum);
		free(filename);
		return;
	}
#endif

	// WRITE DATA
	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error: file %s cannot be written to.\n", filename);
		free(strnum);
		free(filename);
		return;
	}
	// first line: url
	fprintf(fp, "%s\n", webpage_getURL(webpage));
	// second line: depth
	fprintf(fp, "%d\n", depth);
	// third line+ : html data
	fprintf(fp, "%s", webpage_getHTML(webpage));

	// FREE STRUCTURES
	fclose(fp);
	free(strnum);
	free(filename);
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
    strcat(ret, "/");
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

// Lab 4 helper - returns int value of a string
int stringToInt(char *string) {
	return strtol(string, NULL, 10);
}

// Lab 4 helper - checks if a directory is a crawler-produced/reachable directory
// by opening a file dir/.crawler with read permissions
bool isCrawlerDirectory(char *dirname) {
    //allocate new string
    char *filename = ".crawler"; //null terminated file name
    char *crawlerfile = filenameCreator(dirname, filename);
    FILE *fp = NULL;
    if ((fp = fopen(crawlerfile, "r")) == NULL) {
        free(crawlerfile);
        return false;
    } else {
        free(crawlerfile);
        fclose(fp);
        return true;
    }
}

//Lab 5 - simple file check
bool isReadableFile(char *filename) {
    FILE *fp = NULL;
    if ((fp = fopen(filename, "r")) == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}
