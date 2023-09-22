#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

int RetrieveTag(char* _buffer)
{
    int tag = _buffer[0];
    return tag;
}

/*..............REGISTRATION...............*/
int PackRegistration(UserDetail* _userDetail, char* _buffer, int _bufferSize)
{
    int i = 0,lengthPass,lengthUserName,totalLength;
    if(NULL == _userDetail || 0 >= _bufferSize)
    {
        return 0;
    }
    lengthPass = strlen(_userDetail-> m_password);
    lengthUserName = strlen(_userDetail-> m_userName);
    totalLength = lengthPass + lengthUserName + 2;
    /*copy the tag*/
    _buffer[i] = REGISTR_TAG;
    i += 1;
    /*copy the total length*/
    _buffer[i] = totalLength;
    i += 1;

    /*copy the user name and length */
    _buffer[i] = lengthUserName;
    strcpy(&_buffer[i+1], _userDetail->m_userName);
    i = i + lengthUserName + 1;

    /*copy the pass and length */
    _buffer[i] = lengthPass;
    strcpy(&_buffer[i+1], _userDetail->m_password);

    return(totalLength + 2);
}
/************************/
UserDetail UnpackRegistration(char* _buffer, int _bufferSize)
{
    UserDetail pUserDetail;
    int i = 0;
    int lengthPass, lengthUserName, totalLength;

    /*retrieve the tag*/ 
    if(_buffer[i] != REGISTR_TAG || 0 >= _bufferSize)
    {
        pUserDetail.m_userName[i] = '\0';
        pUserDetail.m_password[i] = '\0';
        return pUserDetail;
    }    
    i += 1;
    /*retrieve the total length*/
    totalLength = _buffer[i];
    i += 1;
    /*retrieve the user name and length */
    lengthUserName = _buffer[i];
    strncpy(pUserDetail.m_userName, &_buffer[i+1], lengthUserName);
    pUserDetail.m_userName[lengthUserName] = '\0';
    i = i + 1 + lengthUserName;
    /*retrieve the pass and length */
    lengthPass = _buffer[i];
    strncpy(pUserDetail.m_password, &_buffer[i+1], lengthPass);
    pUserDetail.m_password[lengthPass] = '\0';

    return pUserDetail;
}
/************************/
int PackRegistrationReplay(int _status, char* _buffer) /*empty buffer*/
{
    int i = 0;
    int statusLength = 1;  
    int totalLength = statusLength;
    /*copy the tag*/
    _buffer[i] = REGISTR_REPLAY_TAG;
    i += 1;    
    _buffer[i] = statusLength;
    i += 1;
    _buffer[i] = _status;
    return totalLength + 2;
}
/************************/
int UnPackRegistrationReplay(int _bufferSize, char* _buffer) 
{
    int i = 0;
    int statusLength,totalLength;
    if(_buffer[i] != REGISTR_REPLAY_TAG || _bufferSize <= 0)
    {
        return -1;
    } 
    i += 1;
    _buffer[i] = statusLength;
    i += 1;
    return _buffer[i]; /*return status*/
}
/*..................LOG IN.................*/

int PackLogIn(UserDetail* _userDetail, char* _buffer,  int _bufferSize)
{
    int i = 0,lengthPass,lengthUserName,totalLength;
    if(NULL == _userDetail || 0 >= _bufferSize)
    {
        return 0;
    }
    lengthPass = strlen(_userDetail-> m_password);
    lengthUserName = strlen(_userDetail-> m_userName);
    totalLength = lengthPass + lengthUserName + 2;
    /*copy the tag*/
    _buffer[i] = LOG_IN_TAG;
    i += 1;
    /*copy the total length*/
    _buffer[i] = totalLength;
    i += 1;

    /*copy the user name and length */
    _buffer[i] = lengthUserName;
    strcpy(&_buffer[i+1], _userDetail->m_userName);
    i = i + 1 + lengthUserName;

    /*copy the pass and length */
    _buffer[i] = lengthPass;
    strcpy(&_buffer[i+1], _userDetail->m_password);

    return(totalLength + 2);
}
/************************/
UserDetail UnpackLogIn(char* _buffer, int _bufferSize)
{
    UserDetail pUserDetail;
    int i = 0;
    int lengthPass, lengthUserName, totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != LOG_IN_TAG || _bufferSize <= 0)
    {
        pUserDetail.m_userName[i] = '\0';
        pUserDetail.m_password[i] = '\0';
        return pUserDetail;    
    }
    i += 1;
    /*retrieve the total length*/
    totalLength = _buffer[i];
    i += 1;

    /*retrieve the user name and length */
    lengthUserName = _buffer[i];
    strncpy(pUserDetail.m_userName, &_buffer[i+1], lengthUserName);
    pUserDetail.m_userName[lengthUserName] = '\0';
    i = i + 1 + lengthUserName;

    /*retrieve the pass and length */
    lengthPass = _buffer[i];
    strncpy(pUserDetail.m_password, &_buffer[i+1], lengthPass);
    pUserDetail.m_password[lengthPass] = '\0';

    return pUserDetail;
}
/************************/
int PackLogInReplay(int _status, char* _buffer)
{
    int i = 0;
    int totalLength = 3,length = 1;
    /*copy the tag*/
    _buffer[i] = LOG_IN_REPLAY_TAG;
    i += 1;    
    _buffer[i] = length;
    i += 1;
    _buffer[i] = _status;
    return totalLength;
}

