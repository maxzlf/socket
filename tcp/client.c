#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../type.h"
#include "../common.h"

static int ConnectRetry(IN int iSocket,
						IN struct sockaddr *pstAddr,
						IN socklen_t stLen)
{
	int iSec = 0;

	assert(-1 != iSocket);
	assert(NULL != pstAddr);
	assert(stLen > 0);

	for (iSec = 1; iSec <= MAXSLEEP; iSec <<= 1)
	{
		if (0 == connect(iSocket, pstAddr, stLen))
		{
			return 0;
		}
		if (iSec <= MAXSLEEP/2)
		{
			sleep(iSec);
		}
	}

	return -1;
}

static int InitClient(IN const char *pcInAddr)
{
	int iSocket = -1;
	int iErr = -1;
	unsigned int uiCount = 0;
	struct sockaddr_in stServAddr;

	assert(NULL != pcInAddr);

	iSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocket)
	{
		return -1;
	}

	memset(&stServAddr, 0, sizeof(stServAddr));
	stServAddr.sin_family = AF_INET;
	stServAddr.sin_port = htons(2048);
	iErr = inet_pton(AF_INET, pcInAddr, &stServAddr.sin_addr);
	if (iErr <= 0)
	{
		return -1;
	}

	while (0 != ConnectRetry(iSocket, (struct sockaddr *)&stServAddr, sizeof(stServAddr)))
	{
		uiCount++;
		printf("Connect retry %d\n", uiCount);
		sleep(1);

		return iSocket;
	}
}

static void Chat(IN int iSocket)
{
	char acBuf[BUFLEN];
	int iErr = -1;
	char *pc = NULL;

	assert(-1 != iSocket);

	printf("client:");
	memset(acBuf, 0, sizeof(acBuf));
	if (NULL == fgets(acBuf, sizeof(acBuf), stdin))
	{
		return;
	}

	iErr = send(iSocket, acBuf, strlen(acBuf), 0);
	if (-1 == iErr)
	{
		printf("Failed to send message\n");
		return;
	}

	memset(acBuf, 0, sizeof(acBuf));
	pc = acBuf;
	iErr = recv(iSocket, pc, sizeof(acBuf), 0);
	if (-1 == iErr)
	{
		return;
	}

	printf("server:%s\n", acBuf);

	return;
}

int main(int argc, char *argv[])
{
	int iSocket = -1;

	if (2 != argc)
	{
		printf("usage: client x.x.x.x\n");
		return 0;
	}

	iSocket = InitClient(argv[1]);
	if (-1 == iSocket)
	{
		return 0;
	}
	printf("connected\n");
	
	while (1)
	{
		Chat(iSocket);
	}

	close(iSocket);

	return 0;
}