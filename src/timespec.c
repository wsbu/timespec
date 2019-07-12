/* Functions for working with timespec structures
 * Written by Daniel Collins (2017)
 * 
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
*/

/** \file timespec.c
 *  \brief Functions for working with timespec structures.
 * 
 * This library aims to provide a comprehensive set of functions with
 * well-defined behaviour that handle all edge cases (e.g. negative values) in
 * a sensible manner.
 *
 * Negative values are allowed in the tv_sec and/or tv_usec field of timespec
 * structures, tv_usec is always relative to tv_sec, so mixing positive and
 * negative values will produce consistent results:
 *
 * <PRE>
 * { tv_sec = 1,  tv_nsec = 500000000  } ==  1.5 seconds
 * { tv_sec = 1,  tv_nsec = 0          } ==  1.0 seconds
 * { tv_sec = 1,  tv_nsec = -500000000 } ==  0.5 seconds
 * { tv_sec = 0,  tv_nsec = 500000000  } ==  0.5 seconds
 * { tv_sec = 0,  tv_nsec = 0          } ==  0.0 seconds
 * { tv_sec = 0,  tv_nsec = -500000000 } == -0.5 seconds
 * { tv_sec = -1, tv_nsec = 500000000  } == -0.5 seconds
 * { tv_sec = -1, tv_nsec = 0          } == -1.0 seconds
 * { tv_sec = -1, tv_nsec = -500000000 } == -1.5 seconds
 * </PRE>
 *
 * Furthermore, any timespec structure processed or returned by library functions
 * is normalised according to the rules in timespec_normalise().
*/

#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include "timespec.h"

/** \fn struct timespec timespec_add(struct timespec ts1, struct timespec ts2)
 *  \brief Returns the result of adding two timespec structures.
*/
struct timespec timespec_add(struct timespec ts1, struct timespec ts2)
{
	/* Normalise inputs to prevent tv_nsec rollover if whole-second values
	 * are packed in it.
	*/
	ts1 = timespec_normalise(ts1);
	ts2 = timespec_normalise(ts2);
	
	ts1.tv_sec  += ts2.tv_sec;
	ts1.tv_nsec += ts2.tv_nsec;
	
	return timespec_normalise(ts1);
}

/** \fn struct timespec timespec_sub(struct timespec ts1, struct timespec ts2)
 *  \brief Returns the result of subtracting ts2 from ts1.
*/
struct timespec timespec_sub(struct timespec ts1, struct timespec ts2)
{
	/* Normalise inputs to prevent tv_nsec rollover if whole-second values
	 * are packed in it.
	*/
	ts1 = timespec_normalise(ts1);
	ts2 = timespec_normalise(ts2);
	
	ts1.tv_sec  -= ts2.tv_sec;
	ts1.tv_nsec -= ts2.tv_nsec;
	
	return timespec_normalise(ts1);
}

/** \fn bool timespec_eq(struct timespec ts1, struct timespec ts2)
 *  \brief Returns true if the two timespec structures are equal.
*/
bool timespec_eq(struct timespec ts1, struct timespec ts2)
{
	return (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec == ts2.tv_nsec);
}

/** \fn bool timespec_gt(struct timespec ts1, struct timespec ts2)
 *  \brief Returns true if ts1 is greater than ts2.
*/
bool timespec_gt(struct timespec ts1, struct timespec ts2)
{
	return (ts1.tv_sec > ts2.tv_sec || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec > ts2.tv_nsec));
}

/** \fn bool timespec_ge(struct timespec ts1, struct timespec ts2)
 *  \brief Returns true if ts1 is greater than or equal to ts2.
*/
bool timespec_ge(struct timespec ts1, struct timespec ts2)
{
	return (ts1.tv_sec > ts2.tv_sec || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec >= ts2.tv_nsec));
}

/** \fn bool timespec_lt(struct timespec ts1, struct timespec ts2)
 *  \brief Returns true if ts1 is less than ts2.
*/
bool timespec_lt(struct timespec ts1, struct timespec ts2)
{
	return (ts1.tv_sec < ts2.tv_sec || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec < ts2.tv_nsec));
}

