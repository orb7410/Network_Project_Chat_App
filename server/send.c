#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#define MASSAGE_SIZE 100
#define NAME_SIZE 8
void SendToChat(int _port, const char* _clientIP, char* _userName);

int main(int _argc, char* _argv[])
{
   int port = atoi(_argv[1]);
   SendToChat(port , _argv[2], _argv[3]);
   return 0;
}

void SendToChat(int _port, const char* _clientIP, char* _userName)
{
   int sock;
   struct sockaddr_in sin;
   int sentBytes, messagesSize, bufferSize, optval = 1;
   socklen_t sinSize;
   int pid;
   FILE *ptrFile;
   char messages[MASSAGE_SIZE];
   char buffer[MASSAGE_SIZE];
   char name[NAME_SIZE];
   strcpy(name, _userName);
   memset(&sin, 0, sizeof(sin));
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(sock < 0)
   {
      perror("Socket initialization error");
      return ;
   }  
   if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
   {
      perror("reuse fail");
      return ;
   }
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = inet_addr(_clientIP);
   sin.sin_port = htons(_port);
   if(bind(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0)
   {
      perror("bind failed");
      return;
   }
   if(NULL == (ptrFile = fopen("sendChat.txt", "w")))
   {
      printf("open file error\n");
      return;
   }
   pid = getpid();
   fprintf(ptrFile, "%d", pid);
   fclose(ptrFile);
   strcat(name, ": ");
   while(1)
   {
      strcpy(messages, name);
      printf("%s messages: ", _userName);
      fgets(buffer, MASSAGE_SIZE -1 , stdin);
      strcat(messages, buffer);
      messagesSize = sizeof(messages);     
      sentBytes = sendto(sock, messages, messagesSize, 0, (struct sockaddr *) &sin, sizeof(sin));
      if (sentBytes < 0)
      {
         perror("sendto failed");
      }
   }
}
   
    


        
        