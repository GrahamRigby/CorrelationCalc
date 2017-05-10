#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include <stdlib.h>
#include "tcpserver.h"

#define SER_PORT 2400
#define AWAITINGDATASIZE 1
#define AWAITINGDATA 2
#define CALCULATING 3

/*This file is the server side for calculating correlation and largest spread
* no arguments should be passed in to run this program */

int main() {
  int sersock, newsock, state;
  int *data_size = malloc(sizeof(int));
  *data_size = 0;
  char str[30];
  struct sockaddr_in seraddr;
  struct sockaddr_in cliinfo;
  socklen_t csize=sizeof(cliinfo);
  seraddr.sin_family=AF_INET;
  seraddr.sin_port=htons(SER_PORT);
  seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

  //Sets up a connection via sockets
  if((sersock=socket(AF_INET,SOCK_STREAM,0))<0)
  {
    perror("\n socket");
    exit(0);
  }
  if(bind(sersock,(struct sockaddr *)&seraddr,sizeof(seraddr))<0)
  {
    perror("\nBIND");
    exit(0);
  }
  if(listen(sersock,1)<0)
  {
    perror("\n LISTEN");
  }
  if((newsock=accept(sersock,(struct sockaddr *)&cliinfo,&csize))<0)
  {
    perror("\n ACCEPT");
    exit(0);
  }
  else
  printf("\nConnected to %s\n",inet_ntoa(cliinfo.sin_addr));
  printf("Awaiting data\n");
  //Success


  state = AWAITINGDATASIZE;
  char buf[128];
  if (state == AWAITINGDATASIZE){
    write(newsock, "Sending data size", sizeof(str));
    read(newsock, buf, sizeof(buf));
    *data_size = (atoi(buf));
    printf("Data entries: %d\n", *data_size);
  }

  char **data = (char **) malloc(*data_size * sizeof(char*));
  char data1[*data_size][128];
  int i = 0;

  write(newsock, "Sending data", sizeof(str));
  int terminate = 0;
  while(terminate == 0){
      read(newsock, buf, sizeof(buf));
      if(buf[0] == 'Q'){
        terminate = 1;
      }
      else{
        //data[i] = buf;
        strncpy(data1[i], buf, 128);
        printf("Received: %s",data1[i]);
        data[i] = data1[i];
        i++;
      }
  }

  //check for errors then change the state
  state = CALCULATING;
  printf("Calculating \n");

  /*Here we are going to create an additional process and set up a pipe to
  * allow for interprocessor communication. The two processes will run
  * different functions in computations.c for efficiency. */

  int fd[2];
  pid_t childpid;

  pipe(fd);

  childpid = fork();
  if (childpid == -1)
  {
      perror("Forking fail");
  }
  else if (childpid > 0)
  {
      //parent process
      close(fd[1]);
      float correlation = getCorrelation(data, *data_size);
      printf("Correlation: %f\n", correlation);
      char out[20];
      snprintf(out, 20, "%f", correlation);
      char in[21];
      read(fd[0], in, sizeof(in));
      write(newsock, out, sizeof(out));
      write(newsock, in, sizeof(in));
  }
  else
  {
      //child process
      float largestS = largestSpread(data, *data_size);
      printf("Largets spread between entries on one row: %f\n", largestS);
      char out[20];
      snprintf(out, 20, "%f", largestS);
      close(fd[0]);
      write(fd[1], out, (strlen(out)+1));
      exit(0);
  }
}
