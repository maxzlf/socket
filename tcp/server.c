#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"
#include "util.h"

static long listenfd()
{
	int ilistenfd;
	struct sockaddr_in staddr;

	if ((ilistenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG_ERROR("failed create socket : %s", strerror(errno));
		return FAILED;
	}
	LOG_DEBUG("succeed create listen fd %d", ilistenfd);

	memset(&staddr, 0, sizeof(staddr));
	staddr.sin_family = AF_INET;
	staddr.sin_port = htons(SERVER_PORT);
	staddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (0 != bind(ilistenfd, (struct sockaddr *)&staddr, sizeof(staddr)))
	{
		LOG_ERROR("failed bind listenfd with port : %s", strerror(errno));
		close(ilistenfd);
		return FAILED;
	}

	if (0 != listen(ilistenfd, SOMAXCONN))
	{
		LOG_ERROR("failed listen listenfd %d : %s", ilistenfd, strerror(errno));
		close(ilistenfd);
		return FAILED;
	}

	LOG_DEBUG("succeed listen listenfd %d", ilistenfd);
	return ilistenfd;
}

static void echo(int isockfd)
{
	assert(isockfd >= 0);

	char buf[BUFLEN] = {'\0'};
	long revlen;
	if ((revlen = readn(isockfd, buf, sizeof(buf))) >= 0)
	{
		LOG_DEBUG("succedd read data : %s", buf);
		if (writen(isockfd, buf, revlen) < 0)
		{
			LOG_ERROR("failed response data : %s", buf);
		} else
		{
			LOG_DEBUG("succedd response data : %s", buf);
		}
	} else
	{
		LOG_ERROR("failed read data");
	}
}

int main(int argc, char *argv[])
{
	long ilistenfd, iconnectfd;
	if ((ilistenfd = listenfd()) < 0) return 0;

	for (;;)
	{
		struct sockaddr_in staddr;
		socklen_t stlen = sizeof(staddr);
		memset(&staddr, 0, stlen);
		if ((iconnectfd = accept(ilistenfd, (struct sockaddr*)&staddr, &stlen)) < 0)
		{
			LOG_ERROR("failed accept connection : %s", strerror(errno));
			break;
		}
		echo(iconnectfd);
		close(iconnectfd);
	}
	close(ilistenfd);

	return 0;
}
