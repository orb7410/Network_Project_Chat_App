#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#define BUFFER_SIZE 100
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define RESET "\033[0m"

void RecvFromChat(int _port, const char* _clientIP, char* _userName);

int main(int _argc, char* _argv[])
{
   int port = atoi(_argv[1]);
   RecvFromChat(port, _argv[2], _argv[3]);
   return 0;
}

void RecvFromChat(int _port, const char* _clientIP, char* _userName)
{
   int sock, optval = 1;
   struct sockaddr_in sin;
   struct ip_mreq multisin;
   char buffer[BUFFER_SIZE];
   int readBytes, i=0;
   socklen_t sinSize;
   FILE *ptrFile1;
   int pid;
   memset(&sin, 0, sizeof(sin));
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
      perror("Socket initialization error");
      return;
   }  
   if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
   {
      perror("reuse fail");
      return;
   }
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = inet_addr(_clientIP);
   sin.sin_port = htons(_port);
   if(bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) 
   {
      perror("bind failed");
   }
   if(NULL == (ptrFile1 = fopen("recvChat.txt", "w")))
   {
      printf("open file error\n");
      return;
   }
   pid = getpid();
   fprintf(ptrFile1, "%d", pid);
   fclose(ptrFile1);
   multisin.imr_multiaddr = sin.sin_addr;
	multisin.imr_interface.s_addr = htonl(INADDR_ANY);
   if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multisin, sizeof(multisin)) < 0) 
   {
      perror ("add_membership setsockopt");
      return;
   }
   while(1)
   {
      sinSize = sizeof(sin);
      readBytes = recvfrom(sock, buffer, BUFFER_SIZE -1 ,0 ,(struct sockaddr *) &sin, &sinSize);
      if (readBytes <= 0) 
      {
         perror("recvfrom failed");
      }
      if(i % 2 == 0)
      {
         printf(GREEN"%s"RESET, buffer);
      }
      else
      {
         printf(YELLOW"%s"RESET, buffer);
      }
      i++;
   }
}




