#include "mutest.h"

MUTEST_GROUP(test_result)

MUTEST_TEST(positive_test) { return MUTEST_SUCCESS; }

MUTEST_TEST_WITH_SPECIFIC_RESULT(expect_sucess_result, MUTEST_SUCCESS)
{
	return MUTEST_SUCCESS;
}

MUTEST_TEST_WITH_SPECIFIC_RESULT(expect_error_result, MUTEST_ERROR)
{
	return MUTEST_ERROR;
}
