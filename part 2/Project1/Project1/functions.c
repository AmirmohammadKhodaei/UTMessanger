#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "functions.h"


char* getInput() {
	char* currentInput;
	char input;
	int i = 0;
	printf(">>  ");
	currentInput = (char*)malloc(sizeof(char));
	while ((input = getchar()) != '\n') {
		currentInput[i] = input;
		i++;
		currentInput = (char*)realloc(currentInput, (i + 1) * sizeof(char));
	}
	currentInput[i] = '\0';
	return currentInput;
}

char* stringToken(char* inputCommand, int tokenNumber, char delim) {
	int firstDelim = 0;
	int secondDelim = 0;

	char* temp = (char*)malloc((strlen(inputCommand) + 1) * sizeof(char));
	for (int i = 0; i < strlen(inputCommand); i++) {
		temp[i] = inputCommand[i];
	}
	temp = (char*)realloc(temp, ((strlen(inputCommand) + 2) * sizeof(char)));
	/*I used this statement to be able to add space to fiirst of input command and end of that in order that  I can
	work easier by that*/
	for (int i = strlen(inputCommand); i >= 0; i--) {
		temp[i + 1] = temp[i];
	}
	/*a loop to shift all characters of our string after adding space to first and end of that*/
	temp[ZERO] = delim;/*adding space to first of string*/
	temp[strlen(inputCommand) + 1] = delim;/*adding space to end of string*/
	temp[strlen(inputCommand) + 2] = '\0';
	{
		int delimCounter = 0;
		for (int i = 0; i < strlen(temp); i++) {
			if (temp[i] == delim) {
				if (delimCounter == tokenNumber) {
					firstDelim = i;
					break;
				}
				delimCounter++;
			}
		}
	}
	{
		int delimCounter = 0;
		for (int i = 0; i < strlen(temp); i++) {
			if (temp[i] == delim) {
				if (delimCounter == tokenNumber + 1) {
					secondDelim = i;
					break;
				}
				delimCounter++;
			}
		}
	}

	if (secondDelim > firstDelim) {
		char* returnedToken = (char*)malloc((secondDelim - firstDelim + 1) * sizeof(char));
		for (int i = firstDelim; i < secondDelim; i++) {
			returnedToken[i - firstDelim] = temp[i + 1];
		}

		returnedToken[secondDelim - firstDelim - 1] = '\0';

		return returnedToken;
	}
	else {
		return NULL;
	}

}


int signup(char* inputUsername, char* inputPassword, char* inputPhoneNumber, user* head, char* signupInput) {
	time_t currTime;
	if (tokenCounter(signupInput) != SIGNUP_COMMAND_TOKEN_NUMBER) {
		printf("Command format was wrong.\n");
		return FALSE;
	}
	if ((usernameExist(head, inputUsername) == FALSE) && (checkPasswordFormat(inputPassword) == TRUE)
		&& (checkPhoneNumberFormat(inputPhoneNumber) == TRUE)) {
		time(&currTime);
		char *regTime = (char*)malloc((strlen(ctime(&currTime)) + 1) * sizeof(char));
		strcpy(regTime, ctime(&currTime));
		printf("successfully registerd\n");
		addUser(head, inputUsername, inputPassword, inputPhoneNumber, regTime/*passing reg time*/);
		return TRUE;
	}
	return FALSE;
}

int tokenCounter(char* inputString) {
	char* tempInput = NULL;
	int i = 1;
	tempInput = (char*)malloc((strlen(inputString) + 1)*(sizeof(char)));
	strcpy(tempInput, inputString);
	strtok(tempInput, " ");
	while (strtok(NULL, " ") != NULL) {
		i++;
	}
	return i;
}

int checkPasswordFormat(char* inputPassword) {
	int i = 0;
	while (inputPassword[i] != '\0') {
		i++;
	}
	if (i < MIN_PASS_LENGTH) {
		printf("Your password should have 6 characters at least.\n");
		return FALSE;
	}
	return TRUE;
}

int checkPhoneNumberFormat(char* inputPhoneNumber) {
	int i = 0;
	if (inputPhoneNumber[i] != '0') {
		printf("Your phone number should start with 0.\n");
		return FALSE;
	}
	while (inputPhoneNumber[i] != '\0') {
		i++;
	}
	if (i != NUM_LENGTH) {
		printf("Your phone number should have exactly 11 digits.\n");
		return FALSE;
	}
	for (int i = 0; i < NUM_LENGTH; i++) {
		if ((inputPhoneNumber[i] < ASCII_CODE_0) || (inputPhoneNumber[i] > ASCII_CODE_9)) {
			printf("Your phone number shoulde have numeric characters only.\n");
			return FALSE;
		}
	}
	return TRUE;
}

