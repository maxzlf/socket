#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"

static long sockfd(const char *apcaddr)
{
	assert(NULL != apcaddr);

	int isockfd;
	if ((isockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		LOG_ERROR("failed create socket fd : %s", strerror(errno));
		return FAILED;
	}
	LOG_DEBUG("succeed create socket fd %d", isockfd);

	return isockfd;
}

static void echo(int isockfd, IN const char *apcmsg, long lsize, const char *apcaddr)
{
	assert(isockfd >= 0);
	assert(NULL != apcmsg);
	assert(lsize > 0);

	struct sockaddr_in staddr;
	socklen_t stlen = sizeof(staddr);
	memset(&staddr, 0, sizeof(staddr));
	staddr.sin_family = AF_INET;
	staddr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, apcaddr, &staddr.sin_addr) <= 0)
	{
		LOG_ERROR("failed covert ip address %s : %s",apcaddr, strerror(errno));
		return;
	}

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
	if (3 != argc)
	{
		LOG_ERROR("usage : tcp_client x.x.x.x msg");
		return 0;
	}

	int isockfd;
	if ((isockfd = sockfd(argv[1])) < 0) return 0;
	if (strlen(argv[2]) > 0) echo(isockfd, argv[2], strlen(argv[2]), argv[1]);
	close(isockfd);

	return 0;
}
