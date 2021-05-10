#ifndef _MULIB_MUTEST_H
#define _MULIB_MUTEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEST_ON_GROUP 128
#define MAX_GROUP		  256

typedef int (*mutest_func_t)();
struct mutest_test
{
	mutest_func_t func;
	const char *name;
};

struct mutest_group_test
{
	const char *name;
	size_t count;
	struct mutest_test *test_array;
};

#define MUTEST_SUCCESS 0
#define MUTEST_ERROR   1

void _add_group(struct mutest_group_test *target);
void _add_test(struct mutest_group_test *group, struct mutest_test test);

/* only 1 group per file, used to group test, for exemple test on string, or
 * test on vector */
#define mutest_group(group_name)                                               \
	static struct mutest_group_test group = { #group_name, 0, 0 };                    \
                                                                               \
	__attribute__((constructor)) static void group_name##group_wrapper(void)   \
	{                                                                          \
		_add_group(&group);                                                    \
	}

/* add a test on a group */
#define mutest_test(test)                                                      \
	int test##func();                                                          \
	/* test wrapper is the function that will add the test to the list */      \
	__attribute__((constructor)) static void test##func_wrapper(void)          \
	{                                                                          \
		_add_test(&group, (struct mutest_test){ test##func, #test });          \
	}                                                                          \
                                                                               \
	int test##func()

/* like a mutest_test but with a wrapper around for checking the result */
#define mutest_test_with_specific_result(test, expected_result)                \
	int test##func();                                                          \
	int test##wrapper_check(void)                                              \
	{                                                                          \
		int result = test##func();                                             \
		if (result != expected_result)                                         \
		{                                                                      \
			printf("error expected test with result: %i but got %i \n",           \
				   expected_result, result);                                   \
			return MUTEST_ERROR;                                               \
		}                                                                      \
		return MUTEST_SUCCESS;                                                 \
	}                                                                          \
	/* test wrapper is the function that will add the test to the list */      \
	__attribute__((constructor)) static void test##func_wrapper(void)          \
	{                                                                          \
		_add_test(&group, (struct mutest_test){ test##wrapper_check, #test });          \
	}                                                                          \
                                                                               \
	int test##func()

/* error if (condition) == false */
#define mutest_expect(condition)                                               \
	if (!(condition))                                                          \
	{                                                                          \
		printf("expected %-10s | ", #condition);                               \
		return MUTEST_ERROR;                                                   \
	}

/* error if (condition) == false and print "explain" */
#define mutest_expect_with_info(condition, explain)                            \
	if (!(condition))                                                          \
	{                                                                          \
		printf("expected %-10s : %-10s | ", #condition, explain);              \
		return MUTEST_ERROR;                                                   \
	}

#define mutest_string_equal(str1, str2)                                        \
	if ((strcmp(str1, str2) != 0))                                             \
	{                                                                          \
		printf("expected %s == %-10s | ", str1, str2);                         \
		return MUTEST_ERROR;                                                   \
	}

#define mutest_expect_null(value)	  mutest_expect(value == NULL)
#define mutest_expect_not_null(value) mutest_expect(value != NULL)

/* run all test */
int mutest_run();

#endif /* _MULIB_MUTEST_H */
