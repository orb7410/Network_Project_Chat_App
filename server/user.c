#include "user.h"
#include <string.h>
#include <stdlib.h>
#include "list.h" /*DLList funcions decleration*/
#include "listItr.h" /*ItrList funcions decleration*/
#include "listitrfun.h" /*ItrListForEch funcions decleration*/
int PredicateFuncForUserGroup(void * _element, void* _context);
/******************************************************************/
struct User
{
    char    m_userName[USERNAME_SIZE];
    char    m_password[PASSWORD_SIZE];
    int     m__userId;
    int     m_isActive; /*0/1*/
    List*   m_attachedGroups; /*list of the groups the user joined*/
    int     m_magicNum;
};
/******************************************************************/
User* UserCreate (char* _userName, char* _password, int _userId/*_sockId*/)
{
    User* newUser;
    List* attachedGroupsList;
    if (NULL == _userName || NULL == _password || 0 == _userId)
    {
        return NULL;
    }
    if (NULL == (newUser =(User*)malloc(sizeof(User))))
    {
        return NULL;
    }
    if (NULL == (attachedGroupsList = ListCreate()))
    {
        free(newUser);
        return NULL;
    }

    strcpy(newUser -> m_userName, _userName);
    strcpy(newUser -> m_password, _password);
    newUser -> m__userId = _userId ;
    newUser -> m_isActive = 0; 
    newUser -> m_attachedGroups = attachedGroupsList;
    newUser -> m_magicNum = MAGIC_NUM; 

    return newUser;
}
/******************************************************************/
void UserDestroy(void* _user)
{
    User* curUser = (User*)_user;
    if (NULL == curUser || MAGIC_NUM != curUser -> m_magicNum) 
	{
	    return;
	}

    ListDestroy(&curUser -> m_attachedGroups, NULL);
    curUser->m_magicNum = 0;
    free(curUser);
}
/******************************************************************/
UserResultCode Useractivate(User* _user)
{
    if (1 == _user -> m_isActive)
    {
        return USER_FAIL;
    }
    _user -> m_isActive = 1;
    return USER_SUCCESS;
}
/*******************************************************/
UserResultCode DeactivateUser(User* _user)
{
    if (0 == _user -> m_isActive)
    {
        return USER_FAIL;
    }
    _user -> m_isActive = 0;
    return USER_SUCCESS;
}
/*******************************************************/
void UpdateNewUserIdAfterLogIn(User* _user, int _userId)
{
    _user -> m__userId = _userId;
}
/*******************************************************/
UserResultCode UserInsertGroupToGroupsList(User* _user, char* _groupName)
{
    char* nameGroup;
    int size;
    void* itr;
    if (NULL == _user || NULL == _groupName)
    {
        return USER_NOT_INITIALIZED;
    }
    size = strlen(_groupName+1);
    if (NULL == (nameGroup = (char*)malloc(size*sizeof(char))))
    {
        return USER_FAIL;
    }
    if (TRUE == IsGroupInUserList(_user, _groupName, &itr))
    {
        free(nameGroup);
        return USER_DUPLICATION;
    }
    strcpy(nameGroup, _groupName);
    if (LIST_SUCCESS != ListPushHead(_user -> m_attachedGroups, (void*) nameGroup))
    {
        free(nameGroup);
        return USER_FAIL;
    }
    return USER_SUCCESS;
}
/*******************************************************/
int IsGroupInUserList(User* _user, char* _groupName, void** _itr)
{
    ListItr placeOfGroup, beginToSearch, endToSearch, result; 
    if (NULL == _user || NULL == _groupName)
    {
        return FALSE;
    }
    beginToSearch = ListItrBegin(_user->m_attachedGroups);
    endToSearch = ListItrEnd(_user->m_attachedGroups);
    if (endToSearch == (result = ListItrFindFirst(beginToSearch, endToSearch, PredicateFuncForUserGroup, _groupName)))
    {
        return FALSE;
    }
    *_itr = result;
    return TRUE;
}
/*******************************************************/
UserResultCode UserLeaveGroupRemoveFromList(User* _user,  char* _groupName)
{
    void* itr;
    if (NULL == _user || NULL == _groupName)
    {
        return USER_NOT_INITIALIZED;
    }
    if (FALSE == IsGroupInUserList (_user, _groupName, &itr))
    {
        return USER_NOT_FOUND;
    }
    if (NULL == ListItrRemove(itr))
    {
        return USER_FAIL;
    }
    return USER_SUCCESS;
}
/*******************************************************/
UserResultCode UserRemoveAllGroupsFromUser(User* _user)
{
    ListItr temp, runner, end;  
    void* removedGroup;
    if (NULL == _user)
    {
        return USER_NOT_INITIALIZED;
    }
    runner = ListItrBegin(_user->m_attachedGroups);
    end = ListItrEnd(_user->m_attachedGroups);
  
    while (runner != end)
    {
        temp = runner;
        runner = ListItrNext(runner);
        removedGroup = ListItrRemove(temp);
        free(removedGroup);   
    }
    return USER_SUCCESS;
}
/*******************************************************/
int UserGetActiveStatus(User* _user)
{
    if (NULL == _user)
    {
        return -1;
    }
    return (_user -> m_isActive);
}
/*******************************************************/
List* UserGetListOfGroups(User* _user)
{
    if (NULL == _user)
    {
        return NULL;
    }
    return (_user -> m_attachedGroups);
}
/*******************************************************/
/*******************************************************/
int PredicateFuncForUserGroup(void * _element, void* _context)
{
    if(0 == (strcmp((char*)_element,(char*)_context)))
    {
        return 1;
    }
    return 0;
}
/*****/
size_t UserGetMagicNum(User* _user)
{
	return (_user -> m_magicNum);
}
/*****/
int UserGetReallMagicNum(void)
{
	return MAGIC_NUM;
}
/*****/
char* UserGetPassword(User* _user)
{
    return (_user -> m_password);
}
/*****/
