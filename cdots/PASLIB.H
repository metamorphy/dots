#ifndef __PASLIB_H__
#define __PASLIB_H__

#define maxint ((1 << (sizeof(integer)*8-1)) - 1)

#ifdef __cplusplus
extern "C" { 
#endif

double sin(double);
double cos(double);
double tan(double);
double atan(double);

double exp(double);
double log(double);

double sqrt(double);

char* lpsz(int low, int high, const char* ptr);

#ifdef __cplusplus
} 
#endif

/*
 * Macros for some Pascal standard functions
 */


#define trunc(x)  ((integer)(x))

#define pred(type,x) ((type)((x) - 1))

#define succ(type,x) ((type)((x) + 1))

#define pack(a_l,a_h,a,i,z) memcpy(z, &(a)[(i)-(a_l)], sizeof(z))

#define unpack(z,a_l,a_h,a,i) memcpy(&(a)[(i)-(a_l)], (z), sizeof(z))

#define bitsize(x) (sizeof(x)*8)

#define odd(x) ((x) & 1)

#define chr(n) ((char)(n))

#define ord(c) ((unsigned char)(c))

#ifdef __cplusplus

#ifndef abs
#if 0 // abs for integers is defined in stdlib
inline integer abs(integer x) { return x < 0 ? -x : x; }
#endif
inline real    abs(real x)    { return  x < 0 ? -x : x; }
#endif

inline integer sqr(integer x) { return x*x; } 
inline real    sqr(real x)    { return x*x; } 

extern "C" void timestamp(integer* day,  integer* month, integer* year, 
			  integer* hour, integer* min,   integer* sec);

extern "C" real get_realtime();

extern "C" integer round(real);

inline void timestamp(integer& day,  integer& month, integer& year, 
		      integer& hour, integer& min,   integer& sec)
{
    timestamp(&day, &month, &year, &hour, &min, &sec);
}

#else

#ifndef abs
#define abs(x) ((x) < 0 ? -(x) : (x))
#endif
#define sqr(x) ((x)*(x))

void timestamp(integer* day,  integer* month, integer* year, 
	       integer* hour, integer* min,   integer* sec);

real get_realtime();

integer round(real);

#endif


#endif
