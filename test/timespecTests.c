/**
 * @file timespecTestConan.cpp
 *
 * @copyright (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered
 * trademarks of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.
 */

#include <timespec.h>
#include <stdio.h>

#define TEST_NORMALISE(ts_sec, ts_nsec, expect_sec, expect_nsec) { \
	struct timespec in  = { .tv_sec = ts_sec, .tv_nsec = ts_nsec }; \
	struct timespec got = timespec_normalise(in); \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_normalise({%ld, %ld}) returned wrong values\n", (long)(ts_sec), (long)(ts_nsec)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_ADD(ts1_sec, ts1_nsec, ts2_sec, ts2_nsec, expect_sec, expect_nsec) { \
	struct timespec ts1 = { .tv_sec = ts1_sec, .tv_nsec = ts1_nsec }; \
	struct timespec ts2 = { .tv_sec = ts2_sec, .tv_nsec = ts2_nsec }; \
	struct timespec got = timespec_add(ts1, ts2); \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_add({%ld, %ld}, {%ld, %ld}) returned wrong values\n", \
			(long)(ts1_sec), (long)(ts1_nsec), (long)(ts2_sec), (long)(ts2_nsec)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_SUB(ts1_sec, ts1_nsec, ts2_sec, ts2_nsec, expect_sec, expect_nsec) { \
	struct timespec ts1 = { .tv_sec = ts1_sec, .tv_nsec = ts1_nsec }; \
	struct timespec ts2 = { .tv_sec = ts2_sec, .tv_nsec = ts2_nsec }; \
	struct timespec got = timespec_sub(ts1, ts2); \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_sub({%ld, %ld}, {%ld, %ld}) returned wrong values\n", \
			(long)(ts1_sec), (long)(ts1_nsec), (long)(ts2_sec), (long)(ts2_nsec)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_TEST_FUNC(func, ts1_sec, ts1_nsec, ts2_sec, ts2_nsec, expect) { \
	struct timespec ts1 = { .tv_sec = ts1_sec, .tv_nsec = ts1_nsec }; \
	struct timespec ts2 = { .tv_sec = ts2_sec, .tv_nsec = ts2_nsec }; \
	if(func(ts1, ts2) != expect) { \
		printf(#func "({%ld, %ld}, {%ld, %ld}) returned %s\n", \
			(long)(ts1_sec), (long)(ts1_nsec), (long)(ts2_sec), (long)(ts2_nsec), \
			(expect ? "FALSE" : "TRUE")); \
		result = 1; \
	} \
}

#define TEST_FROM_DOUBLE(d_secs, expect_sec, expect_nsec) { \
	struct timespec got = timespec_from_double(d_secs);  \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_from_double(%f) returned wrong values\n", (double)(d_secs)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_TO_DOUBLE(ts_sec, ts_nsec, expect) { \
	struct timespec ts = { .tv_sec = ts_sec, .tv_nsec = ts_nsec }; \
	double got = timespec_to_double(ts); \
	if(got != expect) { \
		printf("timespec_to_double({%ld, %ld}) returned wrong value\n", (long)(ts_sec), (long)(ts_nsec)); \
		printf("    Expected: %f\n", (double)(expect)); \
		printf("    Got:      %f\n", got); \
		result = 1; \
	} \
}

#define TEST_FROM_TIMEVAL(in_sec, in_usec, expect_sec, expect_nsec) { \
	struct timeval tv = { .tv_sec = in_sec, .tv_usec = in_usec }; \
	struct timespec got = timespec_from_timeval(tv); \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_from_timeval({%ld, %ld}) returned wrong values\n", (long)(in_sec), (long)(in_usec)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_TO_TIMEVAL(ts_sec, ts_nsec, expect_sec, expect_usec) { \
	struct timespec ts = { .tv_sec = ts_sec, .tv_nsec = ts_nsec }; \
	struct timeval got = timespec_to_timeval(ts); \
	if(got.tv_sec != expect_sec || got.tv_usec != expect_usec) \
	{ \
		printf("timespec_to_timeval({%ld, %ld}) returned wrong values\n", (long)(ts_sec), (long)(ts_nsec)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_usec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_usec)); \
		result = 1; \
	} \
}

#define TEST_FROM_MS(msecs, expect_sec, expect_nsec) { \
	struct timespec got = timespec_from_ms(msecs);  \
	if(got.tv_sec != expect_sec || got.tv_nsec != expect_nsec) \
	{ \
		printf("timespec_from_ms(%ld) returned wrong values\n", (long)(msecs)); \
		printf("    Expected: {%ld, %ld}\n", (long)(expect_sec), (long)(expect_nsec)); \
		printf("    Got:      {%ld, %ld}\n", (long)(got.tv_sec), (long)(got.tv_nsec)); \
		result = 1; \
	} \
}

