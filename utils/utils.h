#ifndef UTILS_H
#define UTILS_H

#define MAX(a,b) \
    ({                              \
        __typeof__ (a) _a = (a);    \
        __typeof__ (b) _b = (b);    \
        _a > _b ? _a : _b;          \
    })

#define MIN(a,b) \
    ({ 	    		                \
        __typeof__ (a) _a = (a);    \
        __typeof__ (b) _b = (b);    \
        _a < _b ? _a : _b;          \
    })

#define TMAX(a,b,c) \
    ({                              \
        __typeof__ (a) _a = (a);    \
        __typeof__ (b) _b = (b);    \
        __typeof__ (c) _c = (c);    \
        (_a < _b? (_b < _c? _c : _b) : (_a < _c? _c : _a)); \
    })

#define TMIN(a,b,c) \
    ({                              \
        __typeof__ (a) _a = (a);    \
        __typeof__ (b) _b = (b);    \
        __typeof__ (c) _c = (c);    \
        (_a < _b? (_a < _c? _a : _c) : (_b < _c? _b : _c)); \
    })

#define TMIDDLE(a,b,c) \
    ({                              \
        __typeof__ (a) _a = (a);    \
        __typeof__ (b) _b = (b);    \
        __typeof__ (c) _c = (c);    \
        (_a < _b? (_b < _c? _b : (_a < _c? _c : _a)) : (_a < _c? _a : (_c < _b? _b : _c))); \
    })

#define SWAP(a,b) \
    ({				                \
    	__typeof__ (a) t = (a);		\
    	(a) = (b);	                \
    	(b) = t;		            \
    })

#define pass    do{}while(0)

#define UNREACHABLE ((void)printf("%s, %s, %u : touched unreachable line.\n", __FILE__, __func__, __LINE__), abort())

#define ISODD(n) ((n)&0x01)

#define ARRAYLEN(arrayName) (sizeof(arrayName) / sizeof((arrayName)[0]))

void init_rnd();
int rnd(int begin, int end);

enum LOG_LEVEL
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

void print_log(enum LOG_LEVEL level, const char* filename, const char* func_name, int line, const char* fmt, ...);

#define LOG_DEBUG(...) print_log(LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(...) print_log(LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(...) print_log(LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(...) print_log(LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOG_FATAL(...) print_log(LOG_LEVEL_FATAL, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

typedef struct S_NODE
{
    int value;
    struct S_NODE *pstnext;
} snode;

long readn(int isockfd, OUT char *pcdata, long ulsize);
long writen(int isockfd, IN const char *apcdata, long ulsize);

#endif
