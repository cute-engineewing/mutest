#include "mutest.h"
#include <stdio.h>
#include <sys/wait.h>

#include <unistd.h>

int
run_threaded_test(struct mutest_test test)
{
	pid_t child;
	int status;
	fflush(stdout);
	child = fork();
	if (child == 0)
	{
		exit(test.func());
	}
	waitpid(child, &status, 0);

	return status;
}

int
mutest_run_single_test(struct mutest_test test)
{
	int result;
	printf("test: %-30s | ", test.name);

	result = run_threaded_test(test);

	if (result != MUTEST_SUCCESS) /* we want a success */
	{
		printf("\033[0;31m"); /* set red color */
		printf("error %i\n", result);
		printf("\033[0m"); /* reset */
		return MUTEST_ERROR;
	}

	else
	{
		printf("\033[0;32m"); /* set green color */
		printf("success \n");
		printf("\033[0m"); /* reset */
		return MUTEST_SUCCESS;
	}
}

int
imp_mutest_run_grouped_test(struct mutest_test *test, size_t length,
							const char *name)
{
	size_t i = 0;
	size_t current_test_result = 0;
	size_t success_count = 0;
	int error_count = MUTEST_SUCCESS;

	printf("running grouped test: %10s \n", name);

	for (; i < length; i++)
	{
		/* store the current test result in a temporary variable to check the individual test result instead of all results combined */
		current_test_result = mutest_run_single_test(test[i]);
		
		if(current_test_result == MUTEST_SUCCESS)
		{
			success_count++;
		}
		
		error_count |= current_test_result;
	}
	printf("grouped test %li/%li success \n", success_count, length);
	return error_count;
}
