#ifndef LOG_H
#define LOG_H

#include "config.h"

//* print debug info  
void _debug(char *args, ...);

//* print server info  
void _info(char *args, ...);

//* print mode info  
void _mode(char *args, ...);

//* Show no debug info  
void _no_debug(char *args, ...);

//* Show no server info  
void _no_info(char *args, ...);

//* Displays buffer in both symbols and ascii values  
void whole_chunk_in_ascii();

//* Displays line in both symbols and ascii values  
void line_in_ascii();

//* Show info about bad arguments used
void bad_arg_info();

//* Show server help info
void help_info();

//* Show current server mode
void mode_info();

/**
 * @param error error value
 * @returns name of the error
*/
char* name(error_t error);

#endif