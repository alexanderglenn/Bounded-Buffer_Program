Made by Alexander Glenn

Files in the directory

- buffer.c
- buffer.h
- readme.txt (this file)

To compile the code, use
-> gcc -g -pthread -o buffer buffer.c

To run the program, use
./buffer [sleep time before terminating] [number of producer threads] [number of consumer threads]

Example: To sleep for 10 seconds, 5 producer threads, and 2 consumer threads
-> ./buffer 10 5 2

NOTE: The buffer size is hard coded. To change to the buffer size, edit the buffer.h header file and
and change BUFFER_SIZE to the desired buffer size.
-> nano buffer.h
E.G to define a buffer size of 5, use
#define BUFFER_SIZE	5

