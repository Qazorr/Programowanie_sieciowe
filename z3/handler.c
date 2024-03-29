#include "handler.h"

//! ERRORS

char* err_name(error err)
{
    switch(err)
    {
        case NO_ERROR: return "";
        case NUMBER_OVERFLOW: return "NUMBER_OVERFLOW";
        case SUM_OVERFLOW: return "SUM_OVERFLOW";
        case BAD_CHARACTER: return "BAD_CHARACTER";
        case BAD_ENDING_SEQUENCE: return "BAD_ENDING_SEQUENCE";
        case NO_INPUT: return "NO_INPUT";
        default: return "BAD_ERROR_STATE"; //shouldn't ever occur
    }
}
void err_info(error err) { if(err != NO_ERROR) printf("%s%s%s\n", RED, err_name(err), RESET); }
bool err_check(error_t status) { return status.err != NO_ERROR; }

//! ERROR DETECTION

bool will_overflow(ul a, ul b) { return a > ULONG_MAX - b; }
bool is_valid(char c) { return ((c == 32) || (c == LF) || (c == CR) || (c >= 48 && c <= 57)); }
bool overflows() { return errno == ERANGE; }

//! INITIALIZATION AND RESETS

void status_init(error_t* status) { status->err = NO_ERROR; }
void status_reset(error_t* status) { status->err = NO_ERROR; }
void msg_init(message_t* message)
{
    message->info = (char*) malloc(sizeof(char) * BUFFER);
    message->no_values = 0;
}
void msg_clear(message_t* message)
{
    memset(message->info, 0, BUFFER);
    message->no_values = 0;
}
void msg_free(message_t* message) 
{
    free(message->info);
    message->info = NULL;
    message->no_values = 0;
}
void free2d(char** values, ul rows)
{
    if(values) {
        int i = 0;
        for(i = 0; i<rows; i++)
            free(*(values+i));
        free(values);
        values = NULL;
    }
}

//! PROTOCOL SEGMENTS

char** split(message_t* message, error_t* status)
{
    char* mess = message->info;
    ul del_count = 0, mess_len = 0;
    const char delimiter = ' ';
    bool empty = true;
    
    //? counting the lenght of the message and number of delimiters (multi-spaces count as 1)
    while(*(mess)) {
        if(!is_valid(*mess)) { 
            status->err = BAD_CHARACTER; 
            return NULL;
        }
        if(*mess == LF) //only accepting one line inputs
            break;
        if(*mess == CR) {   //if CR ('\r) occured, we check if ('\n) is next to it, if not switch error state
            if(*(mess+1) == LF) 
                break;
            status->err = BAD_ENDING_SEQUENCE;
            return NULL;
        }
        if(*mess == delimiter) {
            del_count++;
            while(*mess == delimiter) { mess++; mess_len++; } //move past all whitespaces
        } else {
            mess_len++;
            mess++;
            empty = false;
        }
    }

    if(empty) {
        status->err = NO_INPUT;
        return NULL;
    }

    mess -= (mess_len); //comming back at the beginning of the message

    char** array = malloc(sizeof(*array) * (del_count+1)); //2d array with splitted sequences
    char** beginning = array; //pointer to beggining of the array

    ul word_counter = 0, word_len = 0;

    //we have one word more than the delimiter's count (Ala ma kota - 3 words, 2 delims)
    for(word_counter = 0; word_counter < del_count + 1; word_counter++) {
        while(!(*(mess+word_len)) == '\0' && *(mess+word_len) != delimiter) word_len++; //finding the word lenght
        *array = malloc(sizeof(**array) * (word_len+1)); //+1 because we need the space for '\0'
        memcpy(*array, mess, word_len); //copying the word to the allocated array
        *((*array) + word_len) = '\0';
        mess += word_len + 1; //moving the pointer to the next word
        while(*mess == delimiter) mess++; //moving past unnecessary whitespaces
        array++; //moving to the next empty slot
        word_len = 0;
    }
    message->no_values = word_counter; //keeping number of words since it'll be useful later
    return beginning;
}
ul sum(char** values, ul no_values, error_t* status)
{
    int i = 0;
    ul val = 0, total = 0;
    for(i = 0; i<no_values; i++) {
        val = strtoul(*values, NULL, 10);
        if(overflows()) {
            status->err = NUMBER_OVERFLOW;
            errno = 0;
            return total;
        }
        if(will_overflow(total, val)) {
            status->err = SUM_OVERFLOW;
            return total;
        }
        total += val;
        values++;
    }
    return total;
}

//! PROTOCOL

char* summation_protocol(message_t message, error_t status)
{
    char* output_message = malloc(sizeof(*output_message) * BUFFER);
    memset(output_message, 0, strlen(output_message));
    if(SHOW_ON_SERVER) printf("RECEIVED: %s%s%s", GREEN, message.info, RESET);   //print onto the server message from the client
    char** values = split(&message, &status);   //split the message to single numbers
    
    if(err_check(status)) { //check whether split() generated an error
        err_info(status.err);
        sprintf(output_message, "%s\n", ERROR_MESSAGE); //generate error message
    } else {
        ul output_sum = sum(values, message.no_values, &status); //sum the numbers
        if(err_check(status)) { //check whether sum() generated an error
            err_info(status.err);
            sprintf(output_message, "%s\n", ERROR_MESSAGE); //generate error message
        } else {
            if(!SHOW_ON_SERVER) printf("%sMSG SENT%s\n", GREEN, RESET);
            sprintf(output_message, "%lu\n", output_sum); //generate message with sum
        }
    }
    free2d(values, message.no_values);
    return output_message;
}
void reset(message_t *message, error_t *status, char* output_message)
{
    status_reset(status);
    msg_clear(message);
    free(output_message);
    output_message = NULL;
}