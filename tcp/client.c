#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"
#include "util.h"

static long connectRetry(int isockfd, IN const struct sockaddr *apstaddr, long lsize)
{
	assert(isockfd >= 0);
	assert(NULL != apstaddr);
	assert(lsize > 0);

	int iseconds;
	for (iseconds = 1; iseconds <= MAX_SLEEP; iseconds <<= 1)
	{
		if (0 == connect(isockfd, apstaddr, lsize))
		{
			LOG_DEBUG("succeed connect fd %d", isockfd);
			return SUCCESS;
		}
		LOG_DEBUG("failed connect fd %d", isockfd);
		sleep(iseconds);
	}

	return FAILED;
}

static long sockfd(const char *apcaddr)
{
	assert(NULL != apcaddr);

	int isockfd;
	if ((isockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG_ERROR("failed create socket fd : %s", strerror(errno));
		return FAILED;
	}
	LOG_DEBUG("succeed create socket fd %d", isockfd);

	struct sockaddr_in staddr;
	memset(&staddr, 0, sizeof(staddr));
	staddr.sin_family = AF_INET;
	staddr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, apcaddr, &staddr.sin_addr) <= 0)
	{
		LOG_ERROR("failed covert ip address %s : %s",apcaddr, strerror(errno));
		return FAILED;
	}

	if (SUCCESS != connectRetry(isockfd, (struct sockaddr *)&staddr, sizeof(staddr)))
	{
		LOG_ERROR("failed connect server %s : %s", apcaddr, strerror(errno));
		close(isockfd);
		return FAILED;
	}
	LOG_DEBUG("client socketdf %d succeed connect to server %s", isockfd, apcaddr);

	return isockfd;
}

static void echo(int isockfd, IN const char *apcmsg, long lsize)
{
	assert(isockfd >= 0);
	assert(NULL != apcmsg);
	assert(lsize > 0);

	if (writen(isockfd, apcmsg, lsize) < 0)
	{
		LOG_ERROR("failed send data : %s", apcmsg);
	} else
	{
		LOG_DEBUG("succeed send data : %s", apcmsg);
		char buf[BUFLEN] = {'\0'};
		if (readn(isockfd, buf, sizeof(buf)) >= 0)
		{
			LOG_DEBUG("succeed get response : %s", buf);
		} else
		{
			LOG_ERROR("failed get response");
		}
	}
}

int main(int argc, char *argv[])
{
	if (3 != argc)
	{
		LOG_ERROR("usage : tcp_client x.x.x.x msg");
		return 0;
	}

	int isockfd;
	if ((isockfd = sockfd(argv[1])) < 0) return 0;
	if (strlen(argv[2]) > 0) echo(isockfd, argv[2], strlen(argv[2]));
	close(isockfd);

	return 0;
}
