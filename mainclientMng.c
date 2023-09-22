#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <sys/time.h>
#include "clientMng.h"/*client Manager functions decleration*/
#define CLIENT_PORT 1025
#define IP_ADDRESS ("192.168.1.246")

int main()
{
    int clientRegistrationResult;
    int clientGroupResult, flag = 1;
    ClientManager* ptrClientManager;
    if(NULL == (ptrClientManager = ClientManagerCreate(CLIENT_PORT, IP_ADDRESS)))
    {
        return -1;
    }
    while(flag)
    {
        clientRegistrationResult = ClientManagerRunRegistration(ptrClientManager);
        if( clientRegistrationResult == MANAGER_ALLOCATION_ERROR || clientRegistrationResult == MANAGER_UNINITIALIZED_ERROR)
        {
            ClientManagerDestroy(&ptrClientManager);
            return -1;
        }
        if(clientRegistrationResult == MANAGER_CLIEN_EXIT)
        {
            flag = 0;
        }
        clientGroupResult = ClientManagerRunGroup(ptrClientManager);
        if(clientGroupResult == MANAGER_CLIEN_EXIT)
        {
            flag = 0;
        }
        if(clientGroupResult == MANAGER_ALLOCATION_ERROR)
        {
            ClientManagerDestroy(&ptrClientManager);
            return -1;
        }
    }

   
    
    return 0;
}

