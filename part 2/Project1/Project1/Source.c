#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "functions.h"




int main() {
	user* head;
	head = (user*)malloc(sizeof(user));
	head->next = NULL;
	if (head == NULL) {
		printf("There's not enough memory.\n");
		return FALSE;
	}

	message* headMsg;
	headMsg = (message*)malloc(sizeof(message));
	headMsg->next = NULL;
	if (headMsg == NULL) {
		printf("There's not enough memory.\n");
		return FALSE;
	}

	FILE *users = fopen("user data.txt", "a+");
	if (users != NULL) {
		readUserData(users, head);
	}
	FILE *messages = fopen("message data.txt", "a+");
	if (messages != NULL) {
		readMsgData(messages, headMsg);
	}

	char *input, *now, *username, *phoneNumber, *password, *message;
	char *loginUsername = NULL;
	char *loginPassword = NULL;
	/*now is a variable that we work with it after using stringtoken each time*/
	/*loginUsername is a variable that we use it to access the user username when he/she logged in*/
	/*loginPassword is a variable that we use it to access the user password when he/she logged in*/
	int isAfterLogin = FALSE;
	int isAfterSignup = FALSE;
	/*isAfterLogin is a variable that checks user logged in or not to control input command.Because some commands
	can be done only when user logged in*/
	/*isAfterSignup is a variable that enables user to use commands that are related to after login after his/her signup*/
	int logout = FALSE;
	/*a variable that checks when user logged out*/
	printf("Welcome to UT Messenger.\n");
	while (TRUE) {
		printf("Please enter your command.\n");
		while (TRUE) {
			input = getInput();
			now = stringToken(input, ZERO, ' ');

			if ((strcmp(now, "signup") == FALSE) && (isAfterLogin == FALSE)) {
					username = stringToken(input, ONE, ' ');
					password = stringToken(input, TWO, ' ');
					phoneNumber = stringToken(input, THREE, ' ');
					if ((username == NULL) || (password == NULL) || (phoneNumber == NULL))
					{
						printf("Command format was wrong.\n");
						free(input);
						break;
					}
					if (signup(username, password, phoneNumber, head, input) == FALSE) {
						break;
					}
					isAfterSignup = TRUE;
					loginUsername = (char*)malloc((strlen(username)+1) * sizeof(char));
					loginPassword = (char*)malloc((strlen(password)+1) * sizeof(char));
					strcpy(loginUsername, username);
					strcpy(loginPassword, password);
					isAfterLogin = TRUE;
			}

			if ((strcmp(now, "login") == FALSE) || (isAfterSignup == TRUE)) {
				if(isAfterSignup == FALSE) {
					loginUsername = stringToken(input, ONE, ' ');
					loginPassword = stringToken(input, TWO, ' ');
					if ((loginUsername == NULL) || (loginPassword == NULL)) {
						printf("Command format was wrong.\n");
						free(input);
						break;
					}
					if (login(loginUsername, loginPassword, head, input) == TRUE) {
						isAfterLogin = TRUE;
						showUnreadMessage(headMsg, loginUsername);
					}
					else {
						break;
					}
				}
					while (TRUE) {
						input = getInput();
						while (TRUE) {
							now = stringToken(input, ZERO, ' ');
							if ((strcmp(now, "about") == FALSE) && (isAfterLogin == TRUE)) {
								username = stringToken(input, ONE, ' ');
								if (username == NULL) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								about(username, head, input);
								break;
							}
							else if (strcmp(now, "info") == FALSE) {
								info(head);
								break;
							}
							else if ((strcmp(now, "send") == FALSE) && (isAfterLogin == TRUE)) {
								username = stringToken(input, ONE, ' ');
								message = stringToken(input, TWO, ' ');
								if ((username == NULL) || (message == NULL)) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								sendMessage(loginUsername, username, input, headMsg, message, head);
								break;
							}
							else if ((strcmp(now, "show_received") == FALSE) && (isAfterLogin == TRUE)) {
								username = stringToken(input, ONE, ' ');
								if (username == NULL) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								showReceivedMessage(username, loginUsername, headMsg, input, head);
								break;
							}
							else if ((strcmp(now, "show_sent") == FALSE) && (isAfterLogin == TRUE)) {
								username = stringToken(input, ONE, ' ');
								if (username == NULL) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								showSentMessage(loginUsername, username, headMsg, input, head);
								break;
							}
							/*adding edit function*/
							else if ((strcmp(now, "edit_username") == FALSE) && (isAfterLogin == TRUE)) {
								username = stringToken(input, ONE, ' ');
								if (username == NULL) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								editUsername(username, loginUsername, head, input);
								break;
							}
							else if ((strcmp(now, "edit_password") == FALSE) && (isAfterLogin == TRUE)) {
								password = stringToken(input, ONE, ' ');
								if (password == NULL) {
									printf("Command format was wrong.\n");
									free(input);
									break;
								}
								editPassword(password, loginPassword, head, input);
								break;
							}
							else if ((strcmp(now, "logout") == FALSE) && (isAfterLogin == TRUE)) {
								isAfterLogin = FALSE;
								isAfterSignup = FALSE;
								logout = TRUE;
								break;
							}
							else if (strcmp(now, "close") == FALSE) {
								//save data to file
								users = fopen("user data.txt", "w");
								messages = fopen("message data.txt", "w");
								saveData(users, messages, head, headMsg);
								fclose(users);
								fclose(messages);
								return ZERO;
							}
							else {
								printf("command format was wrong.\n");
								free(input);
								break;
							}
						}
						if (logout == TRUE) {
							printf("successfully logged out\n");
							logout = FALSE;
							break;
						}
					}
			}
			else if (strcmp(now, "info") == FALSE) {
				info(head);
				break;
			}
			else if (strcmp(now, "close") == FALSE) {
				/*save data to file*/
				users = fopen("user data.txt", "w");
				messages = fopen("message data.txt", "w");
				saveData(users, messages, head, headMsg);
				fclose(users);
				fclose(messages);
				return ZERO;
			}
			else {
				printf("Command format was wrong.\n");
				free(input);
				break;
			}
		}
	}

}

	

