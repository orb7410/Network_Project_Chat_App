#include "groupManager.h"
#include "serverManager.h"
#include "userManager.h"
#include "protocol.h"
#include "vector.h"
#include "server.h"
#include "list.h" /*DLList funcions decleration*/
#include "listItr.h" /*ItrList funcions decleration*/
#include "listitrfun.h" /*ItrListForEch funcions decleration*/
#include <stdlib.h>
#define MAX_GROUPS 100

struct ServerManager
{
    GroupManager* m_groupManager;
    UserManager*  m_userManager;
    Server*       m_serverNet;
    Vector*       m_groupVector;
};

void* ServerMangerGotMessage(int* _clientId, char* _buffer, int _massageSize, void* _context);
static int CheckStatusInRegistration(int _status, int* _clientId); /*in registration case*/
static int CheckStatusInLogIn(int _status, int* _clientId); /*in login case*/
static int CheckStatusInOpenGroup(int _status, int* _clientId, char* _port, char* _ip); /*in open group case*/
static int CheckStatusInJoinGroup(int _status, int* _clientId, char* _port, char* _ip); /*in join group case*/
static int CheckStatusInLeaveGroup(int _status, int* _clientId); /*in leave group case*/
static void NamesGroupDestroyFuncForVec(void* _groupName);
static int CheckStatusInLogOut(int _status, int* _clientId); /*in  log out case*/
int GroupManagerActionFuncleaveGroup(void* _groupName, void* _groupManager);

ServerManager* CreateServerManger()
{
    ServerManager* newServerManager;
    Server* newServer;
    UserManager* newUserManager;
    GroupManager* newGroupManager;
    Vector* pVector;

    if (NULL == (pVector = VectorCreate(100, 1)))
	{
        return NULL;
	}
    if (NULL == (newServerManager = (ServerManager*)malloc(sizeof(ServerManager))))
    {
        VectorDestroy(&pVector, NULL);        
        return NULL;
    }
    if (NULL == (newServer = ServerCreate(NULL,ServerMangerGotMessage, NULL, NULL, newServerManager)))
    {
        VectorDestroy(&pVector, NULL);
        free(newServerManager);
        return NULL;
    }
    if (NULL == (newUserManager =UserManagerCreate (MAX_USERS)))
    {
        VectorDestroy(&pVector, NULL);
        ServerDestroy (&newServer);
        free(newServerManager);
        return NULL;
    }
    if (NULL == (newGroupManager = GroupManagerCreate(MAX_GROUPS)))
    {
        VectorDestroy(&pVector, NULL);    
        ServerDestroy (&newServer);
        UserManagerDestroy(&newUserManager);
        free(newServerManager);
        return NULL;
    }
    newServerManager -> m_userManager = newUserManager;
    newServerManager -> m_serverNet = newServer;
    newServerManager -> m_groupManager = newGroupManager;
    newServerManager -> m_groupVector = pVector;

    return newServerManager;
}
/*......................................*/
void ServerMangerDestroy( ServerManager** _serverManager)
{
    if (NULL == _serverManager || NULL == *_serverManager)
    {
        return; 
    }
    VectorDestroy(&((*_serverManager)-> m_groupVector), NamesGroupDestroyFuncForVec);
    ServerDestroy (&((*_serverManager)-> m_serverNet));
    GroupManagerDestroy(&((*_serverManager)-> m_groupManager));    
    UserManagerDestroy (&((*_serverManager)-> m_userManager));
    free((*_serverManager));
    *_serverManager = NULL;
}
/*..............................................*/
ServerMenagerResult ServerMenagerRun(ServerManager* _serverManager)
{
    int counter = 0, result;
    char buffer[BUFFER_SIZE];
    if(_serverManager == NULL)
    {
        return SERVER_MENAGER_UNINITIALIZED_ERROR;
    }  
    result = ServerRun(_serverManager->m_serverNet);
    if(result == SERVER_FAIL)
    {
        return SERVER_MENAGER_FAIL;
    }
    if(result == SERVER_UNINITIALIZED_ERROR)
    {
        return SERVER_MENAGER_UNINITIALIZED_ERROR;
    }
    return SERVER_MENAGER_SUCCESS;
}

