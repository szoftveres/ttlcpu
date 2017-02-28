#ifndef __LOGG_H__
#define __LOGG_H__

#include <stdarg.h>

void logg (const char* fmt, ...);
void logg_init (int argc, char **argv);

#endif

