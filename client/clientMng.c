#include <stdio.h> /*printf, NULL*/
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h> /*malloc*/
#include "clientMng.h"/*client Manager functions decleration*/
#include "clientNet.h"/*client.net functions decleration*/
#include "ui.h" /*ui function decleration*/
#include "protocol.h" /*protocol function decleration*/
#include "system.h"
#include "vector.h" /*vector functions decleration*/
#include "HashMap.h"
#define VECTOR_SIZE 10
#define BLOCK_SIZE 0
#define NUM_OF_GROPS 20
static size_t ClientMngHashFunction(void* _groupName);
static int ClientMngEqualityFunction(void* _firstKey, void* _secondKey);
static void PrintGroups(ClientManager* _clientMng);
static int VectorPrintChar(void* _element, size_t _index, void* _context);

struct ClientManager
{
    Client*       m_client;
    UserDetail*   m_userDetail;
    Vector*       m_groups;
    HashMap*      m_pid;
};


ClientManager* ClientManagerCreate(int _clientPort, const char* _clientIP)
{
    ClientManager* ptrClientMng;
    Client* ptrClient;
    Vector* ptrGroupVector; 
    HashMap* ptrPidHash;
    if(NULL == (ptrClient = ClientCreate(_clientPort,_clientIP)))
    {
        return NULL;
    }
    if(NULL == (ptrGroupVector = VectorCreate(VECTOR_SIZE, BLOCK_SIZE)))
    {
        ClientDestroy(&ptrClient);
        return NULL;
    }
    if (NULL == (ptrPidHash = HashMap_Create(NUM_OF_GROPS, ClientMngHashFunction, ClientMngEqualityFunction)))
    {
        free (ptrClient);
        VectorDestroy(&ptrGroupVector, NULL);
        ClientDestroy(&ptrClient);
        return NULL;
    }
    if(NULL == (ptrClientMng = (ClientManager*)malloc(sizeof(ClientManager))))
	{
		ClientDestroy(&ptrClient);
        VectorDestroy(&ptrGroupVector, NULL);
        HashMap_Destroy(&ptrPidHash, NULL, NULL);
        return NULL;
	}
    ptrClientMng -> m_client = ptrClient;
    ptrClientMng -> m_groups = ptrGroupVector;
    ptrClientMng -> m_pid = ptrPidHash;
    return ptrClientMng;
}

void ClientManagerDestroy(ClientManager** _pClientMng)
{
    if (NULL == _pClientMng || NULL == *_pClientMng )
	{
		return;
	}
    VectorDestroy(&((*_pClientMng) -> m_groups), NULL);
    ClientDestroy(&((*_pClientMng) -> m_client));
    HashMap_Destroy(&((*_pClientMng) -> m_pid), NULL, NULL);
    free(*_pClientMng);
	*_pClientMng = NULL;
}

/***************************************************************************************/

UserDetail* UserDetailCreate(char _userName[], char _password[])
{
    UserDetail* ptrUserDetail;
    char userName[NAME_SIZE];
    char passward[PASS_SIZE];
    if(NULL == (ptrUserDetail = (UserDetail*)malloc(sizeof(UserDetail))))
	{
        return NULL;
	}
    strcpy(ptrUserDetail ->  m_userName, _userName);
    strcpy(ptrUserDetail ->  m_password, _password);
    return ptrUserDetail;
}


void UserDetailDestroy(UserDetail** _pUserDetail)
{
    if (NULL == _pUserDetail || NULL == *_pUserDetail)
	{
		return;
	}
	free(*_pUserDetail);
	*_pUserDetail = NULL;
}
/*********************************************************************/