int UnPackLogInReplay(int _bufferSize, char* _buffer) 
{
    int i = 0, statusLength;
    if(_buffer[i] != LOG_IN_REPLAY_TAG || _bufferSize <= 0)
    {
        return -1;
    } 
    i += 1;
    _buffer[i] = statusLength;
    i += 1;
    return _buffer[i]; /*return status*/
}

/*...................OPEN GROUP......................*/
int PackOpenGroup(char* _groupName, char* _buffer,  int _bufferSize) /*empty buffer*/
{
    int i = 0,lengthGroupName,totalLength;
    if(NULL == _groupName || 0 >= _bufferSize)
    {
        return 0;
    }
    lengthGroupName = strlen(_groupName);
    totalLength = lengthGroupName + 1;
    /*copy the tag*/
    _buffer[i] = OPEN_GROUP_TAG;
    i += 1;
    _buffer[i] = totalLength;
    i += 1;
    /*copy the group name length*/
    _buffer[i] = lengthGroupName;
    i += 1;

    /*copy the group name */
    strncpy(&_buffer[i], _groupName,lengthGroupName);

    return totalLength + 2;
}

int UnPackOpenGroup(char* _buffer, int _bufferSize, char* _groupName)
{
    int i = 0;
    int groupNameLength, totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != OPEN_GROUP_TAG || _bufferSize <= 0)
    {
        return 0;   
    }
    i += 1;
    totalLength = _buffer[i];
    i += 1;
    /*retrieve the group length*/
    groupNameLength = _buffer[i];
    i += 1;
    /*retrieve the group name  */
    strncpy(_groupName, &_buffer[i], groupNameLength);
    _groupName[groupNameLength] = '\0';

    return totalLength + 2;
}

int PackOpenGroupReplay(char* _port, char* _ip ,int _status, char* _buffer)
{
    int i = 0,lengthPort,lengthIp ,totalLength,statusLength = 1;
    if(NULL == _ip || NULL == _port)
    {
        return 0;
    }
    if(_status == STATUS_DUPLICATION || _status == STATUS_FAIL)
    {
        totalLength = statusLength + 1;
        /*copy the tag*/
        _buffer[i] = OPEN_GROUP_REPLAY_TAG;
        i += 1;  
        _buffer[i] = totalLength;
        i += 1;
        _buffer[i] = statusLength;
        i += 1;
        _buffer[i] = _status;
        return totalLength + 2; 
    }
    lengthPort = strlen(_port);
    lengthIp = strlen(_ip);
    /*else if status seccess*/
    totalLength = lengthIp + lengthPort + statusLength + 3;
    _buffer[i] = OPEN_GROUP_REPLAY_TAG;
    i += 1;   
    _buffer[i] = totalLength; 
    i += 1;
    /*copy the length of status and status*/
    _buffer[i] = statusLength;
    i += 1;
    _buffer[i] = _status;
    i += 1;
    /*copy the port and lengthPort */
    _buffer[i] = lengthPort;
    strncpy(&_buffer[i+1], _port,lengthPort);
    i = i + 1 + lengthPort;
    /*copy the ip and lengthIP */
    _buffer[i] = lengthIp;
    strncpy(&_buffer[i+1], _ip,lengthIp);


    return totalLength + 2;
}

