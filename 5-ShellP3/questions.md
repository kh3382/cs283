1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation uses waitpid() to pause and monitor the child processes until all pipeline commands are complete. If I forgot to call waitpid() on all the child processes, this would lead to resource leaks, broken prompt behavior, and improper synchronization between the command execution and user interaction.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() to ensure proper EOF signaling and to avoid blocking and potential file descriptor leaks. If you leave the pipes open, the pipeline hangs and the shell could become unresponsive or unable to open new files due to file descriptor leaks. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The command 'cd' is implemented as a built-in rather than an external command because changing the working directory affects the shell's process itself. If 'cd' were to be implemented as an external command, it would run in a child process created through fork(), and changing directories in the child shell would not affect the parent shell. In addition, using execvp() to handle 'cd' would replace the shell entirely, which is not what we want. Thus, 'cd' must be a built-in command in order to properly manage the shell's state. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

In order to handle an arbitrary number of piped commands, I would have to modify the shell to dynamically allocate and resize the command list using malloc() and realloc(). Rather than a fixed-size array, it would require using a pointer to a dynamically allocated array of commands that can adjust the size of the array when needed. The trade-offs would be increased complexity in memory management and peformance, but better scalability and more efficient memory use in comparison to fixed-size arrays. 