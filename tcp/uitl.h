#ifndef UTIL_H
#define UTIL_H

long readn(int isockfd, OUT char *pcdata, long lsize);
long writen(int isockfd, IN const char *apcdata, long lsize);

#endif