GroupDetail UnPackOpenGroupReplay(int _bufferSize, char* _buffer) 
{
    GroupDetail pGroupDetail;
    int lengthPort, lengthIp, totalLength,statusLength;
    int i = 0;
    /*check the tag first*/
    if(_buffer[i] != OPEN_GROUP_REPLAY_TAG || _bufferSize <= 0)
    {
        pGroupDetail.m_ip[i] = '\0';
        pGroupDetail.m_port[i] = '\0';
        pGroupDetail.m_status = -1;
        return pGroupDetail;    
    }
    i += 1;
    totalLength =_buffer[i] ;
    i += 1;
    statusLength = _buffer[i];
    i += 1;
    pGroupDetail.m_status = _buffer[i] ;
    if( pGroupDetail.m_status == STATUS_DUPLICATION || pGroupDetail.m_status == STATUS_FAIL)
    {
        pGroupDetail.m_ip[i] = '\0';
        pGroupDetail.m_port[i] = '\0';
        return pGroupDetail;    
    }
    i += 1;
    /*retrieve the port and lengthPort */
    lengthPort = _buffer[i];
    strncpy(pGroupDetail.m_port, &_buffer[i+1], lengthPort);
    pGroupDetail.m_port[lengthPort] = '\0';
    i = i + 1 + lengthPort;
    /*retrieve the ip and lengthIP */
    lengthIp = _buffer[i];
    strncpy(pGroupDetail.m_ip, &_buffer[i+1], lengthIp);
    pGroupDetail.m_ip[lengthIp] = '\0';
    return pGroupDetail;
}

/*................... JOIN GROUP..................*/

int PackJoinGroup(char* _groupName, char* _buffer,  int _bufferSize) /*empty buffer*/
{
    int i = 0,lengthGroupName,totalLength;
    if(NULL == _groupName || 0 >= _bufferSize)
    {
        return 0;
    }
    lengthGroupName = strlen(_groupName);
    totalLength = lengthGroupName + 1;

    /*copy the tag*/
    _buffer[i] = JOIN_GROUP_TAG;
    i += 1;
    /*copy the group name length*/
    _buffer[i] = totalLength;
    i += 1;

    /*copy the group name and length of name group */
    _buffer[i] = lengthGroupName;
    strncpy(&_buffer[i+1], _groupName,lengthGroupName );

    return totalLength + 2;
}

int UnPackJoinGroup(char* _buffer, int _bufferSize, char* _groupName)
{
    int i = 0;
    int groupNameLength, totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != JOIN_GROUP_TAG || _bufferSize <= 0)
    { 
        return 0;    
    }
    i += 1;
    /*retrieve the group length*/
    totalLength = _buffer[i];
    i += 1;
    groupNameLength = _buffer[i];
    i += 1;
    /*retrieve the group name  */
    strncpy(_groupName, &_buffer[i], groupNameLength);
    _groupName[groupNameLength] = '\0';
    return totalLength + 2;
}

int PackJoinGroupReplay(char* _port, char* _ip ,int _status, char* _buffer)
{
    int i = 0,lengthPort,lengthIp ,totalLength,statusLength = 1;
    if(NULL == _ip || NULL == _port)
    {
        return 0;
    }
    if(_status == STATUS_NAME_GROUP_NOT_FOUND || _status == STATUS_FAIL)
    {
        totalLength = statusLength + 1;
        /*copy the tag*/
        _buffer[i] = JOIN_GROUP_REPLAY_TAG;
        i += 1;  
        _buffer[i] = totalLength;
        i += 1;
        _buffer[i] = statusLength;
        i += 1;
        _buffer[i] = _status;
        return totalLength + 2; 
    }
    lengthPort = strlen(_port);
    lengthIp = strlen(_ip);
    /*else in status seccess*/
    totalLength = lengthIp + lengthPort + statusLength + 3;
    _buffer[i] = JOIN_GROUP_REPLAY_TAG;
    i += 1;   
    _buffer[i] = totalLength;
    i += 1; 
    /*copy the length of status and status*/
    _buffer[i] = statusLength;
    i += 1;
    _buffer[i] = _status;
    i += 1;
    /*copy the port and lengthPort */
    _buffer[i] = lengthPort;
    strncpy(&_buffer[i+1], _port,lengthPort);
    i = i + 1 + lengthPort;
    /*copy the ip and lengthIP */
    _buffer[i] = lengthIp;
    strncpy(&_buffer[i+1], _ip,lengthIp);

    return totalLength +2 ;
}