/** \fn bool timespec_le(struct timespec ts1, struct timespec ts2)
 *  \brief Returns true if ts1 is less than or equal to ts2.
*/
bool timespec_le(struct timespec ts1, struct timespec ts2)
{
	return (ts1.tv_sec < ts2.tv_sec || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec <= ts2.tv_nsec));
}

/** \fn struct timespec timespec_from_double(double s)
 *  \brief Converts a fractional number of seconds to a timespec.
*/
struct timespec timespec_from_double(double s)
{
	struct timespec ts = {
		.tv_sec  = s,
		.tv_nsec = (s - (long)(s)) * 1000000000,
	};
	
	return timespec_normalise(ts);
}

/** \fn double timespec_to_double(struct timespec ts)
 *  \brief Converts a timespec to a fractional number of seconds.
*/
double timespec_to_double(struct timespec ts)
{
	return ((double)(ts.tv_sec) + ((double)(ts.tv_nsec) / 1000000000));
}

/** \fn struct timespec timespec_from_timeval(struct timeval tv)
 *  \brief Converts a timeval to a timespec.
*/
struct timespec timespec_from_timeval(struct timeval tv)
{
	struct timespec ts = {
		.tv_sec  = tv.tv_sec,
		.tv_nsec = tv.tv_usec * 1000
	};
	
	return timespec_normalise(ts);
}

/** \fn struct timeval timespec_to_timeval(struct timespec ts)
 *  \brief Converts a timespec to a timeval.
*/
struct timeval timespec_to_timeval(struct timespec ts)
{
	ts = timespec_normalise(ts);
	
	struct timeval tv = {
		.tv_sec  = ts.tv_sec,
		.tv_usec = ts.tv_nsec / 1000,
	};
	
	return tv;
}

/** \fn struct timespec timespec_from_ms(long milliseconds)
 *  \brief Converts an integer number of milliseconds to a timespec.
*/
struct timespec timespec_from_ms(long milliseconds)
{
	struct timespec ts = {
		.tv_sec  = (milliseconds / 1000),
		.tv_nsec = (milliseconds % 1000) * 1000000,
	};
	
	return timespec_normalise(ts);
}

/** \fn long timespec_to_ms(struct timespec ts)
 *  \brief Converts a timespec to an integer number of milliseconds.
*/
long timespec_to_ms(struct timespec ts)
{
	return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

/** \fn struct timespec timespec_normalise(struct timespec ts)
 *  \brief Normalises a timespec structure.
 *
 * Returns a normalised version of a timespec structure, according to the
 * following rules:
 *
 * 1) If tv_nsec is >1,000,000,00 or <-1,000,000,000, flatten the surplus
 *    nanoseconds into the tv_sec field.
 *
 * 2) If tv_sec is >0 and tv_nsec is <0, decrement tv_sec and roll tv_nsec up
 *    to represent the same value on the positive side of the new tv_sec.
 *
 * 3) If tv_sec is <0 and tv_nsec is >0, increment tv_sec and roll tv_nsec down
 *    to represent the same value on the negative side of the new tv_sec.
*/
struct timespec timespec_normalise(struct timespec ts)
{
	while(ts.tv_nsec >= 1000000000)
	{
		++(ts.tv_sec);
		ts.tv_nsec -= 1000000000;
	}
	
	while(ts.tv_nsec <= -1000000000)
	{
		--(ts.tv_sec);
		ts.tv_nsec += 1000000000;
	}
	
	if(ts.tv_nsec < 0 && ts.tv_sec > 0)
	{
		/* Negative nanoseconds while seconds is positive.
		 * Decrement tv_sec and roll tv_nsec over.
		*/
		
		--(ts.tv_sec);
		ts.tv_nsec = 1000000000 - (-1 * ts.tv_nsec);
	}
	else if(ts.tv_nsec > 0 && ts.tv_sec < 0)
	{
		/* Positive nanoseconds while seconds is negative.
		 * Increment tv_sec and roll tv_nsec over.
		*/
		
		++(ts.tv_sec);
		ts.tv_nsec = -1000000000 - (-1 * ts.tv_nsec);
	}
	
	return ts;
}

#ifdef TEST

#endif
