1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client is able to determine when a command's output is fully received when RDSH_EOF_CHAR is returned. By looping with recv() and checking for EOF, it is able to handle partial reads and ensure complete message reception.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Because TCP does not preserve message boundaries, a networked shell protcol relies on explicit delimiters to detect the beginning and ending of a command sent over TCP connection. If this is not handled properly, this can lead to hanging connections or command/response corruption, which would break the expected shell behavior. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols are able to maintain client state overtime by keeping track of the client session, and because of this, it can be more efficient and preferred for ongoing sessions. A stateless protocol does not remember past interactions, as each request is independent and self-contained. Thus, in the context of our shell project, the remote shell is stateful because it keeps presistent TCP connection and remembers the client session. 

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Despite its unreliability, UDP can be useful in specific scenarios where speed and efficiency are more important. It is used when speed and minimal overhead are more important and is best for real-time communication. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operation system provides sockets, which can be viewed as virtual cables that allow us to connect to other programs on the network. Once connected, data can be shared between the two.