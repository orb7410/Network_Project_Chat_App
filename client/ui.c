#include <stdio.h> /*printf*/
#include <string.h>
#include <termios.h> /*tcsetattr*/
#include <stdlib.h>
#include "ui.h" /*ui function decleration*/
void HidePassward(char _password[], char _userName[]);
/**********************************************************************************/
int ClientEnterApp()
{
    int clientChice;
    printf("%s %s %s %s", BLUE BOLD_ON"\nHello welcome to group chat app: "RESET, CYAN"\n\nTo log in press 1."RESET, GREEN"\nTo sign in and registration press 2."RESET, YELLOW"\nTo exit press 3.\n"RESET);
    printf(BLUE"\nuser: ");
    scanf("%d", &clientChice);
    printf("\n"RESET);
    return clientChice;
}

void PrintArtBlue(char _fileName[]) 
{
    FILE *ptrFile;
    char art[100];
    ptrFile = fopen(_fileName, "r");
    if (ptrFile == NULL) 
    {
        printf("Error opening file");
        return;
    }
    while(fgets(art, 100, ptrFile) != NULL) {
        printf(BLUE"%s", art);
    }
    printf(""RESET);
    fclose(ptrFile);
}

void PrintArtYellow(char _fileName[]) 
{
    FILE *ptrFile;
    char art[100];
    ptrFile = fopen(_fileName, "r");
    if (ptrFile == NULL) 
    {
        printf("Error opening file");
        return;
    }
    while(fgets(art, 100, ptrFile) != NULL) 
    {
        printf(YELLOW"%s", art);
    }
    printf(""RESET);
    fclose(ptrFile);
}

void PrintArtRed(char _fileName[]) 
{
    FILE *ptrFile;
    char art[100];
    ptrFile = fopen(_fileName, "r");
    if (ptrFile == NULL) 
    {
        printf("Error opening file");
        return;
    }
    while(fgets(art, 100, ptrFile) != NULL) {
        printf(RED"%s", art);
    }
    printf(""RESET);
    fclose(ptrFile);
}


void PrintArtGreen(char _fileName[]) 
{
    FILE *ptrFile;
    char art[100];
    ptrFile = fopen(_fileName, "r");
    if (ptrFile == NULL) 
    {
        printf("Error opening file");
        return;
    }
    while(fgets(art, 100, ptrFile) != NULL) {
        printf(GREEN"%s", art);
    }
    printf(""RESET);
    fclose(ptrFile);
}

void EnterClientDetails(char _userName[], char _password[])
{
    printf(BLUE"Enter user name: ");
    scanf("%s", _userName);
    printf("\n"RESET);
    HidePassward(_password, _userName);
    printf("\n");     
}

void PrintWorngIndex()
{
    printf(RED"Worng index, please try again.\n"RESET);
    printf("\n");
}

void PrintWorngUserNameOrPassword()
{
    printf(RED"Worng user name or password, please try again.\n"RESET);
    printf("\n");
}

void PrintUserNameDuplication(char _name[])
{
    printf(RED"The user name %s is already exists, please try different name.\n"RESET, _name);
    printf("\n");
}

int ClientGroupMenu(char _name[])
{
    int clientChice;
    printf(BLUE"%s, what do you want to do next?\n\n"RESET, _name);
    printf("%s%s%s%s%s", CYAN"To open group press 1.\n"RESET , GRAY"To join group press 2.\n"RESET, MAGENTA"To leave group press 3.\n"RESET  ,GREEN"To log out press 4.\n"RESET, YELLOW"To exit press 5.\n"RESET);
    printf("\n");
    printf(BLUE"\n%s: ", _name);
    scanf("%d", &clientChice);
    printf("\n"RESET);
    return clientChice;
}

void EnterGroupName(char _groupName[],char _name[])
{
    printf(BLUE"%s, enter group name: ",_name);
    scanf("%s", _groupName);
    printf("\n"RESET);   
}

void PrintDuplicatTag(char _groupName[])
{
    printf(RED"The group name '%s' is already exists, please try again.\n"RESET, _groupName);
    printf("\n");
}

void PrintNameNotFound(char _groupName[])
{
    printf(RED"The group name '%s' not found, please try again.\n"RESET, _groupName);
    printf("\n");
}

void PrintNoOpenGrops()
{
    printf(RED"There is no open groups, please try again.\n"RESET);
    printf("\n");
}
int JoinGroupAnswer(char _name[])
{
    int answer;
    printf(CYAN"%s ,do you know the name of the group you want to join?\n"RESET, _name);
    printf(CYAN"For yes: 1, for no: 2. \n"RESET);
    printf(BLUE"\n%s: ", _name);
    scanf("%d", &answer);
    printf(""RESET);
    return answer;
}



/**************************************help function****************************/
void HidePassward(char _password[], char _userName[])
{
    struct termios old, new;
    int result;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new);

    printf(BLUE"Enter password: ");
    scanf("%s", _password);
    tcsetattr(0, TCSANOW, &old);
    printf("\n\n"RESET);
    printf(CYAN BOLD_ON"Hello %s! \n"RESET, _userName);
    printf(CYAN"\n%s, do you want to see the password? \nFor yes: 1, for no: 2. \n"RESET, _userName);
    printf(BLUE"\n%s: ", _userName);
    scanf("%d", &result);
    printf(""RESET);
    if(1 == result)
    {
        printf(CYAN"\n%s, your password is: %s"RESET, _userName, _password);
        printf("\n");
    }
    else
    {
        return;
    }
}
