#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

 // helper function for splitting arguments with quote handling
int split_args_with_quotes(char *line, cmd_buff_t *cmd) {
    char *p = line;
    while (*p) {
        // skip spaces
        while (isspace(*p)) p++; 

        // end of string
        if (*p == '\0') break; 

        char *start;
        char quote_char = '\0';

        if (*p == '"' || *p == '\'') {
            // store quote
            quote_char = *p;
            p++; 
            start = p;
            // finds closing quote
            while (*p && *p != quote_char) p++; 
        } else {
            // no quotes
            start = p;
            while (*p && !isspace(*p)) p++; 
        }

        int len = p - start;
        
        if (cmd->argc >= ARG_MAX) return ERR_CMD_OR_ARGS_TOO_BIG;

        cmd->argv[cmd->argc] = malloc(len + 1);
        if (!cmd->argv[cmd->argc]) return ERR_MEMORY;

        strncpy(cmd->argv[cmd->argc], start, len);
        cmd->argv[cmd->argc][len] = '\0'; 

        cmd->argc++;

        if (*p == quote_char) p++; 
    }

    cmd->argv[cmd->argc] = NULL; 
    return OK;
}

// helper function for parsing input
int parse_input(char *cmd_line, command_list_t *cmd_list) {
    char *token, *cmd_pointer;

    // cmd counter
    int cmd_count = 0;

    // split command with pipe tokenization
    token = strtok_r(cmd_line, PIPE_STRING, &cmd_pointer);

    while (token != NULL) {
        if (cmd_count >= SH_CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        // initialize cmd_buff_t struct for each new command
        cmd_buff_t *cmd = &cmd_list->commands[cmd_count++];
        memset(cmd, 0, sizeof(cmd_buff_t));

        // strip leading & trailing spaces
        while (isspace(*token)) token++; 
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';

        // parse arguments
        int arg_result = split_args_with_quotes(token, cmd);
        if (arg_result != OK) return arg_result;

        // next command
        token = strtok_r(NULL, PIPE_STRING, &cmd_pointer);
    }

    cmd_list->num = cmd_count;
    return OK;
        
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
 int exec_local_cmd_loop()
 {
     char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
     if (!cmd_buff) return ERR_MEMORY;
    
     int rc = OK;
 
     // TODO IMPLEMENT MAIN LOOP
     while (1)
     {
         printf("%s", SH_PROMPT);
         if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
         {
             printf("\n");
             break;
         }
 
         // remove the trailing newlines from cmd_buff
         cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
 
         // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
         // initialize cmd_list
         command_list_t cmd_list;
         memset(&cmd_list, 0, sizeof(command_list_t));

         // parse command into cmd_buff_t struct
         rc = parse_input(cmd_buff, &cmd_list);

         if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, cmd_list.num);
            continue;
         } else if (rc == ERR_MEMORY) {
            break;
         }

         // Handle empty input
         if (cmd_list.num == 0) {
             printf(CMD_WARN_NO_CMD);
             rc = WARN_NO_CMDS;
             continue;
         }
 
         // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
         // the cd command should chdir to the provided directory; if no directory is provided, do nothing

        // handle built-in commands (only if there is 1 command, no pipes)
         if (cmd_list.num == 1) {
            cmd_buff_t *cmd = &cmd_list.commands[0];

            // exit command
            if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
                rc = OK_EXIT;
                break;
            }

            // dragon command
            if (strcmp(cmd->argv[0], "dragon") == 0) {
                //print_dragon();
                printf(CMD_OK_HEADER, cmd->argc);
                rc = OK;
                goto cleanup;
            }

            // cd command
            if (strcmp(cmd->argv[0], "cd") == 0) {
                // parse command arguments for cd command
                if (cmd->argc > 1) {
                    rc = (chdir(cmd->argv[1]) == 0) ? OK : ERR_EXEC_CMD;
                } else {
                    // no directory, do nothing
                    rc = OK;
                }
                goto cleanup;
            }
         }
 
         // TODO IMPLEMENT if not built-in command, fork/exec as an external command
         // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

         // handle external commands and pipelines
         int num_pipes = cmd_list.num - 1;
         int *pipefds = NULL;

         if (num_pipes > 0) {
            pipefds = malloc(sizeof(int) * 2 * num_pipes);
            if (!pipefds) {
                rc = ERR_MEMORY;
                goto cleanup;
            }

            // create pipes
            for (int i = 0; i < num_pipes; i++) {
                if (pipe(pipefds + i * 2) < 0) {
                    rc = ERR_EXEC_CMD;
                    goto cleanup;
                }
            }
        }

         int pipeline_rc = OK;

         // fork processes 
         for (int i = 0; i < cmd_list.num; i++) {
            pid_t pid = fork();
            if (pid < 0) {
                rc = ERR_EXEC_CMD;
                goto cleanup;
            }

            // child process
            if (pid == 0) {
                // redirect if not first command
                if (i > 0) {
                    if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) exit(ERR_EXEC_CMD);
                }

                // redirect if not last command
                if (i < cmd_list.num - 1) {
                    if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) exit(ERR_EXEC_CMD);
                }
            
                // close all pipe fds in child process
                if (pipefds) {
                    for (int j = 0; j < 2 * num_pipes; j++) {
                        close(pipefds[j]);
                    }
                }
                
                // execute command
                execvp(cmd_list.commands[i].argv[0], cmd_list.commands[i].argv);
                exit(ERR_EXEC_CMD);
            }
         }
 
        // parent process; close all pipe fds
        if (pipefds) for (int j = 0; j < 2 * num_pipes; j++) close(pipefds[j]);

        // wait for all children
        for (int i = 0; i < cmd_list.num; i++) {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                pipeline_rc = ERR_EXEC_CMD;
            }
        }

        rc = pipeline_rc;

    cleanup:
        // free pipe fds if allocated
        if (pipefds) free(pipefds);

         // free allocated memory
         for (int i = 0; i < cmd_list.num; i++) {
            for (int j = 0; j < cmd_list.commands[i].argc; j++) {
                free(cmd_list.commands[i].argv[j]);
            }
         }

         if (rc == OK_EXIT) break;
     }

     free(cmd_buff);
     return rc;
    }


