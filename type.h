#ifndef _TYPE_H_
#define _TYPE_H_

#ifndef		PUBLIC
#define		PUBLIC
#endif

#ifndef		PRIVATE
#define		PRIVATE		static
#endif

#ifndef		IN 
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef		INOUT
#define		INOUT
#endif

#ifndef		BOOL_TRUE
#define		BOOL_TRUE	1
#endif

#ifndef		BOOL_FALSE
#define 	BOOL_FALSE	0
#endif

typedef	int 			BOOL_T;
typedef char			BYTE;
typedef char			CHAR;
typedef unsigned char 	UCHAR;
typedef short			SHORT; 
typedef unsigned short 	USHORT;
typedef int 			INT; 
typedef unsigned int 	UINT;
typedef long 			LONG;
typedef unsigned long  	ULONG;
typedef float			FLOAT;
typedef double			DOUBLE; 
typedef long double 	LDOUBLE;

#ifndef		ERROR_SUCCESS
#define 	ERROR_SUCCESS	0
#endif

#ifndef		ERROR_FAILED
#define		ERROR_FAILED	1
#endif

#endif
