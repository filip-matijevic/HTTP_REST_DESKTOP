#include "responseUtility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* generateResponseHeader(const char *content);
char* getEndpointOperation(char *content);
int* getOperands(char *content);

char* generateResponseHeader(char *content) {

	char responseMessage[100000] = "HTTP/1.1 200 OK\n";
	int contentLength = strlen(content);
	char contentLengthString[200];
	_itoa(contentLength, contentLengthString, 10);

	strcat(responseMessage, "Content-Length: ");
	strcat(responseMessage, contentLengthString);
	strcat(responseMessage, "\nConnection: keep-alive\n\n");
	strcat(responseMessage, content);
	//strcat(responseMessage, &content);

	return responseMessage;
}

char* generateResponseMessage(char *content) {
	if (strcmp(content, "/") == 0) {
		return generateResponseHeader("Hello World");
	}
	else {
		//so, there is something
		char uri[200];
		strcpy(uri, content);

		if (getEndpointLayer(uri) == 1) {
			char *operation = getEndpointOperation(content);

			int selectedOperation = getEndpointOperationIndex(operation);

			if (selectedOperation == -1) {
				return generateResponseHeader("ERROR 404: NOT FOUND");
			}
			else {
				char *arguments = strtok(NULL, "?");
				printf("ARGUMENS %s\n", arguments);
				getOperands(arguments);
				return generateResponseHeader("THIS OPERATION IS AVAILABLE");
			}
		}
		else {
			return generateResponseHeader("ERROR : ONLY ONE ENDPOINT LAYER SUPPORTED");
		}

	}
	return generateResponseHeader("ERROR : UNDEFINED");
}

char* getEndpointOperation(char *content) {
	return strtok(content, "?");
}

int getEndpointLayer(char *content) {
	int characterPointer = 0;
	int numberOfLayerDelimiters = 0;
	while (content[characterPointer] != '\0') {
		if (content[characterPointer] == '/') {
			numberOfLayerDelimiters++;
		}
		characterPointer++;
	}

	return numberOfLayerDelimiters;
}

int getEndpointOperationIndex(char *content) {
	if (strcmp(content, "/add") == 0) {
		return 0;
	}
	else if (strcmp(content, "/subtract") == 0) {
		return 1;
	}
	else if (strcmp(content, "/multiplicate") == 0) {
		return 2;
	}
	else if (strcmp(content, "/divide") == 0) {
		return 3;
	}
	return -1;
}

//This is not the best way to split a string, syntax trees would be good here
int* getOperands(char *content) {				//A=3&B=4
	int operandValues[3];
	char* leftOperand[10];
	char* rightOperand[10];
	strcpy(leftOperand, strtok(content, "&"));	//*A=3[NULL]B=4
	strcpy(rightOperand, strtok(NULL, "&"));	//A=3[NULL]*B=4

	char *left = strtok(leftOperand, "=");		//*A[NULL]3
	left = strtok(NULL, "=");					//A[NULL]*3
	char *right = strtok(rightOperand, "=");	//*B[NULL]4
	right = strtok(NULL, "=");					//B[NULL]*4

	printf("%s\n", left);
	printf("%s\n", right);
	return operandValues;
}