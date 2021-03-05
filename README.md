# Readme

#### Simple Mail Client/Server Application Implementation

This is a implementation of a simple email client, and a mail server using the Socket Interface Programming in C.

## How to Run

Open terminal and change directory into the assignment folder.

```bash

cd ASSIGNMENT1/

```

There is a Makefile provided, which on typing 'make' generates all the required executables.


```bash

make

```

Run both the server and client files in **different terminals** of the same machine if testing is done on the same machine.

In one terminal run the server file and keep it up and running, listening to port number **8080** or any open port.

```bash

./ms 8080

```

In another terminal, run the client file with IP address of the server(local host in the case of same machine,**127.0.0.1**) and port number same as which the server is listening to, **8080** here in this case, as arguments.

```bash

./mc 127.0.0.1 8080

```

The Client will display "Main-Prompt>", here the valid commands are:

- Listusers
- Adduser <userid>
- SetUser <userid>
- Quit

When any user is set using "SetUser <userid>" , the client will display "Sub-Prompt-<userid> >" , the valid commands here are:

- Read
- Delete
- Send <receiverid>
- Done

After a session is closed, type "make clean" to remove all the executable files.

```bash

make clean

```
This does not remove the spool files generated during a session, so as to resume the session where we left off.

## License
[MIT](https://choosealicense.com/licenses/mit/)