GroupDetail UPackJoinGroupReplay(int _bufferSize, char* _buffer) 
{
    GroupDetail pGroupDetail;
    int lengthPort, lengthIp, totalLength, statusLength;
    int i = 0;
    /*check the tag first*/
    if(_buffer[i] != JOIN_GROUP_REPLAY_TAG || _bufferSize <= 0)
    {
        pGroupDetail.m_ip[i] = '\0';
        pGroupDetail.m_port[i] = '\0';
        pGroupDetail.m_status = -1;
        return pGroupDetail;    
    }
    i += 1;
    totalLength = _buffer[i];
    i += 1;
    statusLength = _buffer[i];
    i += 1;
    pGroupDetail.m_status = _buffer[i];
    if(pGroupDetail.m_status == STATUS_NAME_GROUP_NOT_FOUND || pGroupDetail.m_status == STATUS_FAIL)
    {
        pGroupDetail.m_ip[i] = '\0';
        pGroupDetail.m_port[i] = '\0';
        return pGroupDetail;  
    }
    i += 1;
    /*retrieve the port and lengthPort */
    lengthPort = _buffer[i];
    strncpy(pGroupDetail.m_port, &_buffer[i+1], lengthPort);
    pGroupDetail.m_port[lengthPort] = '\0';
    i = i + 1 + lengthPort;
    /*retrieve the ip and lengthIP */
    lengthIp = _buffer[i];
    strncpy(pGroupDetail.m_ip, &_buffer[i+1], lengthIp);
    pGroupDetail.m_ip[lengthIp] = '\0';

    return pGroupDetail;
}

/*................SHOW LIST GROUPS.................*/

int PackListGroups(char* _buffer,  int _bufferSize) 
{
    int i = 0;
    int totalLength = 1;
    if(_bufferSize <= 0)
    {
        return 0;
    }
    /*copy the tag*/
    _buffer[i] = PRESENT_ACTIVE_GROUPS_LIST_TAG;
    i += 1;
    _buffer[i] = totalLength;
    i += 1;
    _buffer[i] = '\0';

    return totalLength + 2;
}

int UnPackListGroups(char* _buffer,  int _bufferSize) 
{
    int i = 0;
    int totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != PRESENT_ACTIVE_GROUPS_LIST_TAG || _bufferSize <= 0)
    {
        return 0;    
    }
    i += 1;
    totalLength = _buffer[i];
    return totalLength;
}

int PackListGroupsReplay(Vector* _pVector ,int _bufferSize, char* _buffer)
{
    int i = 0, totalLength = 0, lengthGroupName;
    void* nameGroup;
    if(_bufferSize <= 0)
    {
        return 0;
    }
    /*copy the tag*/
    _buffer[i] = PRESENT_ACTIVE_GROUPS_LIST_REPLAY_TAG;
    i += 2; /*skip on total len*/
    while(VectorSize(_pVector) > 0)
    {
        if(VectorRemove(_pVector, &nameGroup) != VECTOR_SUCCESS)
        {
            return 0; 
        }
        lengthGroupName = strlen((char*)nameGroup);
        _buffer[i] = lengthGroupName;
        strncpy(&_buffer[i+1], (char*)nameGroup,lengthGroupName);
        i = i + 1 + lengthGroupName;
        totalLength = totalLength + lengthGroupName + 1; 
    }
    _buffer[1] = totalLength;
    return totalLength + 2;
}
int UnPackListGroupsReplay(Vector* _pVector, int _bufferSize, char* _buffer) 
{
    int lengthGroupName, totalLength;
    int i = 0;
    char* nameOfGroup =  NULL;
    if (NULL == (nameOfGroup = (char*)malloc(GROUP_NAME_SIZE*sizeof(char))))
    {
        return 0;
    } 
    if(_bufferSize <= 0 || VectorSize(_pVector) != 0)
    {
        return 0;
    }
    /*check the tag first*/
    if(_buffer[i] != PRESENT_ACTIVE_GROUPS_LIST_REPLAY_TAG)
    {
        return 0;    
    }
    i += 1;
    totalLength = _buffer[i];
    i += 1;   
    while(i < totalLength)
    {
        lengthGroupName =_buffer[i];
        strcpy(nameOfGroup, &_buffer[i+1]);
        nameOfGroup[lengthGroupName] = '\0';        
        if(VectorAppend(_pVector, (void*)nameOfGroup) != VECTOR_SUCCESS)
        {
            return 0;
        }
        i = i + 1 + lengthGroupName;
    }
    free(nameOfGroup);
    return totalLength + 2;
 }
