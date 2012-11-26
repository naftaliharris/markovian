// test.c
// runs all tests

#include <stdio.h>
#include <stdlib.h>

#include "fenio_test.c"

void use_result(int test_failures, char *testname, int *failures)
{
    *failures += test_failures;

    if(test_failures == 0)
    {
        fprintf(stderr, "Passed: %s\n", testname);
    } 
    else {
        fprintf(stderr, "FAILED: %s\n", testname);
        fprintf(stderr, "        %d errors.\n", test_failures);
    }
}

int main(void)
{
    int failures = 0;
    int verbose_errors = 1;
    
    // run the tests
    use_result(fenio_test(verbose_errors), "fenio.c", &failures);

    // print the summary
    fprintf(stderr, "\nSUMMARY: %d errors.\n", failures);

    return failures;
}
