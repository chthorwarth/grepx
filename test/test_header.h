//
// Created by luke on 08.01.26.
//

#ifndef GREPX_TEST_HEADER_H
#define GREPX_TEST_HEADER_H

#define RUN_TEST(test_fn)                    \
    do                                       \
    {                                        \
        total_tests++;                       \
        if (test_fn() != 0) {                \
            failed_tests++;                  \
            printf("[FAIL] %s\n", #test_fn); \
        }                                    \
        else {                               \
            printf("[ OK ] %s\n", #test_fn); \
        }                                    \
    } while (0)                              \


static int total_tests = 0;
static int failed_tests = 0;

#endif //GREPX_TEST_HEADER_H