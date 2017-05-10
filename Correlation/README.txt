This program uses client server communication via sockets that takes a data file of two columns and an arbitrary amount of rows, and calculates the correlation 
between the two columns. In addition it calculates the largest spread between any two entries on all rows. This calculations are done on the server side, which
creates additional processes and sets up a pipe for interprocessor communication.

Data files must be formatted such that there are two integers on each row separated by a single space.
EX:
12 20
13 15
16 20
333 2000
54 10

Running:
in your console run the makefile: make
then run the server ./tcpserver 
#no arguments for server

then run the client ./tcpclient 127.0.0.1 data.txt
#argument 1 represents address argument 2 represents the data file.