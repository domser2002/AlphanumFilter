# Alphanum Filter
## Description
This is an application written in C which consists of server and client app. Server and client communicate by fifo. Client sends the content of any file passed as an argument, server filters out non alphanumeric characters and prints the output to the console.
## Usage
To run this project, use:
```
make all
./server.o a
./client.o server.c a
```
In this case a is the name of fifo and server.c is the file which will be filtered.
