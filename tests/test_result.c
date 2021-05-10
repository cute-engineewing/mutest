#include "mutest.h"

mutest_group(test_result)

	mutest_test(positive_test)
{
	return MUTEST_SUCCESS;
}

mutest_test_with_specific_result(expect_sucess_result, MUTEST_SUCCESS)
{
	return MUTEST_SUCCESS;
}

mutest_test_with_specific_result(expect_error_result, MUTEST_ERROR)
{
	return MUTEST_ERROR;
}