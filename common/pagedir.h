/* 
* pagedir.h - header file for pagedir.c
* which contains various filename-related modules
*
* Uhuru Hashimoto CS50 21X
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//libcs50 library modules
#include "../libcs50/webpage.h"

/*
* checks existence and write permissions of directory by opening a file 
* "crawler" to write to
*/
bool checkDir(char *dirname);

/*
* saves webpage data to a specified file of name "doc_id" in directory "dirname"
* chardepth is the depth as a string, e.g. "12" or "0"; 
* responsiblity of the user to create directly as an input argument or by using intToString
* and free if necessary afterwards.
*/
void pagesaver(webpage_t *webpage, int depth, char *dirname, int doc_id);

/*
* allocates and creates a string "dirname/filename"
* from two strings "dirname" and "filename" (assumed null-terminated)
* It is left to the user to free the returned string.
*/
char *filenameCreator(char *dirname, char *filename);

/*
* converts an int of arbitrary length to a string using snprintf.
* credit to https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
* for use of snprintf (alternately, one could use log10 and libmath).
* It is left to the user to free the returned string.
*/
char *intToString(int num);

/*
* Lab 4 helper - returns int value of a string
*/
int stringToInt(char *string);

/*
* check if a specified dirname is a crawler-produced directory
* by opening a dirname/.crawler file with read permissions
*/
bool isCrawlerDirectory(char *dirname);

