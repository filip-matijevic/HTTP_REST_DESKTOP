#include "responseUtility.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
char* generateResponseHeader(const char *content);
char* getEndpointOperation(char *content);

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

		printf("Number of delimiters is %d\n", getEndpointLayer(uri));

		if (getEndpointLayer(uri) == 1) {
			char *operation = getEndpointOperation(content);

			int selectedOperation = getEndpointOperationIndex(operation);

			if (selectedOperation == -1) {
				return generateResponseHeader("ERROR 404: NOT FOUND");
			}
			else {
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