ClientManagerResult ClientManagerRunRegistration(ClientManager* _clientMng)
{
    int enterClientChice, massegeSize, status,recvMassegeSize, flag = 1;
    char userName[NAME_SIZE];
    char password[PASS_SIZE];
    char buffer[BUFFER_SIZE];
    UserDetail* ptrUserDetail = NULL;
    if (NULL == _clientMng)
	{
		return MANAGER_UNINITIALIZED_ERROR;
	}
    while(flag == 1)
    {
        PrintArtBlue("start.txt");
        enterClientChice = ClientEnterApp(); /*first menu*/
        switch(enterClientChice)
        {
            case 1: /*log in*/
                EnterClientDetails(userName, password);
                if((ptrUserDetail = UserDetailCreate(userName, password)) == NULL)
                {
                    return MANAGER_ALLOCATION_ERROR;
                }
                massegeSize = PackLogIn(ptrUserDetail, buffer, BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    UserDetailDestroy(&ptrUserDetail);
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, massegeSize, &recvMassegeSize ) != CLIENT_SUCCESS)
                {
                    UserDetailDestroy(&ptrUserDetail);
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                status = UnPackLogInReplay(BUFFER_SIZE, buffer);
                if(status != STATUS_SUCCESS)
                {
                    if(status == STATUS_WRONG_USERNAME_OR_PASSWORD)
                    {
                        PrintWorngUserNameOrPassword();
                        PrintArtRed("error.txt") ;
                    }
                    else
                    {
                        UserDetailDestroy(&ptrUserDetail);
                        printf(RED"MANAGER_MESSAGE_ERROR\n"RESET);
                    }
                }
                else
                {
                    _clientMng -> m_userDetail = ptrUserDetail;
                    printf(GREEN"Log in success\n\n"RESET);
                    PrintArtGreen("OK.txt") ;
                    flag = 0;
                }
                break;
                
            case 2: /*sign in- Registration*/
                EnterClientDetails(userName, password);
                if((ptrUserDetail = UserDetailCreate(userName, password)) == NULL)
                {
                    return MANAGER_ALLOCATION_ERROR;
                }
                massegeSize = PackRegistration(ptrUserDetail, buffer, BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    UserDetailDestroy(&ptrUserDetail);
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, massegeSize, &recvMassegeSize) != CLIENT_SUCCESS)
                {
                    UserDetailDestroy(&ptrUserDetail);
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                status = UnPackRegistrationReplay(BUFFER_SIZE, buffer);
                if(status != STATUS_SUCCESS)
                {
                    if(status == STATUS_DUPLICATION)
                    {
                        PrintUserNameDuplication(userName);
                        PrintArtRed("error.txt") ;
                    }
                    else
                    {
                        UserDetailDestroy(&ptrUserDetail);
                        printf(RED"MANAGER_MESSAGE_ERROR\n"RESET);
                    }
                }
                else
                {
                    _clientMng -> m_userDetail = ptrUserDetail;
                    printf(GREEN"Registration success\n\n"RESET);
                    PrintArtGreen("OK.txt") ;
                    flag = 0;
                }
                break;

            case 3: /*exit*/
                PrintArtYellow("exit.txt") ;
                UserDetailDestroy(&ptrUserDetail);
                ClientManagerDestroy(&_clientMng);
                return MANAGER_CLIEN_EXIT;
                break;

            default:
                PrintWorngIndex();
                PrintArtRed("error.txt") ;
        }
    }
    return MANAGER_SUCCESS;
}

