#ifndef __UI_H__
#define __UI_H__
#define BOLD_ON "\x1b[1m"
#define BOLD_OFF "\x1b[21m"
/* colors */
#define NORMAL "\x1B[0m"
#define BLACK "\x1b[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define GRAY "\x1b[90m"

#define RESET "\033[0m"

 /** 
 * @brief Print the first menue to clientr get an input from user to choose
 * @param[in] none
 * @return  the client choice
 */
int ClientEnterApp();

 /** 
 * @brief inpt from client of name and password
 * @param[in] _userName empty str to fill
 * @param[in] _password -  empty str to fill
 * @return  none
 */
void EnterClientDetails(char _userName[], char _password[]);

 /** 
 * @brief print to client worng index
 * @param[in] none
 * @return  none
 */
void PrintWorngIndex();

 /** 
 * @brief print to client worng user name or password
 * @param[in] none
 * @return  none
 */
void PrintWorngUserNameOrPassword();

 /** 
 * @brief print to client that user name is already in use
 * @param[in] _name the user name 
 * @return  none
 */
void PrintUserNameDuplication(char _name[]);

 /** 
 * @brief Print the secend menue to clientr join group or create group
 * @param[in] none
 * @return  the client choice
 */
int ClientGroupMenu();

  /** 
 * @brief print to client that user group name is already in use
 * @param[in] _groupName the group name the client choose 
 * @return  none
 */
void PrintDuplicatTag(char _groupName[]);

/** 
 * @brief print to client that user group name is not exist
 * @param[in] _groupName the group name the client choose 
 * @return  none
 */
void PrintNameNotFound(char _groupName[]);

/** 
 * @brief print to client that ther is no open groups
 * @param[in] none 
 * @return  none
 */
void PrintNoOpenGrops();

 /** 
 * @brief input from user if he know group name
 * @param[in] _name the user name  
 * @return  user answer
 */
int JoinGroupAnswer(char _name[]);

  /** 
 * @brief inpt from client of name of group
 * @param[in] _groupName an empty string to save the client input op the group name
 * @param[in] _name the user name 
 * @return  none
 */
void EnterGroupName(char _groupName[],char _name[]);

  /** 
 * @brief print file data in color blue
 * @param[in] _fileName the file to print data from 
 * @return  none
 */
void PrintArtBlue(char _fileName[]);

/** 
 * @brief print file data in color green
 * @param[in] _fileName the file to print data from 
 * @return  none
 */
void PrintArtGreen(char _fileName[]);

/** 
 * @brief print file data in color yellow
 * @param[in] _fileName the file to print data from 
 * @return  none
 */
void PrintArtYellow(char _fileName[]);

/** 
 * @brief print file data in color red
 * @param[in] _fileName the file to print data from 
 * @return  none
 */
void PrintArtRed(char _fileName[]) ;

#endif /*__UI_H__*/