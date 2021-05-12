#ifndef _MULIB_MUTEST_H
#define _MULIB_MUTEST_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEST_ON_GROUP 128
#define MAX_GROUP		  256

typedef int mutest_result_t;
typedef mutest_result_t (*mutest_func_t)();
struct mutest_test
{
	mutest_func_t func;
	const char *name;
	mutest_result_t expected_result;
};

struct mutest_group_test
{
	const char *name;
	size_t count;
	struct mutest_test *test_array;
};

struct mutest_location
{
	int line;
	const char *file;
};

#define MUTEST_CURRENT_LOCATION                                                \
	(struct mutest_location) { __LINE__, __FILE__ }

#define MUTEST_SUCCESS 0
#define MUTEST_ERROR   1

void mutest_add_group(struct mutest_group_test *target);
void mutest_add_test(struct mutest_group_test *group, struct mutest_test test);

/* only 1 group per file, used to group test, for exemple test on string, or
 * test on vector */
#define MUTEST_GROUP(group_name)                                               \
	static struct mutest_group_test __mutest_group__ = { #group_name, 0, 0 };  \
                                                                               \
	__attribute__((constructor)) static void group_name##group_wrapper(void)   \
	{                                                                          \
		mutest_add_group(&__mutest_group__);                                   \
	}

/* add a test on a group */
#define MUTEST_TEST(test)                                                      \
	mutest_result_t test##func();                                              \
	/* test wrapper is the function that will add the test to the list */      \
	__attribute__((constructor)) static void test##func_wrapper(void)          \
	{                                                                          \
		mutest_add_test(                                                       \
			&__mutest_group__,                                                 \
			(struct mutest_test){ test##func, #test, MUTEST_SUCCESS });        \
	}                                                                          \
                                                                               \
	mutest_result_t test##func()

/* like a mutest_test but with a wrapper around for checking the result */
#define MUTEST_TEST_WITH_SPECIFIC_RESULT(test, expected_result)                \
	mutest_result_t test##func();                                              \
	/* test wrapper is the function that will add the test to the list */      \
	__attribute__((constructor)) static void test##func_wrapper(void)          \
	{                                                                          \
		mutest_add_test(                                                       \
			&__mutest_group__,                                                 \
			(struct mutest_test){ test##func, #test, expected_result });       \
	}                                                                          \
                                                                               \
	mutest_result_t test##func()

void _muexpect(bool condition, struct mutest_location location);

void _muexpect_false(bool condition, struct mutest_location location);

void _muexpect_with_info(bool condition, const char *info,
						 struct mutest_location location);

void _muexpect_strings_equal(const char *str1, const char *str2,
							 struct mutest_location location);

void _muexpect_equal(long v1, long v2, struct mutest_location location);

void _muexpect_null(void *pointer, struct mutest_location location);

void _muexpect_not_null(void *pointer, struct mutest_location location);

#define muexpect(condition) _muexpect(condition, MUTEST_CURRENT_LOCATION)

#define muexpect_false(condition)                                              \
	_muexpect_false(condition, MUTEST_CURRENT_LOCATION)

#define muexpect_with_info(condition, info)                                    \
	_muexpect_with_info(condition, info, MUTEST_CURRENT_LOCATION)

#define muexpect_string_equals(str1, str2)                                     \
	_muexpect_strings_equal(str1, str2, MUTEST_CURRENT_LOCATION)

#define muexpect_equal(i1, i2)                                                 \
	_muexpect_string_equals(i1, i2, MUTEST_CURRENT_LOCATION)

#define muexpect_null(pointer) _muexpect_null(pointer, MUTEST_CURRENT_LOCATION)

#define muexpect_not_null(pointer)                                             \
	_muexpect_not_null(pointer, MUTEST_CURRENT_LOCATION)

/* run all test */
mutest_result_t mutest_run();

#endif /* _MULIB_MUTEST_H */