ClientManagerResult ClientManagerRunGroup(ClientManager* _clientMng) /*add user name*/
{
    GroupDetail gDetail;
    Window* ptrWindowPid;
    void *pKey, *pValue;
    int enterClientChice, massegeSize,insertPid,findPid, status, recvMassegeSize, flag = 1, answer;
    char buffer[BUFFER_SIZE], groupName[BUFFER_SIZE];
    char* groupNameKey;
    int groupNameLen;
    if (NULL == _clientMng)
	{
		return MANAGER_UNINITIALIZED_ERROR;
	}
    if(ptrWindowPid == NULL)
    {
        ClientManagerDestroy(&_clientMng);
        return MANAGER_ALLOCATION_ERROR;
    }
    while(flag == 1)
    {
       
        enterClientChice = ClientGroupMenu(_clientMng -> m_userDetail -> m_userName); /*secend menu*/
        switch(enterClientChice)
        {
            case 1: /*open group*/
                ptrWindowPid = CreateWindowsPid();
                EnterGroupName(groupName, _clientMng -> m_userDetail -> m_userName);
                groupNameLen = strlen(groupName) +1;
                if(NULL == (groupNameKey = (char*)malloc(groupNameLen*sizeof(char))))
                {
                    WindowDestroy(&ptrWindowPid);
                    ClientManagerDestroy(&_clientMng);
                    return MANAGER_ALLOCATION_ERROR;
                }
                strcpy(groupNameKey, groupName);
                massegeSize = PackOpenGroup(groupNameKey, buffer,  BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, BUFFER_SIZE, &recvMassegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                gDetail = UnPackOpenGroupReplay(massegeSize, buffer);
                if(gDetail.m_status != STATUS_SUCCESS)
                {
                    if(gDetail.m_status == STATUS_DUPLICATION)
                    {
                        PrintDuplicatTag(groupNameKey);
                        PrintArtRed("error.txt") ;
                    }
                    else
                    {
                        printf(RED"OPEN_GROUP_ERROR\n"RESET);
                    }
                }
                else
                {
                    printf(GREEN"Open group success\n\n"RESET);
                    PrintArtGreen("OK.txt") ;
                    *ptrWindowPid = ChatWindowsOpener(gDetail.m_port, gDetail.m_ip, _clientMng -> m_userDetail -> m_userName);
                    if((ptrWindowPid->m_firstWindow) == 0 || (ptrWindowPid->m_secendWindow) == 0)
                    {
                        printf(RED"OPEN_CHAT_ERROR\n"RESET);
                    }
                    if((insertPid = InsertChatPid(_clientMng, groupNameKey, ptrWindowPid)) != MANAGER_SUCCESS)
                    {
                        printf(RED"SEVE_CHAT_ERROR\n"RESET);
                    }
                }
                break;
            case 2: /*join group*/
                ptrWindowPid = CreateWindowsPid();
                answer = JoinGroupAnswer(_clientMng -> m_userDetail -> m_userName);
                if(answer == 2)
                {
                    massegeSize = PackListGroups(buffer,  BUFFER_SIZE);
                    if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                    {
                        printf(RED"MANAGER_SEND_ERROR\n"RESET);
                    }
                    if(ClientNetRunRecv(_clientMng -> m_client, buffer, BUFFER_SIZE, &recvMassegeSize) != CLIENT_SUCCESS)
                    {
                        printf(RED"MANAGER_RECV_ERROR\n"RESET);
                    }
                    UnPackListGroupsReplay(_clientMng -> m_groups, recvMassegeSize, buffer);
                    PrintGroups(_clientMng);
                }
                EnterGroupName(groupName, _clientMng -> m_userDetail -> m_userName);
                groupNameLen = strlen(groupName) + 1;
                if(NULL == (groupNameKey = (char*)malloc(groupNameLen*sizeof(char))))
                {
                    WindowDestroy(&ptrWindowPid);
                    ClientManagerDestroy(&_clientMng);
                    return MANAGER_ALLOCATION_ERROR;
                }
                strcpy(groupNameKey, groupName);
                massegeSize = PackJoinGroup(groupNameKey, buffer,  BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, BUFFER_SIZE, &recvMassegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                gDetail = UPackJoinGroupReplay(recvMassegeSize, buffer);
                if(gDetail.m_status != STATUS_SUCCESS)
                {
                    if(gDetail.m_status == STATUS_NAME_GROUP_NOT_FOUND)
                    {
                        PrintNameNotFound(groupNameKey);
                        PrintArtRed("error.txt");
                    }
                    else
                    {
                        printf(RED"JOIN_GROUP_ERROR\n"RESET);
                    }
                }
                else
                {
                    printf(GREEN"Join group success\n\n"RESET);
                    PrintArtGreen("OK.txt") ;
                    *ptrWindowPid = ChatWindowsOpener(gDetail.m_port, gDetail.m_ip, _clientMng -> m_userDetail -> m_userName);
                    if((ptrWindowPid->m_firstWindow) == 0 || (ptrWindowPid->m_secendWindow) == 0)
                    {
                        printf(RED"SAVE_CHAT_ERROR\n"RESET);
                    }
                    *ptrWindowPid = ChatWindowsOpener(gDetail.m_port, gDetail.m_ip, _clientMng -> m_userDetail -> m_userName);
                    if((insertPid = InsertChatPid(_clientMng, groupNameKey, ptrWindowPid)) != MANAGER_SUCCESS)
                    {
                        printf(RED"SEVE_CHAT_ERROR\n"RESET);
                    }
                }
                break;
            case 3: /*leave group*/
                printf(CYAN"\n%s, please enter the name of the group you want to leave\n"RESET, _clientMng -> m_userDetail -> m_userName);
                EnterGroupName(groupName, _clientMng -> m_userDetail -> m_userName);
                massegeSize = PackLeaveGroup(groupName, buffer,  BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, massegeSize, &recvMassegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                status = UnPackLeaveGroupReplay(recvMassegeSize, buffer);
                if(status != STATUS_SUCCESS)
                {
                    if(status == STATUS_NAME_GROUP_NOT_FOUND)
                    {
                        PrintNameNotFound(groupName);
                        PrintArtRed("error.txt") ;
                    }
                     else
                    {
                        printf(RED"LEAVE_GROUP_ERROR\n"RESET);
                    }
                }
                else
                {
                    if(RemoveChatPid(_clientMng, groupName, &pKey, &pValue) != MAP_SUCCESS)
                    {
                        printf(RED"REMOVE_GROUP_ERROR\n"RESET);
                    }
                    CloseWindows(*(Window*)pValue);
                    printf(GREEN"Leave group success\n"RESET);
                    PrintArtGreen("OK.txt") ; 
                }
                break;
            case 4: /*log out*/
                massegeSize = PackLogOut(buffer,  BUFFER_SIZE);
                if(ClientNetRunSend(_clientMng -> m_client, buffer, massegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_SEND_ERROR\n"RESET);
                }
                if(ClientNetRunRecv(_clientMng -> m_client, buffer, massegeSize, &recvMassegeSize) != CLIENT_SUCCESS)
                {
                    printf(RED"MANAGER_RECV_ERROR\n"RESET);
                }
                status = UnPackLogOutReplay(recvMassegeSize, buffer);
                if(status != STATUS_SUCCESS)
                {
                    printf(RED"LOG_OUT_ERROR\n"RESET);
                }
                else
                {
                    printf(GREEN"Log out success\n"RESET);
                    PrintArtGreen("OK.txt") ;
                }
                free(groupNameKey);
                /*CloseWindows(*ptrWindowPid);*/
                WindowDestroy(&ptrWindowPid);
                ClientManagerDestroy(&_clientMng);
                return LOG_OUT;
                break;
            case 5: /*exit*/
                PrintArtYellow("exit.txt") ;
                free(groupNameKey);
                WindowDestroy(&ptrWindowPid);
                ClientManagerDestroy(&_clientMng);
                return MANAGER_CLIEN_EXIT;
                break;
            default:
                PrintWorngIndex();
                PrintArtRed("error.txt") ;
        }
    }
}

static size_t ClientMngHashFunction(void* _groupName) /*manipulation to get uniq index from a char*/
{
    int i;
    char* nameOfGroup = (char*) _groupName;
    size_t index = 0;
    int size = strlen(nameOfGroup);
    for (i = 0; i < size; i++) 
    {
        index += nameOfGroup[i] * i;
    }
    return index;
}

static int ClientMngEqualityFunction(void* _firstKey, void* _secondKey) /*do strcmpr*/
{
    if (0 == strcmp((char*)_firstKey, (char*)_secondKey))
    {
        return TRUE;
    }
    return FALSE;
}

ClientManagerResult InsertChatPid(ClientManager* _clientMng, char* _groupName, void* _windowPid)
{
    int insertResult;
    if (NULL == _clientMng || NULL == _groupName || NULL == _windowPid)
	{
		return MANAGER_UNINITIALIZED_ERROR;
	}
    insertResult = HashMap_Insert(_clientMng -> m_pid , _groupName, _windowPid);
    if(insertResult != MAP_SUCCESS)
    {
        return SAVE_CHAT_ERROR;
    }
    return MANAGER_SUCCESS;
}

ClientManagerResult FindChatPid(ClientManager* _clientMng, char* _groupName, void** _pValue)
{
    int findResult;
    if (NULL == _clientMng || NULL == _groupName || NULL == _pValue)
	{
		return MANAGER_UNINITIALIZED_ERROR;
	}
    findResult = HashMap_Find(_clientMng -> m_pid, _groupName, _pValue);
    if(findResult != MAP_SUCCESS)
    {
        return FIND_CHAT_ERROR;
    }
    return MANAGER_SUCCESS;
}

ClientManagerResult RemoveChatPid(ClientManager* _clientMng, char* _groupName,void** _pKey, void** _pValue)
{
    int findResult;
    if (NULL == _clientMng || NULL == _groupName || NULL == _pValue || NULL == _pKey)
	{
		return MANAGER_UNINITIALIZED_ERROR;
	}
    findResult = HashMap_Remove(_clientMng -> m_pid, _groupName, _pKey, _pValue);
    if(findResult != MAP_SUCCESS)
    {
        return REMOVE_CHAT_ERROR;
    }
    return MANAGER_SUCCESS;
}
static void PrintGroups(ClientManager* _clientMng)
{
    void* nameGroup;
    VectorForEach(_clientMng -> m_groups, VectorPrintChar, NULL);

    while(VectorSize(_clientMng -> m_groups) > 0)
    {
        if(VectorRemove(_clientMng -> m_groups, &nameGroup) != VECTOR_SUCCESS)
        {
            return; 
        }
    }
}

static int VectorPrintChar(void* _element, size_t _index, void* _context)
{
	if(NULL == _element )
	{
		return FALSE;
	}
	printf("%ld. %s\n", _index+1, (char*)_element);
	return TRUE;
}

size_t ChakNumOfGroups(ClientManager* _clientMng)
{
    size_t nOfGroups;
    if (NULL == _clientMng)
    {
        return 0;
    }
    nOfGroups = HashMap_Size(_clientMng -> m_pid);
    return nOfGroups;
}