/*
int UnPackListGroupsReplay(Vector* _pVector, int _bufferSize, char* _buffer) 
{
    int lengthGroupName, totalLength;
    int i = 0;
    char nameOfGroup[GROUP_NAME_SIZE];
    if(_bufferSize <= 0 || VectorSize(_pVector) != 0)
    {
        return 0;
    }
    /*check the tag first
    if(_buffer[i] != PRESENT_ACTIVE_GROUPS_LIST_REPLAY_TAG)
    {
        return 0;    
    }
    i += 1;
    totalLength = _buffer[i];
    i += 1;   
    while(i < _bufferSize)
    {
        lengthGroupName =_buffer[i];
        strncpy(nameOfGroup, &_buffer[i+1], lengthGroupName);
        nameOfGroup[lengthGroupName] = '\0';        
        if(VectorAppend(_pVector, (void*)nameOfGroup) != VECTOR_SUCCESS)
        {
            return 0;
        }
        i = i + 1 + lengthGroupName;
    }
    return totalLength + 2;
 }
*/
/*................... LIVE GROUP..................*/

int PackLeaveGroup(char* _groupName, char* _buffer,  int _bufferSize)
{
    int i = 0,lengthGroupName,totalLength;
    if(NULL == _groupName || 0 >= _bufferSize)
    {
        return 0;
    }
    lengthGroupName = strlen(_groupName);
    totalLength = lengthGroupName + 1;

    /*copy the tag*/
    _buffer[i] = LIVE_GROUP_TAG;
    i += 1;
    /*copy the group name length*/
    _buffer[i] = totalLength;
    i += 1;

    /*copy the group name and length of name group */
    _buffer[i] = lengthGroupName;
    strncpy(&_buffer[i+1], _groupName,lengthGroupName );

    return totalLength + 2;
}

int UnPackLeaveGroup(char* _buffer, int _bufferSize, char* _groupName)
{
    int i = 0;
    int groupNameLength, totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != LIVE_GROUP_TAG || _bufferSize <= 0)
    {
        return 0;   
    }
    i += 1;
    totalLength = _buffer[i];
    i += 1;
    /*retrieve the group length*/
    groupNameLength = _buffer[i];
    i += 1;
    /*retrieve the group name  */
    strncpy(_groupName, &_buffer[i], groupNameLength);
    _groupName[groupNameLength] = '\0';

    return totalLength + 2;
}
/************************/
int PackLeaveGroupReplay(int _status, char* _buffer)
{
    int i = 0;
    int totalLength = 3,length = 1;
    /*copy the tag*/
    _buffer[i] = LIVE_GROUP_REPLAY_TAG;
    i += 1;    
    _buffer[i] = length;
    i += 1;
    _buffer[i] = _status;
    return totalLength;
}
int UnPackLeaveGroupReplay(int _bufferSize, char* _buffer) 
{
    int i = 0, statusLength;
    if(_buffer[i] != LIVE_GROUP_REPLAY_TAG || _bufferSize <= 0)
    {
        return -1;
    } 
    i += 1;
    _buffer[i] = statusLength;
    i += 1;
    return _buffer[i]; /*return status*/
}
/*................LOG OUT.................*/

int PackLogOut(char* _buffer,  int _bufferSize) 
{
    int i = 0;
    int totalLength = 1;
    if(_bufferSize <= 0)
    {
        return 0;
    }
    /*copy the tag*/
    _buffer[i] = LOG_OUT_TAG;
    i += 1;
    _buffer[i] = totalLength;
    i += 1;
    _buffer[i] = '\0';

    return totalLength + 2;
}

int UnPackLogOut(char* _buffer,  int _bufferSize) 
{
    int i = 0;
    int totalLength;
    /*retrieve the tag*/
    if(_buffer[i] != LOG_OUT_TAG || _bufferSize <= 0)
    {
        return 0;    
    }
    i += 1;
    totalLength = _buffer[i];
    return totalLength;
}

int PackLogOutReplay(int _status, char* _buffer)
{
    int i = 0;
    int totalLength = 3,length = 1;
    /*copy the tag*/
    _buffer[i] = LOG_OUT_REPLAY_TAG;
    i += 1;    
    _buffer[i] = length;
    i += 1;
    _buffer[i] = _status;
    return totalLength;
}

int UnPackLogOutReplay(int _bufferSize, char* _buffer) 
{
    int i = 0, statusLength;
    if(_buffer[i] != LOG_OUT_REPLAY_TAG || _bufferSize <= 0)
    {
        return -1;
    } 
    i += 1;
    _buffer[i] = statusLength;
    i += 1;
    return _buffer[i]; /*return status*/
}