/*.................................................*/
void* ServerMangerGotMessage(int* _clientId, char* _buffer, int _massageSize, void* _context)
{  
    ServerManager* newServerManager = (ServerManager*)_context;
    List* newList;
    ListItr end,begin;
    int bufferSize,tag, status, lengthPack;
    UserDetail pUserDetail;
    GroupDetail pGroupDetail;
    char addressIP[IP_SIZE], port[PORT_SIZE],nameGroup[GROUP_NAME_SIZE], newBuffer[BUFFER_SIZE];

    if(_buffer == NULL || _clientId == NULL || _massageSize <= 0 || newServerManager == NULL)
    {
        return NULL;
    }
    bufferSize = sizeof(_buffer);
    tag = RetrieveTag(_buffer);
    switch (tag)
    {
        case REGISTR_TAG:/*registration*/
            pUserDetail = UnpackRegistration(_buffer, _massageSize);
            status = UserManagerAddNewUser(newServerManager->m_userManager, pUserDetail.m_userName, pUserDetail.m_password, *_clientId);
            if(CheckStatusInRegistration(status , _clientId) != SERVER_MENAGER_SUCCESS) /*send if fail or success */
            {
                return NULL; /*if ServerSend fail*/
            }
            break;
        case LOG_IN_TAG:/*login*/
            pUserDetail = UnpackLogIn(_buffer, _massageSize);
            status = UserManagerLogin(newServerManager->m_userManager, pUserDetail.m_userName, pUserDetail.m_password, *_clientId);
            if(CheckStatusInLogIn(status, _clientId) != SERVER_MENAGER_SUCCESS) 
            {
                return NULL; /*if ServerSend fail*/
            }
            break; 
            
        case OPEN_GROUP_TAG:/*open group*/
            lengthPack = UnPackOpenGroup(_buffer, bufferSize, nameGroup);
            status = GroupManagerOpenGroup(newServerManager->m_groupManager, nameGroup, addressIP, port);
            if(CheckStatusInOpenGroup(status, _clientId, port, addressIP) != SERVER_MENAGER_SUCCESS)
            {
                return NULL;
            }
            /*??????user name???*/

            if(UserManagerInsertGroupToUser(newServerManager->m_userManager, pUserDetail.m_userName, nameGroup) != USERMANAGER_SUCCESS)
            {
                return NULL;
            }
            break;           
        case JOIN_GROUP_TAG:/*join to group - in case the client know the groups name*/
            lengthPack = UnPackJoinGroup(_buffer, bufferSize, nameGroup);
            status = GroupManagerJoinToGroup(newServerManager->m_groupManager, nameGroup, addressIP, port);
            if(CheckStatusInJoinGroup(status, _clientId, port, addressIP) != SERVER_MENAGER_SUCCESS)
            {
                return NULL;
            }
            if(UserManagerInsertGroupToUser(newServerManager->m_userManager, pUserDetail.m_userName, nameGroup) != USERMANAGER_SUCCESS)
            {
                return NULL;
            }
            break;  
        case PRESENT_ACTIVE_GROUPS_LIST_TAG:/*request to see the list of active groups*/
            lengthPack = UnPackListGroups(_buffer, bufferSize);
            GroupManagerGiveListOfGroups(newServerManager-> m_groupManager, newServerManager->m_groupVector);
            lengthPack = PackListGroupsReplay(newServerManager->m_groupVector, BUFFER_SIZE, newBuffer);
            if(ServerSend(*_clientId, newBuffer, lengthPack) < 0)
            {
                return NULL;
            } 
        case LIVE_GROUP_TAG:/*leave group*/
            lengthPack = UnPackLeaveGroup(_buffer, bufferSize, nameGroup);
            status = GroupManagerleaveGroup(newServerManager-> m_groupManager, nameGroup);
            if(CheckStatusInLeaveGroup(status, _clientId) != SERVER_MENAGER_SUCCESS)
            {
                return NULL;
            }
            if(UserManagerLeaveGroup(newServerManager->m_userManager,pUserDetail.m_userName , nameGroup) != USERMANAGER_SUCCESS)
            {
                return NULL;
            }
            break;
            
        case LOG_OUT_TAG:/*log out*/
            lengthPack = UnPackLogOut( _buffer,bufferSize); 
            if(NULL == (newList = UserManagerGetGroupsList(newServerManager->m_userManager,pUserDetail.m_userName)))
            {
                return NULL;
            }
            begin = ListItrBegin(newList);
            end = ListItrEnd(newList);
            ListItrForEach(begin, end, GroupManagerActionFuncleaveGroup, newServerManager-> m_groupManager);
            status = UserManagerCleanUserGroupsForLogOut(newServerManager->m_userManager,pUserDetail.m_userName);
            if(CheckStatusInLogOut(status, _clientId) != SERVER_MENAGER_SUCCESS)
            {
                return NULL;
            }
            break;
                      
    }
    return newServerManager;
}



/*......HELP FUNC........*/
int GroupManagerActionFuncleaveGroup(void* _groupName, void* _groupManager)
{
    GroupManagerleaveGroup((GroupManager*) _groupManager, (char*) _groupName);
    return 1;
}

static void NamesGroupDestroyFuncForVec(void* _groupName)
{
    free(_groupName);
}

