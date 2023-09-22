#include <stdio.h> /*printf, NULL*/
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include "clientNet.h"/*client functions decleration*/
#define CLIENT_PORT 1025
#define IP_ADDRESS ("10.0.0.19") /*("192.168.1.246")*/

static void Test1_ClientCreate(void);
static void Test1_ClientDestroy(void);
static void Test2_ClientDestroyDouble(void);
static void Test1_ClientRun(void);

int main(void)
{
    /*printf("\n");
	Test1_ClientCreate();
    Test1_ClientDestroy();
    Test2_ClientDestroyDouble();
	Test1_ClientRun();
    */
	/*char data[12] = "I am client";
    int dataSize = sizeof(data);
	Client* ptrClient = ClientCreate(CLIENT_PORT, IP_ADDRESS);
	ClientNet(ptrClient, data, dataSize);
	ClientDestroy(&ptrClient);*/
    return 0;
}
/*
static void Test1_ClientCreate(void)
{
	Client *ptrClient;
	ptrClient = ClientCreate(CLIENT_PORT, IP_ADDRESS);
	if(NULL != ptrClient)
	{
		printf("%-50s %s", "Test1_ClientCreate","**PASS**\n");
	}
	else
	{
		printf("%-40s %s","Test1_ClientCreate","**FAIL**\n");
	}
	ClientDestroy(&ptrClient);
	printf("\n");
}
static void Test1_ClientDestroy(void)
{
	Client *ptrClient;
	ptrClient = ClientCreate(CLIENT_PORT, IP_ADDRESS);
    ClientDestroy(&ptrClient);
	if(NULL == ptrClient)
	{
		printf("%-50s %s", "Test1_ClientCreate","**PASS**\n");
	}
	else
	{
		printf("%-40s %s","Test1_ClientCreate","**FAIL**\n");
	}
	printf("\n");
}

static void Test2_ClientDestroyDouble(void)
{
	Client *ptrClient;
	ptrClient = ClientCreate(CLIENT_PORT, IP_ADDRESS);
    ClientDestroy(&ptrClient);
    ClientDestroy(&ptrClient);
	printf("%-50s %s", "Test2_ClientDestroyDouble","**PASS**\n");
	printf("\n");
}

static void Test1_ClientRun(void)
{
	Client *ptrClient;
	char data[12] = "I am client";
    int dataSize = sizeof(data);
	ptrClient = ClientCreate(CLIENT_PORT, IP_ADDRESS);
	
	printf("%-50s %s", "Test1_ClientCreate","**PASS**\n");
	ClientDestroy(&ptrClient);
	printf("\n");
}*/