#include <stdio.h> 
#include "serverManager.h"

int main()
{
    ServerManager* ptrServerManager;

    if(NULL == (ptrServerManager = CreateServerManger()))
    {
        return -1;
    }
    if(ServerMenagerRun(ptrServerManager) != SERVER_MENAGER_SUCCESS)
    {
        return -1;
    }
    ServerMangerDestroy(&ptrServerManager);
    return 0;
}

