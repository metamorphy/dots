#ifndef __PTOC_H__
#define __PTOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define items(x) (sizeof(x)/sizeof(*(x)))

typedef int           integer;
typedef unsigned      cardinal;
typedef double        real;

typedef unsigned char boolean; 
#define true          (1)
#define false         (0)

#define nil           NULL

/*
 * Pascal runtime library headers
 */

#include "paslib.h"
//#include "io.h"
//#include "set.h"
//#include "array.h"

#endif
