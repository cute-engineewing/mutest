#include "mutest.h"
#include <stdbool.h>
mutest_group(test_expect)

	mutest_test(expect_positive_result)
{
	mutest_expect(true);
	return MUTEST_SUCCESS;
}

mutest_test_with_specific_result(expect_negative_result, MUTEST_ERROR)
{
	mutest_expect(false);
	return MUTEST_SUCCESS;
}

mutest_test(string_equal_positive_result)
{
	mutest_string_equal("hello", "hello");
	return MUTEST_SUCCESS;
}

mutest_test_with_specific_result(string_equal_negative_result, MUTEST_ERROR)
{
	mutest_string_equal("uwuwu", "owo");
	return MUTEST_SUCCESS;
}

mutest_test(expect_null_positive_result)
{
	mutest_expect_null(NULL);
	return MUTEST_SUCCESS;
}

mutest_test_with_specific_result(expect_null_negative_result, MUTEST_ERROR)
{
	mutest_expect_null(((void*)(10)));
	return MUTEST_SUCCESS;
}
