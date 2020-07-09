#include <stdio.h>
#include "httpDependency.h"
#include "httpHost.h"
#include "responseUtility.h"

#define MAX_CONNECTIONS 100
int connectedClients[MAX_CONNECTIONS];
int listenID;
char *messageBuffer;
char *method, *uri;


void startHttpServer(const char *port);
void resetConnections();
void respond(int clientID);



void connectionLoop(const char *PORT) {

	int connectionSlot = 0;
	printf("Server started at localhost:%s\n", PORT);

	resetConnections();
	startHttpServer(PORT);

	struct sockaddr_in clientaddr;
	socklen_t addressLength = sizeof(clientaddr);
	while (1) {
		connectedClients[connectionSlot] = accept(listenID, (struct sockaddr *) &clientaddr, &addressLength);

		if (connectedClients[connectionSlot] < 0) {
			//printf("Acceptance error\n");
		}
		else {
			respond(connectionSlot);
			//exit(0);
		}

		while (connectedClients[connectionSlot] != -1) {
			connectionSlot = (connectionSlot + 1) % MAX_CONNECTIONS;
		}
	}
}

void startHttpServer(const char *port) {

	struct addrinfo hostData;
	memset(&hostData, 0, sizeof(hostData));
	hostData.ai_family = AF_INET;
	hostData.ai_socktype = SOCK_STREAM;
	hostData.ai_flags = AI_PASSIVE;

	WSADATA wsData;

	int wsError = WSAStartup(MAKEWORD(1, 1), &wsData);

	printf("SOCKET STATUS : \t\t%d\n", wsError);

	struct addrinfo *resources, *pointer;
	int addressStatus = getaddrinfo(NULL, port, &hostData, &resources);
	printf("ADDRESS FETCHING STATUS : \t%d\n", addressStatus);
	
	if (addressStatus != 0) {
		printf("Error getting address!\n");
		return;
		//exit(1);
	}

	for (pointer = resources; pointer != NULL; pointer = pointer->ai_next) {
		int option = 1;
		listenID = socket(pointer->ai_family, pointer->ai_socktype, 0);

		int socketOptions = setsockopt(listenID, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

		printf("SOCKET OPTIONS STATUS : \t%d\n", socketOptions);

		if (listenID == -1) {
			continue;
		}
		int bindID = bind(listenID, pointer->ai_addr, pointer->ai_addrlen);
		printf("BINDING STATUS : \t\t%d\n", bindID);
		if (bindID == 0) {
			printf("---------------------------------\n");
			break;
		}
	}

	if (pointer == NULL) {
		printf("Socker or Binding error\n");
		return;
		//exit(1);
	}

	freeaddrinfo(resources);

	if (listen(listenID, 1000000) != 0) {
		printf("Listen error\n");
		return;
		//exit(1);
	}
}

void respond(int clientID) {

	messageBuffer = malloc(65535);
	int receivedID = recv(connectedClients[clientID], messageBuffer, 65535, 0);

	if (receivedID > 0) {
		messageBuffer[receivedID] = '\0';

		method = strtok(messageBuffer, " \t\r\n");
		uri = strtok(NULL, " \t");
		
		int isSame = strcmp(uri, "/favicon.ico");

		if (isSame != 0) {
			printf("[%s] %s\n", method, uri);
			char *reply = generateResponseMessage(uri);

			if (reply != NULL) {
				printf("%s\n", reply);
			}
			send(connectedClients[clientID], reply, strlen(reply), 0);
		}
	}

	free(messageBuffer);
}

void resetConnections() {
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		connectedClients[i] = -1;
	}
}