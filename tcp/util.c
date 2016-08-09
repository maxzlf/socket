#include "../utils/sys.h"
#include "../utils/type.h"
#include "../utils/error.h"
#include "../utils/const.h"
#include "../utils/utils.h"
#include "util.h"

long readn(int isockfd, OUT char *pcdata, long lsize)
{
    assert(isockfd >= 0);
	assert(pcdata != NULL);
	assert(lsize > 0);

	ssize_t n;
	while ((n = recv(isockfd, pcdata, lsize, 0)) < 0 && EINTR == errno)
	{
		LOG_ERROR("read error : %s", strerror(errno));
	}

	if (n < 0)
    {
        LOG_ERROR("failed read data");
        return FAILED;
    } else
    {
        LOG_DEBUG("received %ld bytes of data.", n);
        return n;
    }
}

long writen(int isockfd, IN const char *apcdata, long lsize)
{
    assert(isockfd >= 0);
	assert(apcdata != NULL);
	assert(lsize > 0);

	const char *snddata = apcdata;
	long nleft = lsize;
    ssize_t n;
	while (nleft > 0)
	{
		if ((n = send(isockfd, snddata, nleft, 0)) < 0)
		{
			if (EINTR != errno)
			{
				LOG_ERROR("write error : %s", strerror(errno));
				return FAILED;
			} else pass;
		} else
		{
			snddata += n;
			nleft -= n;
		}
	}
	LOG_DEBUG("send %ld bytes of data.", lsize);
	return lsize;
}