static int CheckStatusInRegistration(int _status, int* _clientId)/*registration*/
{
    int lengthPackMessage;
    char newBuffer[BUFFER_SIZE];
    if(_status == USERMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        lengthPackMessage = PackRegistrationReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer , lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }
    }
    if(_status == USERMANAGER_FAIL)
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackRegistrationReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }
    }
    if(_status == USERMANAGER_DUPLICATION)
    {
        _status = STATUS_DUPLICATION;
        lengthPackMessage = PackRegistrationReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }    
    } 
    return SERVER_MENAGER_SUCCESS;
}

/*.............................................*/
static int CheckStatusInLogIn(int _status, int* _clientId)/*login*/
{
    int lengthPackMessage;
    char newBuffer[BUFFER_SIZE];
    if(_status == USERMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        /*add do list user*/
        lengthPackMessage = PackLogInReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }   
    } 
    if(_status == USERMANAGER_FAIL)
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackLogInReplay(_status,newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    } 
    if(_status == USERMANAGER_USER_NOT_FOUND || _status == USERMANAGER_WRONG_PASSWORD)
    {
        _status = STATUS_WRONG_USERNAME_OR_PASSWORD;
        lengthPackMessage = PackLogInReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    }
    return SERVER_MENAGER_SUCCESS; 
}
/*.............................................*/
static int CheckStatusInOpenGroup(int _status, int* _clientId, char* _port, char* _ip) /*open group case*/
{
    int lengthPackMessage;
    GroupDetail pGroupDetail;
    char newBuffer[BUFFER_SIZE];
    
    if(_status ==  GROUPMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        lengthPackMessage = PackOpenGroupReplay(_port, _ip, _status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }   
    } 
    if(_status ==  GROUPMANAGER_FAIL)
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackOpenGroupReplay(_port, _ip,_status,newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    } 
    if(_status ==  GROUPMANAGER_DUPLICATION)
    {
        _status = STATUS_DUPLICATION;
        lengthPackMessage = PackOpenGroupReplay(_port, _ip,_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    }
    return SERVER_MENAGER_SUCCESS;  
}

/*.............................................*/
static int CheckStatusInJoinGroup(int _status, int* _clientId, char* _port, char* _ip) /*in join group case*/
{
    int lengthPackMessage;
    GroupDetail pGroupDetail;
    char newBuffer[BUFFER_SIZE];
    
    if(_status ==  GROUPMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        lengthPackMessage = PackJoinGroupReplay(_port, _ip, _status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }   
    } 
    if(_status == GROUPMANAGER_FAIL || _status ==  GROUPMANAGER_NOT_INITIALIZED)
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackJoinGroupReplay(_port, _ip,_status,newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    } 
    if(_status ==   GROUPMANAGER_NOT_FOUND)
    {
        _status = STATUS_NAME_GROUP_NOT_FOUND;
        lengthPackMessage = PackJoinGroupReplay(_port, _ip,_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    }
    return SERVER_MENAGER_SUCCESS;  
}
/*...............................................*/
static int CheckStatusInLeaveGroup(int _status, int* _clientId) /*in leave group case*/
{
    int lengthPackMessage;
    char newBuffer[BUFFER_SIZE];
    
    if(_status ==  GROUPMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        lengthPackMessage = PackLeaveGroupReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }   
    } 
    if(_status == GROUPMANAGER_FAIL || _status ==  GROUPMANAGER_NOT_INITIALIZED)
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackLeaveGroupReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    } 
    if(_status == GROUPMANAGER_NOT_FOUND)
    {
        _status = STATUS_NAME_GROUP_NOT_FOUND;
        lengthPackMessage = PackLeaveGroupReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    }
    return SERVER_MENAGER_SUCCESS;  
}
/*...............................................*/
static int CheckStatusInLogOut(int _status, int* _clientId) /*in  log out case*/
{
    int lengthPackMessage;
    char newBuffer[BUFFER_SIZE];
    
    if(_status ==  USERMANAGER_SUCCESS)
    {
        _status = STATUS_SUCCESS;
        lengthPackMessage = PackLogOutReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }   
    } 
    else    
    {
        _status = STATUS_FAIL;
        lengthPackMessage = PackLogOutReplay(_status, newBuffer);
        if(ServerSend(*_clientId, newBuffer, lengthPackMessage) < 0)
        {
            return SERVER_MENAGER_SEND_FAIL;
        }     
    } 
    return SERVER_MENAGER_SUCCESS;  
}

UserManager* GetUserManager(ServerManager *_serverManager)
{
	return _serverManager -> m_userManager;
}
Server* GetServerNet(ServerManager *_serverManager)
{
	return _serverManager -> m_serverNet;
}
Vector* GetGroupVector(ServerManager *_serverManager)
{
	return _serverManager -> m_groupVector;
}
GroupManager* GetGroupManager(ServerManager *_serverManager)
{
	return _serverManager -> m_groupManager;
}
