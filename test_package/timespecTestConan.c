/**
 * @file timespecTestConan.cpp
 *
 * @copyright (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered
 * trademarks of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.
 */

#include <timespec.h>

int main () {
    struct timespec ts1 = { .tv_sec = 1 };
    struct timespec ts2 = { .tv_sec = 2 };
    struct timespec result = timespec_add(ts1, ts2);
    return 3 != result.tv_sec;
}