int addUser(user* head, char* username, char* password, char* phoneNumber, char* regTime) {
	user* current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = (user*)malloc(sizeof(user));
	current->next->username = (char*)malloc((strlen(username) + 1) * sizeof(char));
	current->next->password = (char*)malloc((strlen(password) + 1) * sizeof(char));
	current->next->phoneNumber = (char*)malloc((NUM_LENGTH + 1) * sizeof(char));
	current->next->registerTime = (char*)malloc((strlen(regTime) + 1) * sizeof(char));
	strcpy(current->next->username, username);
	strcpy(current->next->password, password);
	strcpy(current->next->phoneNumber, phoneNumber);
	strcpy(current->next->registerTime, regTime);
	current->next->next = NULL;
	return TRUE;
}

int usernameExist(user* head, char* inputUsername) {
	user* current = head->next;
	if (current == NULL) {
		return FALSE;
	}
	while (current != NULL) {
		if (strcmp(current->username, inputUsername) == FALSE) {
			printf("Username exists.\n");
			return TRUE;
		}
		current = current->next;
	}
	return FALSE;
}

int info(user* head) {
	user* temp = head;
	if (temp->next == NULL) {
		printf("No user registered yet.\n");
		return TRUE;
	}
	while (temp->next != NULL) {
		printf("%s\n", temp->next->username);
		temp = temp->next;
	}
	return TRUE;
}

int login(char* username, char* password, user* head, char* loginInput) {
	if (tokenCounter(loginInput) != LOGIN_COMMAND_TOKEN_NUMBER) {
		printf("Command format was wrong.\n");
		return FALSE;
	}
	if ((searchUsername(username, head) == TRUE) && ((matchPassword(password, username, head) == TRUE))) {
		printf("successfully logged in\n");
		return TRUE;
	}
	return FALSE;
}

user* searchUsername(char* inputUsername, user* head) {
	user* temp = head;
	while (temp->next != NULL) {
		if (strcmp(temp->next->username, inputUsername) == FALSE) {
			return TRUE;
		}
		temp = temp->next;
	}
	printf("Username doesn't exist.\n");
	return FALSE;
}

int matchPassword(char* inputPassword, char* username, user* head) {
	user* temp = head;
	while (temp->next != NULL) {
		if ((strcmp(temp->next->username, username) == FALSE) && (strcmp(temp->next->password, inputPassword) == FALSE)) {
			return TRUE;
		}
		temp = temp->next;
	}
	printf("Password doesn't match.\n");
	return FALSE;
}

int about(char* inputUsername, user* head, char* aboutInput) {
	if (tokenCounter(aboutInput) != ABOUT_COMMAND_TOKEN_NUMBER) {
		printf("Command format was wrong.\n");
	}
	if (searchUsername(inputUsername, head) == TRUE) {
		user* temp = head;
		while (temp->next != NULL) {
			if (strcmp(temp->next->username, inputUsername) == FALSE) {
				printf("%s\n", temp->next->username);
				printf("%s\n", temp->next->phoneNumber);
				return TRUE;
			}
			temp = temp->next;
		}
	}
	return FALSE;
}

int sendMessage(char* sender, char* receiver, char*sendMsgInput, message* headMsg, char* msgTxt, user* head) {
	time_t currTime, seconds;
	long long int msgTotalSec;
	char* msgTime;
	if (tokenCounter(sendMsgInput) != SEND_MSG_COMMAND_TOKEN_NUMBER) {
		printf("command format was wrong\n");
		return FALSE;
	}
	if (searchUsername(receiver, head) == FALSE) {
		return FALSE;
	}
	time(&currTime);
	msgTime = (char*)malloc((strlen(ctime(&currTime)) + 1) * sizeof(char));
	strcpy(msgTime, ctime(&currTime));
	seconds = time(NULL);
	msgTotalSec = seconds;
	printf("message seccussfully sent.\n");
	addMessage(headMsg, sender, receiver, msgTxt, msgTime, msgTotalSec);
	return TRUE;
}

int addMessage(message* headMsg, char* sender, char* receiver, char* msgTxt, char* msgTime, long long int msgTotalSec) {
	message* curr = headMsg;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = (message*)malloc(sizeof(message));
	curr->next->sender = (char*)malloc((strlen(sender) + 1) * sizeof(char));
	curr->next->receiver = (char*)malloc((strlen(receiver) + 1) * sizeof(char));
	curr->next->messageText = (char*)malloc((strlen(msgTxt) + 1) * sizeof(char));
	curr->next->msgTime = (char*)malloc((strlen(msgTime) + 1) * sizeof(char));
	curr->next->seenMessage = FALSE;
	strcpy(curr->next->sender, sender);
	strcpy(curr->next->receiver, receiver);
	strcpy(curr->next->messageText, msgTxt);
	strcpy(curr->next->msgTime, msgTime);

	curr->next->next = NULL;
	return TRUE;
}

