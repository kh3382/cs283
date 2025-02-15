1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` is a good choice for this application because it reads a line of text from an input stream and is able to prevent buffer overflow by specifying a maximum number of characters to read.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using `malloc()` to allocate memory rather than allocating a fixed array allows for flexibility on input size and avoids stack overflow. In addition, a fixed-size array wastes memory when commands are less than shorter than the allocated memory. With `malloc()`, we are able to allocate memory without wasting memory space, and can use `realloc()` to expand the buffer when necessary.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces is necessary to ensure proper parsing and execution of commands in the shell. If we didn't trim spaces, the shell could possibly fail to locate the executable and the shell might pass additional unecessary arguments. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Three redirections we will need to implement are redirecting output (> and >>), redirecting input (<), and redirecting STDERR (2>, 2>>). Some challenges we might experience implementing them could be correctly parsing redirections while handling multiple arguments or managing file descriptors.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection controls how a command reads input or writes output to or from files, while piping connects the output of one command to the input of another command in a single operation without using any temporary files.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep STDERR and STDOUT separate in a shell in order to distinguish between useful output and error messages. By redirecting errors, it ensures for efficient error handling and debugging.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  By default, the shell should print error messages (STDERR) to the terminal separately from STDOUT to avoid confusion. There should be a way to merge both STDOUT and STDERR because users sometimes need a single log file with both normal output and errors or there may be some programs that require both outputs to be processed together. To merge STDOUT and STDERR, we need to handle the redirection syntax 2>&1 to redirect STDERR to wherever STDOUT is going.