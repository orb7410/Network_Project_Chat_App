#include "userManager.h"
#include "user.h"
#include "HashMap.h"
#include <string.h> /*strings*/
#include <stdlib.h>
#include <stdio.h> /*file*/
#include "list.h" /*DLList funcions decleration*/
#include "listItr.h" /*ItrList funcions decleration*/
#include "listitrfun.h" /*ItrListForEch funcions decleration*/


static void KeyUserDestroyFunc(void* _key);
static int UserEqualityFunction(void* _firstKey, void* _secondKey);
static size_t UserIndexHashFunction(void* _data);
static char* CreateUserKey(int _size);
static UserManagerResultCode fillHashFromUserFile(UserManager* _userManager);
struct UserManager
{
    HashMap*   m_usersHash;
};
/**********************************************************************************/
UserManager* UserManagerCreate (size_t _maxUsers)
{
    UserManager* newUserManager;
    HashMap*   newUsersHash;
    if(0 == _maxUsers)
    {
        return NULL;
    }
    if (NULL == (newUserManager =(UserManager*)malloc(sizeof(UserManager))))
    {
        return NULL;
    }
    if (NULL == (newUsersHash = HashMap_Create(_maxUsers, UserIndexHashFunction, UserEqualityFunction)))
    {
        free (newUserManager);
        return NULL;
    }
    newUserManager -> m_usersHash = newUsersHash;
    fillHashFromUserFile(newUserManager);
    return newUserManager;
}
/**********************************************************************************/
void UserManagerDestroy (UserManager** _userManager)
{
    if (NULL == _userManager || NULL == *_userManager)
    {
        return; 
    }

    HashMap_Destroy((&(*_userManager)->m_usersHash), KeyUserDestroyFunc, UserDestroy);
    free((*_userManager));
    *_userManager = NULL;
}
/**********************************************************************************/
UserManagerResultCode UserManagerAddNewUser(UserManager* _userManager, char* _userName, char* _password,  int _userId)
{
    char* userKey;
    User* newUser;
    int userNameLen;
    FILE* usersFile = NULL;
    if (NULL == _userManager | NULL == _userName || NULL == _password || 0 == _userId)
    {
        return USERMANAGER_FAIL;
    }  
    if (TRUE ==  IsUserExist(_userManager, _userName, (void*)&newUser))/*check if already sighned up*/
    {
        return USERMANAGER_DUPLICATION;
    }
    userNameLen = (1+strlen(_userName));
    if (NULL == (userKey = CreateUserKey(userNameLen))) /*malloc to username*/
    {
        return USERMANAGER_FAIL;
    }
    strcpy(userKey, _userName);
    if (NULL == (newUser = UserCreate (_userName , _password, _userId)))/*create new user*/
    {
        free(userKey);
        return USERMANAGER_FAIL;
    }
    if (MAP_SUCCESS != HashMap_Insert(_userManager-> m_usersHash, (void*)userKey, (void*)newUser))/*insert user to hash*/
    {
        free(userKey);
        UserDestroy((void*)newUser);
        return USERMANAGER_FAIL;
    }
    if ((usersFile=fopen("usersFile.txt", "a+")) != NULL)
    {
        fprintf( usersFile, "%s %s \n",  _userName, _password); /*update the file*/
        fclose(usersFile);  
    }
    return USERMANAGER_SUCCESS;
}
/**********************************************************************************/
UserManagerResultCode UserManagerLogin(UserManager* _userManager, char* _userName, char* _password,  int _userId)
{
    User* user;
    if (NULL == _userManager | NULL == _userName || NULL == _password)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }
    if (FALSE ==  IsUserExist(_userManager, _userName, (void*)&user))/*checks if exist*/
    {
        return USERMANAGER_USER_NOT_FOUND;
    }
    if (FALSE == IsPasswordCorrect(user, _password))
    {
        return USERMANAGER_WRONG_PASSWORD;
    }
    if (USER_SUCCESS != Useractivate(user))
    {
        return USERMANAGER_FAIL; /*already active*/
    }
    UpdateNewUserIdAfterLogIn(user, _userId);
    return USERMANAGER_SUCCESS;
}
/**********************************************************************************/
int IsUserExist(UserManager* _userManager, char* _userKey, void** _user)/*check if already sighned up*/
{
    if (NULL == _userManager | NULL == _userKey)
    {
        return FALSE;
    }
    if (MAP_SUCCESS ==  HashMap_Find(_userManager-> m_usersHash, (void*)_userKey, _user))
    {
        return TRUE;
    }
    return FALSE;
}
/**********************************************************************************/
UserManagerResultCode UserManagerInsertGroupToUser(UserManager* _userManager, char* _userName, char* _groupName)
{
    User* user;
    if (NULL== _userManager || NULL == _userName || NULL == _groupName)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }
    if (FALSE ==  IsUserExist(_userManager, _userName, (void*)&user))
    {
        return USERMANAGER_USER_NOT_FOUND;
    }
    if (1 != UserGetActiveStatus(user))
    {
        return USERMANAGER_NOT_ACTIVE;
    }
    if (USER_SUCCESS != UserInsertGroupToGroupsList(user, _groupName))
    {
        return USERMANAGER_FAIL;
    }
    return USERMANAGER_SUCCESS;
}
/**********************************************************************************/
UserManagerResultCode UserManagerLeaveGroup(UserManager* _userManager, char* _userName, char* _groupName)
{
    User* user;
    int result;
    if (NULL== _userManager || NULL == _userName || NULL == _groupName)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }
    if (FALSE ==  IsUserExist(_userManager, _userName, (void*)&user))
    {
        return USERMANAGER_USER_NOT_FOUND;
    }
    if (1 != UserGetActiveStatus(user))
    {
        return USERMANAGER_NOT_ACTIVE;
    }
    if (USER_NOT_FOUND == (result = UserLeaveGroupRemoveFromList(user, _groupName)))
    {
        return USERMANAGER_GROUP_NOT_FOUND;
    }
    else if ( USER_SUCCESS != result)
    {
        return USERMANAGER_FAIL;
    }
    return USERMANAGER_SUCCESS;
}
/**********************************************************************************/
List* UserManagerGetGroupsList(UserManager* _userManager, char* _userName)
{ 
    User* user;
    if (NULL== _userManager || NULL == _userName)
    {
        return NULL;
    }
    if (FALSE ==  IsUserExist(_userManager, _userName, (void*)&user))
    {
        return NULL;
    }
    if (1 != UserGetActiveStatus(user))
    {
        return NULL;
    }
    return  (UserGetListOfGroups(user));
}
/**********************************************************************************/
UserManagerResultCode UserManagerCleanUserGroupsForLogOut(UserManager* _userManager, char* _userName)
{
    User* user;
    if (NULL== _userManager || NULL == _userName)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }
    if (FALSE ==  IsUserExist(_userManager, _userName, (void*)&user))
    {
        return USERMANAGER_USER_NOT_FOUND;
    }
    if (1 != UserGetActiveStatus(user))
    {
        return USERMANAGER_NOT_ACTIVE;
    }
    if ( USER_SUCCESS != UserRemoveAllGroupsFromUser(user));
    {
       USERMANAGER_FAIL; 
    }
    return USERMANAGER_SUCCESS;
}

