#include "handler.h"

//! ERRORS

char* err_name(error err)
{
    switch(err)
    {
        case 0: return "NO_ERROR";
        case 1: return "NUMBER_OVERFLOW";
        case 2: return "SUM_OVERFLOW";
        case 3: return "BAD_CHARACTER";
        default: return "BAD_ERROR_STATE"; //shouldn't ever occur
    }
}
void err_info(error err) { if(err != NO_ERROR) printf("%s%s%s\n", RED, err_name(err), RESET); }
bool err_check(error_t status) { return status.err != NO_ERROR; }

//! ERROR DETECTION

bool will_overflow(uint32_t a, uint32_t b) { return a > UINT32_MAX - b; }
bool is_valid(char c) { return ((c == 32) || (c == 10) || (c == 13) || (c >= 48 && c <= 57)); }
bool overflows(char* value)
{
    if(!strcmp(value, LIMIT)) return false;
    if(strlen(value) > strlen(LIMIT)) return true;
    else if(strlen(value) < strlen(LIMIT)) return false;
    else {
        int i = 0;
        for(i = 0; i<strlen(value); i++) {
            if(value[i] < LIMIT[i]) 
                return false;
        }
        return true;
    }
}

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
void free2d(char** values, uint32_t rows)
{
    int i = 0;
    for(i = 0; i<rows; i++)
        free(*(values+i));
    free(values);
    values = NULL;
}

//! PROTOCOL SEGMENTS

char** split(message_t* message, error_t* status)
{
    char* mess = message->info;
    uint32_t del_count = 0, mess_len = 0;
    const char delimiter = ' ';
    while(*(mess)) {
        if(!is_valid(*mess)) { 
            status->err = BAD_CHARACTER; 
            return NULL;
        }
        if(*mess == delimiter) {
            del_count++;
            while(*mess == delimiter) { mess++; mess_len++; }
        } else {
            mess_len++;
            mess++;
        } 
    }

    mess -= (mess_len); //wracamy do poczatku wiadomosci

    char** array = malloc(sizeof(*array) * (mess_len+1));
    char** beginning = array;

    uint32_t word_counter = 0, word_len = 0;

    //slow mamy o 1 wiecej niz spacji
    for(word_counter = 0; word_counter < del_count + 1; word_counter++) {
        while(*(mess+word_len) != delimiter) word_len++;
        *array = malloc(sizeof(**array) * (word_len+1)); //+1 bo trzymamy na koncu '\0'
        memcpy(*array, mess, word_len); //kopiujemy slowo do naszej tablicy slow
        *((*array) + word_len) = '\0'; //ustawiamy na koniec slowa w tablicy '\0'
        mess += word_len + 1; //przesuwamy pointer tak aby wskazywal na koniec poprzedniego slowa
        while(*mess == delimiter) mess++;
        array++; //bierzemy kolejna, pustą tablice
        word_len = 0; //zerujemy, bo bierzemy nowe slowo
    }
    message->no_values = word_counter;
    return beginning;
}
uint32_t sum(char** values, uint32_t no_values, error_t* status)
{
    int i = 0;
    uint32_t val = 0, total = 0;
    for(i = 0; i<no_values; i++) {
        if(overflows(*values)) {
            status->err = NUMBER_OVERFLOW;
            return total;
        }
        val = atoi(*values);
        if(will_overflow(total, val)) {
            status->err = SUM_OVERFLOW;
            return total;
        } 
        else total += val;
        values++;
    }
    return total;
}

//! PROTOCOL

char* summation_protocol(uint32_t* no_message, message_t message, error_t status)
{
    char* output_message = malloc(sizeof(*output_message) * BUFFER);
    memset(output_message, 0, strlen(output_message));
    printf("[%d] RECEIVED: %s%s%s", *no_message, GREEN, message.info, RESET);
    char** values = split(&message, &status);
    if(err_check(status)) 
        sprintf(output_message, "[%d] %s%s%s\r\n", *no_message, RED, err_name(status.err), RESET);
    else {
        uint32_t output_sum = sum(values, message.no_values, &status);
        if(err_check(status)) 
            sprintf(output_message, "[%d] %s%s%s\r\n", *no_message, RED, err_name(status.err), RESET);
        else
            sprintf(output_message, "[%d] SUM = %s%u%s\r\n", *no_message, GREEN, output_sum, RESET);
    }
    return output_message;
}

void reset(char** values, message_t message, error_t status, char* output_message)
{
    free2d(values, message.no_values);
    status_reset(&status);
    msg_clear(&message);
    free(output_message);
}