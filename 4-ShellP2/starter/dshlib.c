#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

// helper function for parsing input
void parse_input(char *cmd_line, cmd_buff_t *cmd) {
    char *token = NULL;  // Ensure token is initialized
    int in_quotes = 0;
    char quote_char = '\0';

    // Initialize command buffer
    cmd->argc = 0;

    for (int i = 0; cmd_line[i] != '\0'; i++) {
        char current_char = cmd_line[i];

        if (current_char == '"' || current_char == '\'') {
            if (in_quotes && quote_char == current_char) {
                cmd_line[i] = '\0';  // End the quoted string
                in_quotes = 0;
            } else if (!in_quotes) {
                in_quotes = 1;
                quote_char = current_char;
                token = &cmd_line[i + 1];  // Start of quoted string
            }
        } else if (!in_quotes && !isspace(current_char)) {
            if (token == NULL) {
                token = &cmd_line[i];  // Start of a new word
            }
        } else if (!in_quotes && (isspace(current_char) || cmd_line[i + 1] == '\0')) {
            if (token != NULL) {
                // Ensure we don't exceed argument limit
                if (cmd->argc >= ARG_MAX) {
                    fprintf(stderr, "Too many arguments\n");
                    return;
                }

                // Replace space or newline with null terminator
                cmd_line[i] = '\0';

                // Store the argument
                cmd->argv[cmd->argc] = strdup(token);
                if (cmd->argv[cmd->argc] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");

                    // Free already allocated arguments before returning
                    for (int j = 0; j < cmd->argc; j++) {
                        free(cmd->argv[j]);
                    }
                    return;
                }

                cmd->argc++;
                token = NULL;  // Reset token for next argument
            }
        }
    }

    // Final check in case the last token wasn't added
    if (token != NULL && cmd->argc < ARG_MAX) {
        cmd->argv[cmd->argc] = strdup(token);
        if (cmd->argv[cmd->argc] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");

            // Free already allocated arguments before returning
            for (int j = 0; j < cmd->argc; j++) {
                free(cmd->argv[j]);
            }
            return;
        }
        cmd->argc++;
    }

    // Null-terminate the argument list (important for execvp)
    cmd->argv[cmd->argc] = NULL;
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
    if (!cmd_buff) {
        return ERR_MEMORY;
    }

    int rc = 0;
    cmd_buff_t cmd;
    memset(&cmd, 0, sizeof(cmd_buff_t));

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
        // parse command into cmd_buff_t struct
        memset(&cmd, 0, sizeof(cmd_buff_t));
        parse_input(cmd_buff, &cmd);

        // ignore empty input
        if (cmd.argc == 0) {
            printf(CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
            continue;
        }

        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing

        // handle built-in commands
        // exit command
        if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
            rc = OK_EXIT;
            break;
        }

        // dragon command
        else if (strcmp(cmd.argv[0], "dragon") == 0) {
            print_dragon();
            printf(CMD_OK_HEADER, cmd.argc);
            rc = OK;
            continue;
        }

        // cd command
        else if (strcmp(cmd.argv[0], "cd") == 0) {
            // parse command arguments for cd command
            if (cmd.argc <= 1) {
                rc = OK;
            } else { 
                if (chdir(cmd.argv[1]) != 0) {
                    rc = ERR_EXEC_CMD;
                } else {
                    rc = OK;
                }
            }
            continue;
        }

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
        // handle external commands
        pid_t pid = fork();
        if (pid < 0) {
            rc = ERR_EXEC_CMD;
        }

        // child process
        else if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            exit(ERR_MEMORY);
        }

        // parent process
        else {
            waitpid(pid, NULL, 0);
        }

        // free allocated memory
        for (int i = 0; i < cmd.argc; i++) {
            free(cmd.argv[i]);
            cmd.argv[i] = NULL;
        }
    }

    free(cmd_buff);
    return rc;
}
