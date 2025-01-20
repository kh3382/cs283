#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
char *reverse_string(char *, int, int);
void word_print(char *, int, int);
void search_replace(char *, int, int, char *, char *);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int length = 0;

    // Calculate the length of user_str
    int user_str_len = 0;
    const char *temp = user_str;        // Temporary pointer
    while (*temp++ != '\0') {
        user_str_len++;
    }

    // Return error code if user_str is longer than len
    if (user_str_len > len) {
        return -1;
    }

    // Copy characters from user_str to buff
    while (*user_str != '\0') {
        if (*user_str == ' ' && (length > 0 && buff[-1] == ' ')) {
            user_str++;         // Skip consecutive spaces
            continue;
        } else if (*user_str == '\t') {
            *buff = ' ';        // Replace tabs with single space
        } else {
            *buff = *user_str;  // Copy characters
        }
        length++;               // Increment length counter
        buff++;                 // Move buffer pointer forward
        user_str++;             // Move string pointer forward
    }

    // Pad remaining buffer with '.'
    for (int i = length; i < len; i++) {
        *buff = '.';
        buff++;
    }

    // Return number of characters (excluding padding)
    return length; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int word_count = 0;             // Word count
    bool new_word = false;          // Flags True if loop is inside a word

    for (int i = 0; len && i < str_len; i++) {
        if ((buff[i] != ' ') && (buff[i] != '.') && (buff[i] != '\0')) {
            // Inside a word
            if (!new_word) {
                word_count++;       // New word
                new_word = true;    // Set flag
            }
        } else {
            // Outside a word
            new_word = false;       // Reset flag
        }
    }

    // Returns word count of string
    return word_count;
}

char *reverse_string(char *buff, int len, int str_len) {
    // Handle error if str_len exceeds buffer size
    if (str_len > len) {
        return NULL;
    }

    // Allocate space for reversed string using malloc
    char *rev_string = (char *)malloc((str_len + 1) * sizeof(char));

    // Handle memory allocation failure
    if (rev_string == NULL) {
        return NULL;
    }

    // Reverse the string
    for (int i = 0; i < str_len; i++) {
        rev_string[i] = buff[str_len - i - 1];
    }

    // Null-terminate reversed string
    rev_string[str_len] = '\0';

    return rev_string;
}

void word_print(char *buff, int len, int str_len) {
    int num = 1;                    // Number counter
    printf("Word Print\n");
    printf("----------\n");
    
    for (int i = 0; i < str_len && i < len; i++) {
        // Skip whitepsaces until the start of a word
        while (i < str_len && i < len && buff[i] == ' ') {
            i++;                    // Next character
        }

        if (i >= str_len || i >= len) {
            break;                  // Exit loop if end of buffer
        }

        int word_start = i;         // Start of a word
        int length = 0;             // Length of word

        // Count characters in the word
        while (i < str_len && i < len && buff[i] != ' ') {
            length++;               // Increment length counter
            i++;                    // Next character
        }

        // Print the word and length
        printf("%d. ", num++);

        // Print word character by character
        for (int j = word_start; j < word_start + length; j++) {
            putchar(buff[j]);       
        }

        printf(" (%d)\n", length);
    }
}

void search_replace(char *buff, int len, int str_len, char *old_word, char *new_word) {
    char temp[len];             // Temporary pointer
    int temp_index = 0;
    int old_word_len = 0;
    int new_word_len = 0;
    int i = 0;

    // Calculate the length of old word
    while (old_word[old_word_len] != '\0') {
        old_word_len++;
    }

    // Calculate the length of the new word
    while (new_word[new_word_len] != '\0') {
        new_word_len++;
    }

    while (i < str_len && i < len) {
        bool match = true;       // Flag if substring is a match
        // Check if the current substring matches the old word
        for (int j = 0; j < old_word_len; j++) {
            // Not a match
            if (i + j >= str_len || buff[i+j] != old_word[j]) {
                match = false;
                break;
            }
        }
        
        // If matched, replace old word with new word
        if (match) {
            for (int j = 0; j < new_word_len; j++) {
                if (temp_index < len - 1) {
                    temp[temp_index++] = new_word[j];
                } else {
                    break;      // Prevents overflow
                }
            }
            i += old_word_len;      // Skip over substring                     
        } else {
            // Copy current character as is
            if (temp_index < len - 1) {
                temp[temp_index++] = buff[i++];
            } else {
                break;              // Prevent overflow
            }
        }
    }

    // Null-terminate the temporary buffer
    temp[temp_index] = '\0';

    // Copy modified string from temp to buff
    for (int k = 0; k < len && temp[k] != '\0'; k++) {
        buff[k] = temp[k];
    }

    // Null-terminate buff
    if (temp_index < len) {
        buff[temp_index] = '\0';
    }
    
    printf("Modified String: %s\n", buff);
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    char *rc_char;          //return code for char values
    int  user_str_len;      //length of user supplied string
    char *old_word;         //old word to be replaced
    char *new_word;         //new word to replace old word

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      This is a safe because it is essentially error checking the bash script command.
    //      Because of this, if argv[1] does not exist, then the program calls the usage() function
    //      with the program file name as its input (in this case, "./stringfun"), which outputs
    //      a string with instructions on how to call the program correctly.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      The if statement is checking to see if the total command-line arguments is less
    //      than 3 arguments (including the name of the program). 
    //      In this case, if it is less than 3 arguments, then the program handles the error
    //      by calling the usage() function with the program file name as its input 
    //      (in this case, "./stringfun"), which outputs
    //      a string with instructions on how to call the program correctly.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*) malloc(BUFFER_SZ * sizeof(char)); 
    if (buff == NULL){
        usage(argv[0]);
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r':
            rc_char = reverse_string(buff, BUFFER_SZ, user_str_len);
            if (rc_char == NULL) {
                printf("Error reversing string, rc = %s", rc_char);
                exit(2);
            }
            printf("Reversed String: %s\n", rc_char);
            break;

        case 'w':

            word_print(buff, BUFFER_SZ, user_str_len);
            break;

        case 'x':
            if (argc < 5) {
                printf("Error, not enough arguments for -x option");
                exit(2);
            }
            old_word = argv[3];
            new_word = argv[4];
            search_replace(buff, BUFFER_SZ, user_str_len, old_word, new_word);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          The reason why explicitly passing the buffer's length despite
//          knowing that the buff variable will have exactly 50 bytes is good practice
//          is because it makes the helper functions become more reusable in being able
//          to work with buffers of different sizes. If the buffer size changes, the helper
//          function will not need any modification because it relies on the len value. In
//          addition, it prevents buffer overflows by enabling the helper functions to verify
//          and control how many bytes it processes.