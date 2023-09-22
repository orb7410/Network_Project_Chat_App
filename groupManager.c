#include "groupManager.h"
#include "group.h"
#include "HashMap.h"
#include "genqueue.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>

struct GroupManager
{
    Queue* m_addressesPoolQ; 
    HashMap*  m_groupsHash;
};
GroupManagerResultCode GroupManagerCloseGroup(GroupManager* _groupManager, char* _groupName);
static size_t GroupIndexHashFunction(void* _groupName);
static int GroupEqualityFunction(void* _firstKey, void* _secondKey);
static void AddressDestroyForQ(void* _address);
static void KeyGroupDestroyFunc(void* _key);
GroupManagerResultCode FillAddressesPoolQ(Queue* _queue, int _sizeOfIP, int __maxNumOfGroups);
static char* CreateGroupKey(int _size) ;
int ActionForEachGroup(const void* _key, void* _value, void* _context);
/******************************************************************************/
GroupManager* GroupManagerCreate(size_t _maxNumOfGroups)
{
    GroupManager* newGroupManager;
    Queue* newAddressesQ;
    HashMap* newGroupsHash;
    if(0 == _maxNumOfGroups)
    {
        return NULL;
    }
    if (NULL == (newGroupManager =(GroupManager*)malloc(sizeof(GroupManager))))
    {
        return NULL;
    }
    if (NULL == (newAddressesQ = QueueCreate(_maxNumOfGroups)))
    {
        free (newGroupManager);
        return NULL;
    } 
    newGroupManager ->  m_addressesPoolQ = newAddressesQ;
    FillAddressesPoolQ(newGroupManager ->  m_addressesPoolQ, IP_SIZE, _maxNumOfGroups);

    if (NULL == (newGroupsHash = HashMap_Create(_maxNumOfGroups, GroupIndexHashFunction, GroupEqualityFunction)))
    {
        QueueDestroy(&newAddressesQ, NULL);
        free (newGroupManager);
        return NULL;
    }
    newGroupManager -> m_groupsHash = newGroupsHash;
    return newGroupManager;
}
/**********************************************************************************/
void GroupManagerDestroy (GroupManager** _groupManager)
{
    if (NULL == _groupManager || NULL == *_groupManager)
    {
        return; 
    }

    QueueDestroy((&(*_groupManager) -> m_addressesPoolQ), AddressDestroyForQ);
    HashMap_Destroy((&(*_groupManager)->m_groupsHash),KeyGroupDestroyFunc , GroupDestroy);
    free((*_groupManager));
    *_groupManager = NULL;
}
/**********************************************************************************/
GroupManagerResultCode GroupManagerOpenGroup(GroupManager* _groupManager, char* _groupName, char* _addressIP, char* _port)
{
    Group* newGroup;
    char* groupKey;
    int groupNameLen;
    char* newIP;
    if (NULL == _groupManager || NULL == _groupName || NULL == _groupName || NULL == _port || NULL == _addressIP)
    {
        return GROUPMANAGER_NOT_INITIALIZED; 
    }
    if (TRUE ==  IsGroupExist(_groupManager, _groupName, (void*)&newGroup))
    {
        return GROUPMANAGER_DUPLICATION; 
    }
    if(QUEUE_SUCCESS != QueueRemove(_groupManager -> m_addressesPoolQ,(void*)&newIP))
    {
        return GROUPMANAGER_FAIL;
    }
    strcpy(_addressIP, newIP);
    groupNameLen = (1+strlen(_groupName));
    if (NULL == (groupKey = CreateGroupKey(groupNameLen))) /*malloc to username*/
    {
        return GROUPMANAGER_FAIL;
    }
    strcpy(groupKey, _groupName);
    if(NULL == (newGroup = GroupCreate(_groupName, PORT, _addressIP)))
    {
        return GROUPMANAGER_FAIL;
    }
    if (MAP_SUCCESS != HashMap_Insert(_groupManager-> m_groupsHash, (void*)groupKey, (void*)newGroup))
    {
        free(groupKey);
        GroupDestroy((void*)newGroup);
        return GROUPMANAGER_FAIL;
    }
    strcpy(_port, PORT);
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
GroupManagerResultCode GroupManagerJoinToGroup(GroupManager* _groupManager, char* _groupName, char* _addressIP, char* _port)/*not update userlist!!*/
{
    Group* newGroup;
    char getIp[IP_SIZE];
    if (NULL == _groupManager || NULL == _groupName || NULL == _groupName || NULL == _port || NULL == _addressIP)
    {
        return GROUPMANAGER_NOT_INITIALIZED; 
    }
    if (FALSE ==  IsGroupExist(_groupManager, _groupName, (void*)&newGroup))
    {
        return GROUPMANAGER_NOT_FOUND; 
    } 
    if (GROUP_SUCCESS != GroupGetIPAddress(newGroup, getIp))
    {
        return GROUPMANAGER_FAIL;
    }
    strcpy(_addressIP, getIp);
    strcpy(_port, PORT);
    if (GROUP_SUCCESS != IncreasNumOfGroupUsers(newGroup))
    {
        return GROUPMANAGER_NOT_INITIALIZED;
    }
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
GroupManagerResultCode GroupManagerGiveListOfGroups(GroupManager* _groupManager, Vector* _vector)
{
    if (NULL == _groupManager || NULL == _vector)
    {
        return GROUPMANAGER_NOT_INITIALIZED; 
    }
    if (0 == HashMap_ForEach(_groupManager-> m_groupsHash, ActionForEachGroup , (void*)_vector))
    {
        return GROUPMANAGER_IS_EMPTY;
    }
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
int ActionForEachGroup(const void* _key, void* _value, void* _context)
{
    Vector* vector = (Vector*)_context;
    void* keyforInsert = (void*)_key;
    if(VECTOR_SUCCESS != VectorAppend(vector, keyforInsert))
    {
        return 0;
    }
    return 1;    
}
/**********************************************************************************/
GroupManagerResultCode GroupManagerleaveGroup(GroupManager* _groupManager, char* _groupName)
{
    Group* group;
    int result;
    if (NULL == _groupManager || NULL == _groupName)
    {
        return GROUPMANAGER_NOT_INITIALIZED; 
    }
    if (FALSE ==  IsGroupExist(_groupManager, _groupName, (void*)&group))
    {
        return GROUPMANAGER_NOT_FOUND; 
    } 
    if (GROUP_NOT_INITIALIZED == (result = DecreasNumOfGroupUsers(group)))
    {
        return GROUPMANAGER_FAIL;
    }
    if(GROUP_IS_EMPTY == result)
    {
        if (GROUPMANAGER_SUCCESS != GroupManagerCloseGroup(_groupManager , _groupName)) 
        {
            return GROUPMANAGER_CLOSE_GROUP_FAIL;
        }
    }
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
int IsGroupExist(GroupManager* _groupManager, char* _groupKey, void** _group)
{
    if (MAP_SUCCESS ==  HashMap_Find(_groupManager-> m_groupsHash, (void*)_groupKey, _group))
    {
        return TRUE;
    }
    return FALSE;
}
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
GroupManagerResultCode GroupManagerCloseGroup(GroupManager* _groupManager, char* _groupName)
{
    Group* group;
    void* key, *value;
    if (FALSE ==  IsGroupExist(_groupManager, _groupName, (void*)&group))
    {
        return GROUPMANAGER_NOT_FOUND; 
    } 
    if (MAP_SUCCESS !=  HashMap_Remove(_groupManager->m_groupsHash, (void*)_groupName, &key, &value))
    {
        return GROUPMANAGER_FAIL;
    }
    free(key);
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
static char* CreateGroupKey(int _size) 
{
    char* key = (char*)malloc(_size*sizeof(char));
    if (key == NULL)
    {
        return NULL;
    }
    return key;
}
/**********************************************************************************/
static void KeyGroupDestroyFunc(void* _key)
{
    free(_key);
}
/**********************************************************************************/
GroupManagerResultCode FillAddressesPoolQ(Queue* _queue, int _sizeOfIP, int _maxNumOfGroups) /*fill the Q with addresses 224.0.0.(1-100)*/
{
    char* addressIP; 
    int i;
    for(i = 1; i < (_maxNumOfGroups+1) ; i++)
    {
        if (NULL == (addressIP = (char*)malloc(_sizeOfIP*sizeof(char))))
        {
            return GROUPMANAGER_FAIL;
        }
        snprintf(addressIP, IP_SIZE, "224.0.0.%d", i);
        if ( QUEUE_SUCCESS !=QueueInsert(_queue, addressIP))
        {
            free(addressIP);
            return GROUPMANAGER_FAIL;
        }
    }
    return GROUPMANAGER_SUCCESS;
}
/**********************************************************************************/
static void AddressDestroyForQ(void* _address)
{
	if(NULL == _address)
	{
		return;
	}
	free(_address);
}
/**********************************************************************************/
static size_t GroupIndexHashFunction(void* _groupName) /*manipulation to get uniq index from a char*/
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
/*****************/
static int GroupEqualityFunction(void* _firstKey, void* _secondKey) /*do strcmpr*/
{
    if (0 == strcmp((char*)_firstKey, (char*)_secondKey))
    {
        return TRUE;
    }
    return FALSE;
}