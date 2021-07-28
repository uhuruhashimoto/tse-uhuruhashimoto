/* 
* pagedir.h - header file for pagedir.c
* which contains various filename-related modules
*
* Uhuru Hashimoto CS50 21X
*/

/*
* checks existence and write permissions of directory by opening a file 
* "crawler" to write to
*/
bool checkDir(char *dirname);

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