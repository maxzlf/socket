#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"

static long sockfd()
{
	int isockfd;
	if ((isockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
	{
		LOG_ERROR("failed create socket fd: %s", strerror(errno));
		return FAILED;
	}
	LOG_DEBUG("succeed create socket fd : %d", isockfd);

	struct sockaddr_un staddr;
	memset(&staddr, 0, sizeof(staddr));
	staddr.sun_family = AF_LOCAL;
	strncpy(staddr.sun_path, UNIX_ADDR, sizeof(staddr.sun_path));
	if (0 != bind(isockfd, (struct sockaddr *)&staddr, sizeof(staddr)))
	{
		LOG_ERROR("failed bind socket fd with name %d: %s", UNIX_ADDR, strerror(errno));
		close(isockfd);
		return FAILED;
	}

	LOG_DEBUG("succeed bind socket fd %d with port %d", isockfd, UNIX_ADDR);
	return isockfd;
}

static void echo(int isockfd)
{
	assert(isockfd >= 0);

	struct sockaddr_in staddr;
	memset(&staddr, 0, sizeof(staddr));
	socklen_t stlen = sizeof(staddr);
	char buf[BUFLEN] = {'\0'};
	long revlen;
	if ((revlen = recvfrom(isockfd, buf, sizeof(buf), 0, (struct sockaddr*)&staddr, &stlen)) >= 0)
	{
		LOG_DEBUG("succeed read data : %s", buf);
		if (sendto(isockfd, buf, revlen, 0, (struct sockaddr*)&staddr, stlen) < 0)
		{
			LOG_ERROR("failed response data : %s", buf);
		} else
		{
			LOG_DEBUG("succeed response data : %s", buf);
		}
	} else
	{
		LOG_ERROR("failed read data");
	}
}

int main(int argc, char *argv[])
{
	long isockfd;
	if ((isockfd = sockfd()) < 0) return 0;

	for (;;)
	{
		echo(isockfd);
	}
	close(isockfd);

	return 0;
}
