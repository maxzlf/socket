#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"

static long sockfd(const char *apcaddr)
{
	assert(NULL != apcaddr);

	int isockfd;
	if ((isockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
	{
		LOG_ERROR("failed create socket fd : %s", strerror(errno));
		return FAILED;
	}
	LOG_DEBUG("succeed create socket fd %d", isockfd);

	struct sockaddr_un staddr;
	memset(&staddr, 0, sizeof(staddr));
	staddr.sun_family = AF_LOCAL;
	char tmpname[sizeof(int)];
	*(int*)tmpname = rnd(0, 0x7FFFFFFF);
	strncpy(staddr.sun_path, tmpname, sizeof(staddr.sun_path));
	if (0 != bind(isockfd, (struct sockaddr *)&staddr, sizeof(staddr)))
	{
		LOG_ERROR("failed bind socket fd %s: %s", tmpname, strerror(errno));
		close(isockfd);
		return FAILED;
	}

	return isockfd;
}

static void echo(int isockfd, IN const char *apcmsg, long lsize)
{
	assert(isockfd >= 0);
	assert(NULL != apcmsg);
	assert(lsize > 0);

	struct sockaddr_un staddr;
	socklen_t stlen = sizeof(staddr);
	memset(&staddr, 0, sizeof(staddr));
	staddr.sun_family = AF_LOCAL;
	strncpy(staddr.sun_path, UNIX_ADDR, sizeof(staddr.sun_path));

	if (sendto(isockfd, apcmsg, lsize, 0, (struct sockaddr*)&staddr, stlen) < 0)
	{
		LOG_ERROR("failed send data : %s", apcmsg);
	} else
	{
		LOG_DEBUG("succeed send data : %s", apcmsg);
		char buf[BUFLEN] = {'\0'};
		if (recvfrom(isockfd, buf, sizeof(buf), 0, (struct sockaddr*)&staddr, &stlen) >= 0)
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
	if (2 != argc)
	{
		LOG_ERROR("usage : tcp_clien msg");
		return 0;
	}

	init_rnd();
	int isockfd;
	if ((isockfd = sockfd(argv[1])) < 0) return 0;
	if (strlen(argv[1]) > 0) echo(isockfd, argv[1], strlen(argv[1]));
	close(isockfd);

	return 0;
}
