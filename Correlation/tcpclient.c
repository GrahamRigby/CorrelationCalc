#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include <stdlib.h>

#define SER_PORT 2400

/*This file is the client side for calculating correlation and largest spread
*While tcpserver.c is running in your console run this file with
*argument one as your host address, argument 2 as your data file*/

int main(int count,char*arg[]) {
  int thesock;
  char str[30], str2[30], corr[25], largestS[25];

  // Creating Client Server Connection Using Sockets
  struct sockaddr_in cliaddr;
  cliaddr.sin_port=htons(SER_PORT);
  cliaddr.sin_family=AF_INET;
  cliaddr.sin_addr.s_addr=inet_addr(arg[1]);
  thesock=socket(AF_INET,SOCK_STREAM,0);

  if(thesock<0)
  {
    perror("\n SOCKET");
    exit(0);
  }
  if(connect(thesock,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
  {
    perror("\n CONNECT");
    exit(0);
  }
  //successful connetion
  printf("\nclient connected to %s \n",arg[1]);

  read(thesock,str2,sizeof(str2));
  printf("%s\n", str2);
  //opens the file specified and prints everyline in file
  FILE *ptr_file;
  char buf[128];
  int lines = 0;

  //this sends to the server the amount of lines in the file
  ptr_file =fopen(arg[2], "r");
  while (fgets(buf, 128, ptr_file)!=NULL)
      lines++;
  sprintf(buf, "%d", lines);
  write(thesock, buf, sizeof(buf));

  read(thesock,str,sizeof(str));
  printf("%s\n", str);
  //resets file pointer to the begginning and then sends the data line by line.
  fseek(ptr_file, 0, SEEK_SET);
  while (fgets(buf, 128, ptr_file)!=NULL)
      write(thesock, buf, sizeof(buf));

  //Closes file and lets the server know we are out of data
  fclose(ptr_file);
  write(thesock, "Q", sizeof(buf));

  //Reads the anaysis from the server and prints to the console
  read(thesock, corr, sizeof(corr));
  read(thesock, largestS, sizeof(largestS));
  float correlation = atof(corr);
  float LargestSPREAD = atof(largestS);
  printf("Correlation: %f\n", correlation);
  printf("Largest Spread between two entries on any row: %f\n", LargestSPREAD);
  printf("Closing Connection");

  close(thesock);
  return 0;
}
