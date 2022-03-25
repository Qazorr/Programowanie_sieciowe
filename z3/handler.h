#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memcpy
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //sockaddr_in
#include <errno.h>
#include <limits.h>

//ANSI COLORS
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

/**
*? The field size sets a theoretical limit of 65,535 bytes (8-byte header + 65,527 bytes of data) for a UDP datagram. 
*? However the actual limit for the data length, which is imposed by the underlying IPv4 protocol, is 65,507 bytes 
*? (65,535 bytes − 8-byte UDP header − 20-byte IP header).
*/

#define BUFFER 65507 //maximum size of input
#define ul unsigned long
#define LF 10
#define CR 13

/** 
 ** error states
 * @param NO_ERROR no error occured 
 * @param NUMBER_OVERFLOW number exeeded the size limit
 * @param SUM_OVERFLOW adding two numbers resulted in overflow
 * @param BAD_CHARACTER message containted not supported character
 * @param BAD_ENDING_SEQUENCE CR was not followed by LF
*/
typedef enum error {
    NO_ERROR,
    NUMBER_OVERFLOW,
    SUM_OVERFLOW,
    BAD_CHARACTER,
    BAD_ENDING_SEQUENCE
} error;

/**
 * @param info full message 
 * @param no_values amount of words separated by ' '(32)(SPACE) | after calling split()
*/
typedef struct message
{
    char *info;
    ul no_values;
} message_t;

/* struct wrapping error state */
typedef struct error_t {
    error err;
} error_t;

/** @returns error name*/
char* err_name(error err);

/** Prints information about the error to the stdout */
void err_info(error err);

/** 
 * @param status variable containing error information
 * @returns true if an error occured */
bool err_check(error_t status);

/** 
 * * check if sum of 2 numbers will result in overflow
 * @param a first number to sum 
 * @param b second number to sum
 * @returns true if an overflow occured
*/
bool will_overflow(ul a, ul b);

/**
 * @param c character checked
 * @returns true if c is CR, LF, SPACE, '0-9'
*/
bool is_valid(char c);

/**
 ** function for checking whether a number is bigger than LIMIT 
 * @param number number checked
*/
bool check(char* number);

/**
 * @returns true if converting c-string to unsigned long resulted in error
*/
bool overflows();

/**
 ** initialize error_t struct
 * @param status variable initialzied
*/
void status_init(error_t* status);

/**
 ** reset status to NO_ERROR
 * @param status variable reseted
*/
void status_reset(error_t* status);

/**
 ** initialize message_t variable
 * @param message variable initialized 
*/
void msg_init(message_t* message);

/**
 ** clear message_t.info 
 * @param message variable in which message_t.info is cleared
*/
void msg_clear(message_t* message); 

/**
 ** free the memory initialized by msg_init()
 * @param message variable freed 
*/
void msg_free(message_t* message);

/**
 ** free 2d array 
 * @param values 2d array to free
 * @param rows number of rows of values
*/
void free2d(char** values, ul rows);

/**
 ** split sequence of characters on ' '(32)(SPACE)
 * @param message variable containing sequence to split
 * @param status variable containing error info
 * @returns array containing character sequences containing only numbers
*/
char** split(message_t* message, error_t* status);

/**
 ** sum 2d array containing only numbers (e.g. created by split()) 
 * @param values 2d array to sum
 * @param no_values amount of numbers
 * @param status variable containing error info
 * @returns sum of values array
*/
ul sum(char** values, ul no_values, error_t* status);

/**
 ** reset variables to initial state 
 * @param message wrapper for message reseted by msg_clear()
 * @param status wrapper for error reseted to NO_ERROR
 * @param output_message message served to client freed by free()
*/
void reset(message_t* message, error_t* status, char* output_message);

/**
 ** sum all numeric values in a given message if every condition is met
 * @param no_message indicator for number of the message
 * @param message message given by client
 * @param status state of the protocol - could be NO_ERROR, NUMBER_OVERFLOW, SUM_OVERFLOW, BAD_CHARACTER, BAD_ENDING_SEQUENCE
 * @returns message containing sum of the numbers if everything went alright, else an error which occured
*/ 
char* summation_protocol(int* no_message, message_t message, error_t status);

#endif