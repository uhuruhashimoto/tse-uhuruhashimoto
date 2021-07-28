# Readme for Common Library: Lab 4
## Uhuru Hashimoto CS50 21X

The common directory contains the `pagedir` modules and associated header file, `pagedir.h`. These are all file handlers, and are referenced by the crawler program within the crawler directory. 

Run `make` to build the common library `common.a` for use by crawler, or alternately run `make` within the crawler directory (which will make both
necessary libraries automatically). 

Run `make clean` to remove object files after use, or alternately `make clean` within the crawler directory, which also cleans libraries. 

Please note: the `checkDir` module does not remove a user from the directory after filewriting, but remains in the same location. 