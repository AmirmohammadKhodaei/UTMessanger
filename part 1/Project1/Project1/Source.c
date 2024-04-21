#include <stdio.h>

typedef struct user {
	char* phoneNumber;
	char* password;
	char* username;
	struct user* next;
};

typedef struct message {
	char* receiver;
	char* sender;
	char* messageText;
	struct message* next;
	int seenMeesage;
};