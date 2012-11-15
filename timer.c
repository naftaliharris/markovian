// timer.c
// for timing performance

#define BILLION 1000000000

long long unsigned minus_time(struct timespec *a, struct timespec *b){//returns a - b in nanosecs
        long long unsigned result = BILLION;
	result *= (a->tv_sec - b->tv_sec);
        result += (a->tv_nsec - b->tv_nsec);
        return(result);
}
