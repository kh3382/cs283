#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

void print_dragon() {
    puts("                                                                        @%%%%                       ");
    puts("                                                                     %%%%%%                         ");
    puts("                                                                    %%%%%%                          ");
    puts("                                                                 % %%%%%%%           @              ");
    puts("                                                                %%%%%%%%%%        %%%%%%%           ");
    puts("                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        ");
    puts("                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          ");
    puts("                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           ");
    puts("                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            ");
    puts("                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            ");
    puts("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                ");
    puts("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              ");
    puts("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              ");
    puts("      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                ");
    puts("    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                ");
    puts("  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        ");
    puts(" %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       ");
    puts("%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     ");
    puts("%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  ");
    puts("%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              ");
    puts("%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  ");
    puts("%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%");
    puts("%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%");
    puts("%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%");
    puts(" %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%");
    puts("  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% ");
    puts("   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% ");
    puts("    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% ");
    puts("     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   ");
    puts("      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               ");
    puts("        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          ");
    puts("           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         ");
    puts("              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        ");
    puts("                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         ");
    puts("                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            ");
    puts("                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          ");
    puts("                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          ");
    puts("                                                                                 %%%%%%%@           ");
}

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // check if no commands
    if (cmd_line == NULL || clist == NULL) {
        return WARN_NO_CMDS;
    }

    // reset clist struct
    memset(clist, 0, sizeof(command_list_t));

    // trim leading spaces    
    while (*cmd_line == SPACE_CHAR) cmd_line++;

    // trim trailing spaces and null terminate command
    int len = strlen(cmd_line);
    while (len > 0 && cmd_line[len - 1] == SPACE_CHAR) cmd_line[--len] = '\0';

    // split command
    char *token, *arg_token, *saveptr1, *saveptr2;
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr1);

    // cmd counter
    int cmd_count = 0;

    while (token != NULL) {
        // handle error if too many commands
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        // get command arguments
        arg_token = strtok_r(token, " ", &saveptr2);

        // handle error if too many arguments
        if (!arg_token || strlen(arg_token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        // dragon command
        if (strcmp(arg_token, "dragon") == 0) {
            print_dragon();
            cmd_count++;
            token = strtok_r(NULL, PIPE_STRING, &saveptr1);
            continue;
        }

        // parse command
        command_t *cmd = &clist->commands[cmd_count++];
        memset(cmd, 0, sizeof(command_t));

        // save arguments to struct
        strcpy(cmd->exe, arg_token);

        cmd->args[0] = '\0';

        // parse arguments and handle error 
        while ((arg_token = strtok_r(NULL, " ", &saveptr2)) != NULL) {
            if (strlen(cmd->args) + strlen(arg_token) + 1 < ARG_MAX) {
                if (cmd->args[0] != '\0') {
                    strcat(cmd->args, " ");
                }
                strcat(cmd->args, arg_token);
            } else {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }
        
        token = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }
    clist->num = cmd_count;
    return OK;
}    