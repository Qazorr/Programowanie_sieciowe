#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "log.h"

/**
 ** Set line.status
 * @param error error set
 * @returns whatisreturned
*/
void err_set(error_t error);

/**
 * @returns true if an error occured
*/
bool err_check();

/**
 ** Reinitalize buffer to default  
*/
void buffer_reset();

/**
 ** Reinitalize line to default  
*/
void line_reset();

/**
 ** Check if sum of 2 numbers will result in overflow  
 * @param listofparameters
 * @returns whatisreturned
*/
bool will_overflow(ul a, ul b);

/**
 ** Set the time after which client will be disconnected after providing no data
 * @param seconds amount of seconds
 * @param microseconds amount of microseconds
*/
void set_timeout(int seconds, int microseconds);

/**
 ** If in info-mode, displays corresponding message and closes client descriptor  
 * @param cause cause of loss
*/
void connection_lost(const char* cause);

/**
 ** Copy source to destination
 * @param destination 
 * @param source c-string to get characters from
 * @param num amount of characters copied at maximum
 * @returns pointer to beggining destination c-string
*/
char* my_strncpy(char* destination, const char* source, size_t num);

/**
 ** Append 'num' characters of source to destination
 * @param destination c-string to append to
 * @param source c-string to get characters from
 * @param num amount of characters copied at maximum
 * @returns pointer to beggining destination c-string
*/
char* my_strncat(char* destination, const char* source, size_t num);

/**
 ** Read client message and detect ending sequences  
 * @returns if an error occured sets line.status to corresponding error
*/
bool line_read();

/**
 ** Interpret finished line (ended with '\r\n' sequence)  
 * @returns sets an error state if the line wasn't right with the protocol
*/
void line_interpret();

/**
 ** Convert a c-string to number and add without overflow
 * @param number number to be added to sum in c-string
 * @param sum current value of sum
 * @returns if an error occured sets line.status to corresponding error
*/
void safe_add(char* number, ul* sum);

/**
 ** Parse c-string to corresponding mode value
 * @param argv c-string to be parsed
 * @returns mode value (mode indicator)
*/
int convert_to_mode(char* argv);

/**
 ** Parse main function arguments and activate server modes  
 * @param argc count of main function arguments
 * @param argv list of main function arguments
*/
void handle_arguments(int argc, char* argv[]);

/**
 ** Initalize the server with chosen modes  
 * @param argc count of main function arguments
 * @param argv list of main function arguments
*/
void server_init(int argc, char* argv[]);

/**
 ** Start the summation protocol for initialized server  
*/
void summation_protocol();

#endif