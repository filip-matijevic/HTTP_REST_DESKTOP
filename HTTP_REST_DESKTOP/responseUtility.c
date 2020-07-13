#include "responseUtility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* generateResponseHeader(char *buffer, char *content);
char* getEndpointOperation(char *content);
int* getOperands(char *content);
int isArgumentLineCorrect(char *content);
char* generateBadRequest(char *buffer);

char* generateResponseHeader(char *buffer, char *content) {

	char responseMessage[100000] = "HTTP/1.1 200 OK\n";

	//strcat(buffer, "HTTP/1.1 200 OK\n");
	int contentLength = strlen(content) + 10;
	char contentLengthString[200];
	_itoa(contentLength, contentLengthString, 10);

	strcat(responseMessage, "Content-Length: ");
	strcat(responseMessage, contentLengthString);
	strcat(responseMessage, "\nConnection: keep-alive\n\nRESPONSE :");
	strcat(responseMessage, content);

	strcpy(buffer, responseMessage);
	printf("RESPONSE %s\n", content);
	//return responseMessage;
}

char* generateBadRequest(char *buffer) {
	char responseMessage[100000] =	"HTTP/1.1 400\n"
									"Content-Length: 11"
									"Connection: close\n\n"
									"Bad Request";


	strcpy(buffer, responseMessage);
}

char* generateResponseMessage(char *buffer, char *content) {
	if (strcmp(content, "/") == 0) {
		return generateResponseHeader(buffer, "Hello World");
	}
	else {
		//so, there is something
		char uri[200];
		strcpy(uri, content);

		if (getEndpointLayer(uri) == 1) {
			char *operation = getEndpointOperation(content);

			int selectedOperation = getEndpointOperationIndex(operation);

			if (selectedOperation == -1) {
				return generateResponseHeader(buffer, "ERROR 404: NOT FOUND");
			}
			else {
				char *arguments = strtok(NULL, "?");
				int *operands = getOperands(arguments);

				if (operands[2] == -1) {
					//SOMETHING IS WRONG WITH OPERANDS
					return generateResponseHeader(buffer, "400 Bad Request");
				}
				else {
					char solution[10];
					_itoa(calculate(selectedOperation, operands[0], operands[1]), solution, 10);
					return generateResponseHeader(buffer, solution);
				}


			}
		}
		else {
			return generateResponseHeader(buffer, "ERROR : ONLY ONE ENDPOINT LAYER SUPPORTED");
		}

	}
	return generateResponseHeader(buffer, "ERROR : UNDEFINED");
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

int calculate(int operation, int a, int b) {
	switch (operation)
	{
	case 0:
		return a + b;
		break;
	case 1:
		return a - b;
		break;	
	case 2:
		return a * b;
		break;
	case 3:
		return a / b;
		break;
	default:
		break;
	}
}

//This is not the best way to split a string, syntax trees would be good here
int* getOperands(char *content) {				//A=3&B=4
	int operandValues[3];
	operandValues[2] = isArgumentLineCorrect(content);
	printf("%d\n", operandValues[2]);
	char* leftOperand[10];
	char* rightOperand[10];

	strcpy(leftOperand, strtok(content, "&"));	//*A=3[NULL]B=4

	strcpy(rightOperand, strtok(NULL, "&"));	//A=3[NULL]*B=4

	char *left = strtok(leftOperand, "=");		//*A[NULL]3
	left = strtok(NULL, "=");					//A[NULL]*3
	char *right = strtok(rightOperand, "=");	//*B[NULL]4
	right = strtok(NULL, "=");					//B[NULL]*4

	operandValues[0] = atoi(left);
	operandValues[1] = atoi(right);
	return operandValues;
}

int isArgumentLineCorrect(char *content) {
	int charIndex = 0;
	int numberOfA = 0;
	int numberOfB = 0;
	

	while (content[charIndex] != '\0') {
		if (content[charIndex] == 'a') {
			numberOfA++;
		}

		if (content[charIndex] == 'b') {
			numberOfB++;
		}
		charIndex++;
	}

	if (numberOfA == 1 && numberOfB == 1) {
		return 1;
	}
	else {
		return -1;
	}
}