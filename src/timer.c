// timer.c
// for timing performance

#define BILLION 1000000000

long long unsigned minus_time(struct timespec *a, struct timespec *b)
{               //returns a - b in nanosecs
    long long unsigned result = BILLION;
    result *= (a->tv_sec - b->tv_sec);
    result += (a->tv_nsec - b->tv_nsec);
    return (result);
}

// From https://gist.github.com/jbenet/1087739
// Get the time on either OSX or linux

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


void current_utc_time(struct timespec *ts)
{
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}
