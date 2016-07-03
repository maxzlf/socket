#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../type.h"
#include "../common.h"

static int InitServer()
{
	int iListenFd = -1;
	int iConnectFd = -1;
	int iErr = -1;
	int iLen = 0;
	struct sockaddr_in stAddr;

	iListenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > iListenFd)
	{
		return iListenFd;
	}

	memset(&stAddr, 0, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	stAddr.sin_port = htons(2048);
	stAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iErr = bind(iListenFd, (struct sockaddr *)&stAddr, sizeof(stAddr));
	if (0 != iErr)
	{
		close(iListenFd);
		return -1;
	}

	iErr = listen(iListenFd, SOMAXCONN);
	if (0 != iErr)
	{
		close(iListenFd);
		return -1;
	}

	iLen = sizeof(stAddr);
	iConnectFd = accept(iListenFd, (struct sockaddr *)&stAddr, &iLen);
	if (-1 == iConnectFd)
	{
		close(iListenFd);
		return -1;
	}

	close(iListenFd);
	return iConnectFd;
}

static void Chat(IN int iSocket)
{
	char acBuf[BUFLEN];
	char acSendBuf[BUFLEN];
	int iErr = -1;
	char *pc = NULL;

	assert(-1 != iSocket);

	memset(acBuf, 0, sizeof(acBuf));
	pc = acBuf;
	iErr = recv(iSocket, pc, sizeof(acBuf), 0);
	if (-1 == iErr)
	{
		return;
	}

	printf("client:%s\n", acBuf);

	memset(acSendBuf, 0, sizeof(acSendBuf));
	snprintf(acSendBuf, sizeof(acSendBuf), "echo");
	strncat(acSendBuf, acBuf, sizeof(acSendBuf));
	iErr = send(iSocket, acSendBuf, strlen(acSendBuf), 0);
	if (-1 == iErr)
	{
		printf("Failed to answer message\n");
		return;
	}

	return;
}

int main(int argc, char *argv[])
{
	int iConnectFd = -1;

	iConnectFd = InitServer();
	if (-1 == iConnectFd)
	{
		printf("Failed to connect\n");
		return 0;
	}
	printf("Success to connect\n");

	while(1)
	{
		Chat(iConnectFd);
	}

	close(iConnectFd);

	return 0;
}