/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
static UserManagerResultCode fillHashFromUserFile(UserManager* _userManager)
{
    FILE* usersFile = NULL;
    char userName[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char* userKey;
    User* newUser;
    int userID = 1026;
    int userId, userNameLen;
    if (NULL == _userManager)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }
    if ((usersFile=fopen("usersFile.txt", "r")) == NULL) 
	{
      return USERMANAGER_FAIL;
    }  
        fscanf(usersFile, "%s",  userName);   
        fscanf(usersFile, "%s",  password);  
    while((!feof(usersFile)))/*loop that read from a file and insert to hash*/
    {
        userNameLen = (1+strlen(userName));
        if (NULL == (userKey = CreateUserKey(userNameLen))) 
        {
            fclose(usersFile);
            return USERMANAGER_FAIL;
        }
        strcpy(userKey, userName);
        if (NULL == (newUser = UserCreate (userName , password, userID)))
        {
            fclose(usersFile);
            free(userKey);
            return USERMANAGER_FAIL;
        }
        userID++;
        if (TRUE ==  IsUserExist(_userManager, userName, (void*)&newUser))/*checks if exist*/
        {
            fclose(usersFile);
            free(userKey);
            UserDestroy((void*)newUser);
            return USERMANAGER_FAIL;
        }
        if (MAP_SUCCESS != HashMap_Insert(_userManager-> m_usersHash, (void*)userKey, (void*)newUser))
        {
            fclose(usersFile);
            free(userKey);
            UserDestroy((void*)newUser);
            return USERMANAGER_FAIL;
        }
        fscanf(usersFile, "%s",  userName);   
        fscanf(usersFile, "%s",  password); 
    }       
  close(usersFile);
  return USERMANAGER_SUCCESS;
}
/**********************************************************************************/
int IsPasswordCorrect(User* _user, char* _password)
{
    if (NULL == _user || NULL == _password)
    {
        return FALSE;
    }
    if (0 == strcmp(_password, UserGetPassword(_user)))
    {
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************************/
static char* CreateUserKey(int _size) 
{
    char* key = (char*)malloc(_size*sizeof(char));
    if (key == NULL)
    {
        return NULL;
    }
    return key;
}
/**********************************************************************************/
static size_t UserIndexHashFunction(void* _data) /*manipulation to get uniq index from a char*/
{
    int i;
    char* username = (char*) _data;
    size_t index = 0;
    int size = strlen(username);
    for (i = 0; i < size; i++) 
    {
        index += username[i] * i;
    }
    return index;
}
/*****************/
static int UserEqualityFunction(void* _firstKey, void* _secondKey) /*do strcmpr*/
{
    if (0 == strcmp((char*)_firstKey, (char*)_secondKey))
    {
        return TRUE;
    }
    return FALSE;
}
/***************/
static void KeyUserDestroyFunc(void* _key)
{
    free(_key);
}
/*************************************************************************/
HashMap* UserManagerGetHash(UserManager* _userManager)
{
    return _userManager-> m_usersHash;
}