int showReceivedMessage(char* sender, char* receiver, message* headMsg, char* inputCommand, user* head) {
	int unreadMsgCounter = 0;
	int msgCounter = 0;
	message* curr = headMsg->next;

	if (curr == NULL) {
		printf("No message.\n");
		return TRUE;
	}
	if (tokenCounter(inputCommand) != RECEIVED_MSG_COMMAND_TOKEN_NUMBER) {
		printf("command format was wrong.\n");
		return FALSE;
	}
	if (searchUsername(sender, head) == FALSE) {
		return FALSE;
	}
	while (curr != NULL) {
		curr = curr->next;
		msgCounter++;
	}
	curr = headMsg->next;
	for (msgCounter - 1; msgCounter > 0; msgCounter--) {
		for (int i = msgCounter - 1; i > 0; i--) {
			curr = curr->next;
		}
		if ((strcmp(curr->sender, sender) == FALSE) && (strcmp(curr->receiver, receiver) == FALSE)) {

			if (curr->seenMessage == FALSE) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				printf("message from : %s\n", curr->sender);
				printf("%s", curr->msgTime);
				printf("%s\n\n", curr->messageText);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
			}
			else {
				printf("message from : %s\n", curr->sender);
				printf("%s", curr->msgTime);
				printf("%s\n\n", curr->messageText);
			}

			curr->seenMessage = TRUE;
			unreadMsgCounter++;
		}
		curr = headMsg->next;
	}
	if (unreadMsgCounter == 0) {
		printf("No message.");
	}
	return TRUE;
}

int showSentMessage(char* sender, char* receiver, message* headMsg, char* inputCommand, user* head) {
	int sentMsgCounter = 0;
	int msgCounter = 0;
	message* curr = headMsg->next;
	if (curr == NULL) {
		printf("No message.\n");
		return TRUE;
	}
	if (tokenCounter(inputCommand) != SENT_MSG_COMMAND_TOKEN_NUMBER) {
		printf("command format was wrong.\n");
		return FALSE;
	}
	if (searchUsername(receiver, head) == FALSE) {
		return FALSE;
	}
	while (curr != NULL) {
		curr = curr->next;
		msgCounter++;
	}
	curr = headMsg->next;
	for (msgCounter - 1; msgCounter > 0; msgCounter--) {
		for (int i = msgCounter - 1; i > 0; i--) {
			curr = curr->next;
		}
		if ((strcmp(curr->sender, sender) == FALSE) && (strcmp(curr->receiver, receiver) == FALSE)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			printf("message to : %s\n", curr->receiver);
			printf("%s", curr->msgTime);
			printf("%s\n\n", curr->messageText);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
			sentMsgCounter++;
		}
		curr = headMsg->next;
	}
	if (sentMsgCounter == 0) {
		printf("No message.\n");
	}
	return TRUE;
}

// dougt about this function
int editUsername(char* newUsername, char* formerUsername, user* head, char* inputCommand) {
	user* curr = head;
	if (tokenCounter(inputCommand) != EDIT_COMMAND_TOKEN_NUMBER) {
		printf("Command format was wrong.\n");
		return FALSE;
	}
	while (curr->next != NULL) {
		if (strcmp(curr->next->username, newUsername) == FALSE) {
			printf("username has been used before\n");
			return FALSE;
		}
		curr = curr->next;
	}

	curr = head;

	while (curr->next != NULL) {
		if (strcmp(curr->next->username, formerUsername) == FALSE) {
			curr->next->username = (char*)malloc((strlen(newUsername) + 1) * sizeof(char));
			strcpy(curr->next->username, newUsername);
			printf("username successfully edited.\n");
			return TRUE;
		}
		curr = curr->next;
	}
}

int editPassword(char* newPassword, char* formerPassword, user* head, char* inputCommand) {
	user* curr = head;
	if (tokenCounter(inputCommand) != EDIT_COMMAND_TOKEN_NUMBER) {
		printf("Command format was wrong.\n");
		return FALSE;
	}
	if (strcmp(newPassword, formerPassword) == FALSE) {
		printf("New password shoud differ with your former password.\n");
		return FALSE;
	}
	while (curr->next != NULL) {
		if (strcmp(curr->next->password, formerPassword) == FALSE) {
			curr->next->password = (char*)malloc((strlen(newPassword) + 1) * sizeof(char));
			strcpy(curr->next->password, newPassword);
			printf("Password successfully edited.\n");
			return TRUE;
		}
		curr = curr->next;
	}
}