#define TEST_TO_MS(ts_sec, ts_nsec, expect) { \
	struct timespec ts = { .tv_sec = ts_sec, .tv_nsec = ts_nsec }; \
	long got = timespec_to_ms(ts); \
	if(got != expect) { \
		printf("timespec_to_ms({%ld, %ld}) returned wrong value\n", (long)(ts_sec), (long)(ts_nsec)); \
		printf("    Expected: %ld\n", (long)(expect)); \
		printf("    Got:      %ld\n", got); \
		result = 1; \
	} \
}

int main()
{
    int result = 0;

    // timespec_add

    TEST_ADD(0,0,         0,0,         0,0);
    TEST_ADD(0,0,         1,0,         1,0);
    TEST_ADD(1,0,         0,0,         1,0);
    TEST_ADD(1,0,         1,0,         2,0);
    TEST_ADD(1,500000000, 1,0,         2,500000000);
    TEST_ADD(1,0,         1,500000000, 2,500000000);
    TEST_ADD(1,500000000, 1,500000000, 3,0);
    TEST_ADD(1,500000000, 1,499999999, 2,999999999);
    TEST_ADD(1,500000000, 1,500000000, 3,0);
    TEST_ADD(1,999999999, 1,999999999, 3,999999998);
    TEST_ADD(0,500000000, 1,500000000, 2,0);
    TEST_ADD(1,500000000, 0,500000000, 2,0);

    // timespec_sub

    TEST_SUB(0,0,         0,0,         0,0);
    TEST_SUB(1,0,         0,0,         1,0);
    TEST_SUB(1,0,         1,0,         0,0);
    TEST_SUB(1,500000000, 0,500000000, 1,0);
    TEST_SUB(5,500000000, 2,999999999, 2,500000001);
    TEST_SUB(0,0,         1,0,         -1,0);
    TEST_SUB(0,500000000, 1,500000000, -1,0);
    TEST_SUB(0,0,         1,500000000, -1,-500000000);
    TEST_SUB(1,0,         1,500000000, 0,-500000000);
    TEST_SUB(1,0,         1,499999999, 0,-499999999);

    // timespec_eq

    TEST_TEST_FUNC(timespec_eq, 0,0,    0,0,    true);
    TEST_TEST_FUNC(timespec_eq, 100,0,  100,0,  true);
    TEST_TEST_FUNC(timespec_eq, -200,0, -200,0, true);
    TEST_TEST_FUNC(timespec_eq, 0,300,  0,300,  true);
    TEST_TEST_FUNC(timespec_eq, 0,-400, 0,-400, true);

    TEST_TEST_FUNC(timespec_eq, 100,1,  100,0,  false);
    TEST_TEST_FUNC(timespec_eq, 101,0,  100,0,  false);
    TEST_TEST_FUNC(timespec_eq, -100,0, 100,0,  false);
    TEST_TEST_FUNC(timespec_eq, 0,10,   0,-10,  false);

    // timespec_gt

    TEST_TEST_FUNC(timespec_gt, 1,0, 0,0,  true);
    TEST_TEST_FUNC(timespec_gt, 0,0, -1,0, true);
    TEST_TEST_FUNC(timespec_gt, 0,1, 0,0,  true);
    TEST_TEST_FUNC(timespec_gt, 0,0, 0,-1, true);

    TEST_TEST_FUNC(timespec_gt, 1,0,  1,0, false);
    TEST_TEST_FUNC(timespec_gt, 1,1,  1,1, false);
    TEST_TEST_FUNC(timespec_gt, -1,0, 0,0, false);
    TEST_TEST_FUNC(timespec_gt, 0,-1, 0,0, false);

    // timespec_ge

    TEST_TEST_FUNC(timespec_ge, 1,0, 0,0,  true);
    TEST_TEST_FUNC(timespec_ge, 0,0, -1,0, true);
    TEST_TEST_FUNC(timespec_ge, 0,1, 0,0,  true);
    TEST_TEST_FUNC(timespec_ge, 0,0, 0,-1, true);
    TEST_TEST_FUNC(timespec_ge, 1,0,  1,0, true);
    TEST_TEST_FUNC(timespec_ge, 1,1,  1,1, true);

    TEST_TEST_FUNC(timespec_ge, -1,0, 0,0, false);
    TEST_TEST_FUNC(timespec_ge, 0,-1, 0,0, false);

    // timespec_lt

    TEST_TEST_FUNC(timespec_lt, 0,0,  1,0, true);
    TEST_TEST_FUNC(timespec_lt, -1,0, 0,0, true);
    TEST_TEST_FUNC(timespec_lt, 0,0,  0,1, true);
    TEST_TEST_FUNC(timespec_lt, 0,-1, 0,0, true);

    TEST_TEST_FUNC(timespec_lt, 1,0, 1,0,  false);
    TEST_TEST_FUNC(timespec_lt, 1,1, 1,1,  false);
    TEST_TEST_FUNC(timespec_lt, 0,0, -1,0, false);
    TEST_TEST_FUNC(timespec_lt, 0,0, 0,-1, false);

    // timespec_le

    TEST_TEST_FUNC(timespec_le, 0,0, 1,0,  true);
    TEST_TEST_FUNC(timespec_le, -1,0, 0,0, true);
    TEST_TEST_FUNC(timespec_le, 0,0, 0,1,  true);
    TEST_TEST_FUNC(timespec_le, 0,-1, 0,0, true);
    TEST_TEST_FUNC(timespec_le, 1,0,  1,0, true);
    TEST_TEST_FUNC(timespec_le, 1,1,  1,1, true);

    TEST_TEST_FUNC(timespec_le, 0,0, -1,0, false);
    TEST_TEST_FUNC(timespec_le, 0,0, 0,-1, false);

    // timespec_from_double

    TEST_FROM_DOUBLE(0.0,   0,0);
    TEST_FROM_DOUBLE(10.0,  10,0);
    TEST_FROM_DOUBLE(-10.0, -10,0);
    TEST_FROM_DOUBLE(0.5,   0,500000000);
    TEST_FROM_DOUBLE(-0.5,  0,-500000000);
    TEST_FROM_DOUBLE(10.5,  10,500000000);
    TEST_FROM_DOUBLE(-10.5, -10,-500000000);

    // timespec_to_double

    TEST_TO_DOUBLE(0,0,            0.0);
    TEST_TO_DOUBLE(10,0,           10.0);
    TEST_TO_DOUBLE(-10,0,          -10.0);
    TEST_TO_DOUBLE(0,500000000,    0.5);
    TEST_TO_DOUBLE(0,-500000000,   -0.5);
    TEST_TO_DOUBLE(10,500000000,   10.5);
    TEST_TO_DOUBLE(10,-500000000,  9.5);
    TEST_TO_DOUBLE(-10,500000000,  -9.5);
    TEST_TO_DOUBLE(-10,-500000000, -10.5);

    // timespec_from_timeval

    TEST_FROM_TIMEVAL(0,0,     0,0);
    TEST_FROM_TIMEVAL(1,0,     1,0);
    TEST_FROM_TIMEVAL(1000,0,  1000,0);
    TEST_FROM_TIMEVAL(0,0,     0,0);
    TEST_FROM_TIMEVAL(-1,0,    -1,0);
    TEST_FROM_TIMEVAL(-1000,0, -1000,0);

    TEST_FROM_TIMEVAL(1,1,      1,1000);
    TEST_FROM_TIMEVAL(1,1000,   1,1000000);
    TEST_FROM_TIMEVAL(1,-1,     0,999999000);
    TEST_FROM_TIMEVAL(1,-1000,  0,999000000);
    TEST_FROM_TIMEVAL(-1,-1,    -1,-1000);
    TEST_FROM_TIMEVAL(-1,-1000, -1,-1000000);

    // timespec_to_timeval

    TEST_TO_TIMEVAL(0,0,   0,0);
    TEST_TO_TIMEVAL(1,0,   1,0);
    TEST_TO_TIMEVAL(10,0,  10,0);
    TEST_TO_TIMEVAL(-1,0,  -1,0);
    TEST_TO_TIMEVAL(-10,0, -10,0);

    TEST_TO_TIMEVAL(1,1,       1,0);
    TEST_TO_TIMEVAL(1,999,     1,0);
    TEST_TO_TIMEVAL(1,1000,    1,1);
    TEST_TO_TIMEVAL(1,1001,    1,1);
    TEST_TO_TIMEVAL(1,2000,    1,2);
    TEST_TO_TIMEVAL(1,2000000, 1,2000);

    TEST_TO_TIMEVAL(1,-1,       0,999999);
    TEST_TO_TIMEVAL(1,-999,     0,999999);
    TEST_TO_TIMEVAL(1,-1000,    0,999999);
    TEST_TO_TIMEVAL(1,-1001,    0,999998);
    TEST_TO_TIMEVAL(1,-2000,    0,999998);
    TEST_TO_TIMEVAL(1,-2000000, 0,998000);

    TEST_TO_TIMEVAL(-1,-1,       -1,0);
    TEST_TO_TIMEVAL(-1,-999,     -1,0);
    TEST_TO_TIMEVAL(-1,-1000,    -1,-1);
    TEST_TO_TIMEVAL(-1,-1001,    -1,-1);
    TEST_TO_TIMEVAL(-1,-2000,    -1,-2);
    TEST_TO_TIMEVAL(-1,-2000000, -1,-2000);

    TEST_TO_TIMEVAL(1,1500000000,   2,500000);
    TEST_TO_TIMEVAL(1,-1500000000,  0,-500000);
    TEST_TO_TIMEVAL(-1,-1500000000, -2,-500000);

    // timespec_from_ms

    TEST_FROM_MS(0,     0,0);
    TEST_FROM_MS(1,     0,1000000);
    TEST_FROM_MS(-1,    0,-1000000);
    TEST_FROM_MS(1500,  1,500000000);
    TEST_FROM_MS(-1000, -1,0);
    TEST_FROM_MS(-1500, -1,-500000000);

    // timespec_to_ms

    TEST_TO_MS(0,0,            0);
    TEST_TO_MS(10,0,           10000);
    TEST_TO_MS(-10,0,          -10000);
    TEST_TO_MS(0,500000000,    500);
    TEST_TO_MS(0,-500000000,   -500);
    TEST_TO_MS(10,500000000,   10500);
    TEST_TO_MS(10,-500000000,  9500);
    TEST_TO_MS(-10,500000000,  -9500);
    TEST_TO_MS(-10,-500000000, -10500);

    // timespec_normalise

    TEST_NORMALISE(0,0,           0,0);

    TEST_NORMALISE(0,1000000000,  1,0);
    TEST_NORMALISE(0,1500000000,  1,500000000);
    TEST_NORMALISE(0,-1000000000, -1,0);
    TEST_NORMALISE(0,-1500000000, -1,-500000000);

    TEST_NORMALISE(5,1000000000,   6,0);
    TEST_NORMALISE(5,1500000000,   6,500000000);
    TEST_NORMALISE(-5,-1000000000, -6,0);
    TEST_NORMALISE(-5,-1500000000, -6,-500000000);

#if (64 <= __WORDSIZE)
    /* These tests use more than 32-bits for the nanosecond field and are
       therefore not expected to succeed on 32-bit platforms */
    TEST_NORMALISE(0,2000000000,  2,0);
    TEST_NORMALISE(0,2500000000,  2,500000000);
    TEST_NORMALISE(0,-2000000000, -2,0);
    TEST_NORMALISE(0,-2500000000, -2,-500000000);

    TEST_NORMALISE(1,-500000001,  0,499999999);
    TEST_NORMALISE(1,-500000000,  0,500000000);
    TEST_NORMALISE(1,-499999999,  0,500000001);

    TEST_NORMALISE(-1,500000000,  0,-500000000);
    TEST_NORMALISE(-1,499999999,  0,-500000001);
#endif

    if(result > 0)
    {
        printf("%d tests failed\n", result);
    }
    else{
        printf("All tests passed\n");
    }

    return result;
}
