#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "system.h"
#define STR_SIZE 100
Window ChatWindowsOpener(char* _clientPort, const char* _clientIP, char* _userName)
{
    Window window;
    FILE *ptrFile1;
    FILE *ptrFile2;
    char sendOut[STR_SIZE];
    char recvOut[STR_SIZE];
    sprintf(sendOut, "gnome-terminal -e 'bash -c \"./send.out %s %s %s\"'", _clientPort, _clientIP ,_userName);
    sprintf(recvOut, "gnome-terminal -e 'bash -c \"./recv.out %s %s %s\"'", _clientPort, _clientIP ,_userName);
    printf("%s\n", sendOut);
    system(sendOut);
    system(recvOut);
    ptrFile1 = fopen("sendChat.txt", "r");
    if(NULL == ptrFile1)
    {
        printf("open file error\n");
        window.m_firstWindow = 0;
    }
    fscanf(ptrFile1, "%d", &(window.m_firstWindow));
    fclose(ptrFile1);
    ptrFile2 = fopen("recvChat.txt", "r");
    if(NULL == ptrFile2)
    {
        printf("open file error\n");
        window.m_secendWindow = 0;
    }

    fscanf(ptrFile2, "%d", &(window.m_secendWindow)); 
    fclose(ptrFile2);
    return window;
}

void CloseWindows(Window _window)
{
    char firstPid[STR_SIZE];
    char secPid[STR_SIZE];
    sprintf(firstPid, "kill -9 %d", _window.m_firstWindow);
    sprintf(secPid, "kill -9 %d", _window.m_secendWindow);
    system(firstPid);
    system(secPid);
}


Window* CreateWindowsPid()
{
    Window* ptrWindow; 
    if(NULL == (ptrWindow = (Window*)malloc(sizeof(Window))))
    {
        return NULL;
    }
    ptrWindow -> m_firstWindow;
    ptrWindow -> m_secendWindow;
    return ptrWindow;
}

void WindowDestroy(Window** _pWindow)
{
    if (NULL == _pWindow || NULL == *_pWindow )
	{
		return;
	}
    free(*_pWindow);
	*_pWindow = NULL;
}







/*
    sprintf(sendOut, "gnome-terminal --window -- bash -c './send.out' %s %s %s", _clientPort, _clientIP ,_userName);
    sprintf(recvOut, "gnome-terminal --window -- bash -c './recv.out' %s %s %s", _clientPort, _clientIP ,_userName);

    sprintf(sendOut, "gnome-terminal -e 'bash -c \"./send.out %s %s %s; exec bash\"'", _clientPort, _clientIP ,_userName);
    sprintf(recvOut, "gnome-terminal -e 'bash -c \"./recv.out %s %s %s; exec bash\"'", _clientPort, _clientIP ,_userName);*/