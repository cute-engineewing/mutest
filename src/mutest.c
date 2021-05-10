#include "mutest.h"
#include <stdio.h>
#include <sys/wait.h>

#include <unistd.h>

struct mutest_group_test *group_array_test[MAX_GROUP];
size_t current_group_count = 0;

void
_add_group(struct mutest_group_test *target)
{
	target->test_array = malloc(sizeof(struct mutest_test) * MAX_TEST_ON_GROUP);
	group_array_test[current_group_count] = target;
	current_group_count++;
}

void
_add_test(struct mutest_group_test *group, struct mutest_test test)
{
	group->test_array[group->count] = test;
	group->count++;
	printf(" added test: %s on group %s\n", test.name, group->name);
}

/* launch a thread a get its result */
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
mutest_run_group(struct mutest_group_test *group)
{
	int errors = MUTEST_SUCCESS;
	size_t current_test_result = 0;
	size_t success_count = 0;
	size_t i = 0;
	printf("=====================\n");
	printf("running grouped test: %10s \n", group->name);

	for (; i < group->count; i++)
	{
		current_test_result = mutest_run_single_test(group->test_array[i]);

		if (current_test_result == MUTEST_SUCCESS)
		{
			success_count++;
		}

		errors |= current_test_result;
	}

	printf("test %li/%li success \n", success_count, group->count);
	return errors;
}

int
mutest_run()
{
	size_t current_test_result = 0;
	size_t success_count = 0;
	int errors = MUTEST_SUCCESS;
	size_t i = 0;

	for (; i < current_group_count; i++)
	{
		current_test_result = mutest_run_group(group_array_test[i]);

		if (current_test_result == MUTEST_SUCCESS)
		{
			success_count++;
		}

		errors |= current_test_result;
	}
	printf("grouped %li/%li success \n", success_count, current_group_count);
	return errors;
}

/* call order:
 * -> mutest_run():
 * 		-> mutest_run_group():
 * 			-> mutest_run_single_test():
 * 				-> run_threaded_test():
 * 					-> run mutest_test.func()
 *
 */