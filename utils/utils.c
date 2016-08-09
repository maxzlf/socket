#include "sys.h"
#include "type.h"
#include "error.h"
#include "const.h"
#include "utils.h"

void init_rnd()
{
	srandom(time(NULL));
}

int rnd(int begin, int end)
{
	assert(end > begin);
	return random() % (end-begin) + begin;
}

void print_log(enum LOG_LEVEL level, const char* filename, const char* func_name, int line, const char* fmt, ...)
{
	char* pclevel;
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        pclevel = "[DEBUG]";
        break;
    case LOG_LEVEL_INFO:
		pclevel = "[INFO]";
        break;
    case LOG_LEVEL_WARN:
		pclevel = "[WARNING]";
        break;
    case LOG_LEVEL_ERROR:
		pclevel = "[ERROR]";
        break;
    default:
		pclevel = "INFO";
        break;
    }

	time_t rawtime;
	time(&rawtime);

    fprintf(stderr, "%s %s, %s, %d | %s", pclevel, filename, func_name, line, ctime(&rawtime));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	printf("\n");
	va_end(ap);
    fflush(stderr);
}