int showUnreadMessage(message* head, char* loginUsername) {
	message* curr = head->next;
	int unreadMsgCounter = 0;
	int msgCounter = 0;
	printf("Unread messages:\n");
	while (curr != NULL) {
		curr = curr->next;
		msgCounter++;
	}
	curr = head->next;
	for (msgCounter - 1; msgCounter > 0; msgCounter--) {
		for (int i = msgCounter - 1; i > 0; i--) {
			curr = curr->next;
		}
		if ((strcmp(curr->receiver, loginUsername) == FALSE) && (curr->seenMessage == FALSE)) {
			printf("message from :");
			printf("%s\n", curr->sender);
			printf("%s\n", curr->msgTime);
			unreadMsgCounter++;
		}
		curr = head->next;
	}

	if (unreadMsgCounter == 0) {
		printf("No unread message.\n");
	}
	return TRUE;
}

int saveData(FILE *fileUser, FILE *fileMsg, user *head, message *headMsg) {
	user *curr = head->next;
	message *current = headMsg->next;
	while (curr != NULL) {
		fprintf(fileUser, "%s|", curr->username);
		fprintf(fileUser, "%s|", curr->password);
		fprintf(fileUser, "%s|", curr->phoneNumber);
		fprintf(fileUser, "%s", curr->registerTime);
		curr = curr->next;
	}
	while (current != NULL) {
		fprintf(fileMsg, "%s|", current->messageText);
		fprintf(fileMsg, "%s|", current->receiver);
		fprintf(fileMsg, "%s|", current->sender);
		fprintf(fileMsg, "%d|", current->seenMessage);
		fprintf(fileMsg, "%s", current->msgTime);
		current = current->next;
	}
	return TRUE;
}

int readUserData(FILE *fileUser, user *head) {
	user *currUser = head;
	rewind(fileUser);
	int i = 0;
	int tempInt = 0;

	char fInput;
	while (TRUE) {
		user *neww = (user*)malloc(sizeof(user));
		char *totalInput = (char*)malloc(sizeof(char));
		i = 0;
		while ((fInput = fgetc(fileUser)) != '\n') {
			if (fInput == EOF) {
				currUser->next = NULL;
				free(neww);
				return TRUE;
			}
			totalInput[i] = fInput;
			i++;
			totalInput = realloc(totalInput, (i + 1) * sizeof(char));
		}
		totalInput[i] = '\0';
		neww->username = stringToken(totalInput, ZERO, '|');
		neww->password = stringToken(totalInput, ONE, '|');
		neww->phoneNumber = stringToken(totalInput, TWO, '|');
		neww->registerTime = stringToken(totalInput, THREE, '|');
		neww->registerTime = (char*)realloc(neww->registerTime, (strlen(neww->registerTime) + 1) * sizeof(char));
		tempInt = strlen(neww->registerTime);
		neww->registerTime[tempInt] = '\n';
		neww->registerTime[tempInt + 1] = '\0';

		currUser->next = neww;
		currUser = neww;
	}
	return TRUE;
}

int readMsgData(FILE *fileMsg, message *headMsg) {
	message *currMsg = headMsg;
	rewind(fileMsg);
	int i = 0;
	int tempInt = 0;

	char fInput;
	while (TRUE) {
		message *neww = (message*)malloc(sizeof(message));
		char *totalInput = (char*)malloc(sizeof(char));
		i = 0;
		while (((fInput = fgetc(fileMsg)) != '\n')) {
			if (fInput == EOF) {
				currMsg->next = NULL;
				free(neww);
				return TRUE;
			}
			totalInput[i] = fInput;
			i++;
			totalInput = realloc(totalInput, (i + 1) * sizeof(char));
		}
		totalInput[i] = '\0';
		neww->messageText = stringToken(totalInput, ZERO, '|');
		neww->receiver = stringToken(totalInput, ONE, '|');
		neww->sender = stringToken(totalInput, TWO, '|');
		neww->seenMessage = stringToken(totalInput, THREE, '|');
		neww->msgTime = stringToken(totalInput, FOUR, '|');
		neww->msgTime = (char*)realloc(neww->msgTime, (strlen(neww->msgTime) + 1) * sizeof(char));

		tempInt = strlen(neww->msgTime);
		neww->msgTime[tempInt] = '\n';
		neww->msgTime[tempInt + 1] = '\0';

		currMsg->next = neww;
		currMsg = neww;
	}
	return TRUE;
}
