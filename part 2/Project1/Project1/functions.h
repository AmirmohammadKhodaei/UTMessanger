#pragma once


#define TRUE 1
#define FALSE 0
#define MIN_PASS_LENGTH 6
#define SIGNUP_COMMAND_TOKEN_NUMBER 4
#define LOGIN_COMMAND_TOKEN_NUMBER 3
#define ABOUT_COMMAND_TOKEN_NUMBER 2
#define SEND_MSG_COMMAND_TOKEN_NUMBER 3
#define RECEIVED_MSG_COMMAND_TOKEN_NUMBER 2
#define SENT_MSG_COMMAND_TOKEN_NUMBER 2
#define EDIT_COMMAND_TOKEN_NUMBER 2
#define NUM_LENGTH 11
#define ASCII_CODE_0 48
#define ASCII_CODE_9 57
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4

struct user {
	char* phoneNumber;
	char* password;
	char* username;
	char* registerTime;
	struct user* next;
};

/*a struct that contains user info such as phone number, passwors, ...*/

struct message {
	char* receiver;
	char* sender;
	char* messageText;
	char* msgTime;
	int seenMessage;
	struct message* next;
};

typedef struct user user;
typedef struct message message;


/*a struct that contains message info such as sende, receiver, ...*/

char* getInput();
/*a function that gets user input only*/

char* stringToken(char* inputCommand, int tokenNumber, char delim);
/*a function that devides input sting to smaller parts*/

int signup(char* inputUsername, char* inputPassword, char* inputPhoneNumber, user* head, char* signupInput);
/*a function that checks user info by other functions and if they were true saves them and pass them to addUser function*/

int tokenCounter(char* inputString);
/*a function that checks if there is space character in input string or not */

int checkPasswordFormat(char* inputPassword);
/*a function that checks that if password has at least 6 characters or not*/

int checkPhoneNumberFormat(char* inputPhoneNumber);
/*a function that checks phone number format is true or not*/

int usernameExist(user* head, char* inputUsername);
/*a function that checks if input username exists or not*/

int addUser(user* head, char* username, char* password, char* phoneNumber, char* regTime);
/*a function that adds new user to users list*/

user* searchUsername(char* inputUsername, user* head);
/*a function that searches on user list to find input username*/

int matchPassword(char* inputPassword, char* username, user* head);
/*a function that checks if password matches username or not*/

int info(user* head);
/*a function that shows all usernames that registerd in the program*/

int about(char* inputUsername, user* head, char* aboutInput);
/*a function that shows wanted username and his/her phone number if username be available*/

int login(char* username, char* password, user* head, char* loginInput);
/*a function that uses other functions to check user password and username and enables user to messaging*/

int editUsername(char* newUsername, char* formerUsername, user* head, char* inputCommand);
/*a function that allows user to edit his/her username if input data be true*/

int editPassword(char* newPassword, char* formerPassword, user* head, char* inputCommand);
/*a function that enables user to edit his password if input data be true*/

int showUnreadMessage(message* head, char* loginUsername);
/*a function that shows all unread messages of user according its time*/

int showReceivedMessage(char* sender, char* receiver, message* headMsg, char* inputCommand, user* head);
/*a function that shows received messages by finding its sender and its receiver*/

int showSentMessage(char* sender, char* receiver, message* headMsg, char* inputCommand, user* head);
/*a function that shows sent messages by finding its sender and its receiver*/

int sendMessage(char* sender, char* receiver, char*sendMsgInput, message* headMsg, char* msgTxt, user* head);
/*a function that at first finds receiver user by search user function and then pass message info to addMessage function
to add a new message to message list*/

int addMessage(message* headMsg, char* sender, char* receiver, char* msgTxt, char* msgTime, long long int msgTotalSec);
/*a function that adds new message to message list*/

int saveData(FILE *fileUser, FILE *fileMsg, user *head, message *headMsg);
/*a function that saves user list and message list to file*/

int readUserData(FILE *fileUser, user *head);
/*a function that reads info from user file and puts them in user list*/

int readMsgData(FILE *fileMsg, message *headMsg);
/*a function that reads info from message file and puts them in message list*/