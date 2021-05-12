#include "mutest.h"
#include <stdbool.h>
MUTEST_GROUP(test_expect)

MUTEST_TEST(expect_positive_result)
{
	muexpect(true);
	return MUTEST_SUCCESS;
}

MUTEST_TEST_WITH_SPECIFIC_RESULT(expect_negative_result, MUTEST_ERROR)
{
	muexpect(false);
	return MUTEST_SUCCESS;
}

MUTEST_TEST(string_equal_positive_result)
{
	muexpect_string_equals("hello", "hello");
	return MUTEST_SUCCESS;
}

MUTEST_TEST_WITH_SPECIFIC_RESULT(string_equal_negative_result, MUTEST_ERROR)
{
	muexpect_string_equals("uwuwu", "owo");
	return MUTEST_SUCCESS;
}

MUTEST_TEST(expect_null_positive_result)
{
	muexpect_null(NULL);
	return MUTEST_SUCCESS;
}

MUTEST_TEST_WITH_SPECIFIC_RESULT(expect_null_negative_result, MUTEST_ERROR)
{
	muexpect_null(((void *)(10)));
	return MUTEST_SUCCESS;
}
