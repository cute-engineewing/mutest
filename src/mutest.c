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
mutest_result_t
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
	waitpid(-1, &status, 0);

	return WEXITSTATUS(status);
}

mutest_result_t
mutest_run_single_test(struct mutest_test test)
{
	mutest_result_t result = 0;
	printf("test: %-30s | ", test.name);

	result = run_threaded_test(test);

	if (result != test.expected_result) /* we want a success */
	{
		printf("\033[0;31m"); /* set red color */
		printf("error %i (expected: %i) \n", result, test.expected_result);
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

mutest_result_t
mutest_run_group(struct mutest_group_test *group)
{
	mutest_result_t errors = MUTEST_SUCCESS;
	mutest_result_t current_test_result = 0;
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

mutest_result_t
mutest_run()
{
	mutest_result_t current_test_result = 0;
	size_t success_count = 0;
	mutest_result_t errors = MUTEST_SUCCESS;
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

void
quit_test(mutest_result_t status)
{
	fflush(stdout);
	exit(status);
}

const char *
print_error(const struct mutest_location location)
{
	printf("(file: %s / line: %i) | ", location.file, location.line);
	return "";
}

void
_muexpect(bool condition, struct mutest_location location)
{
	if (!condition)
	{
		printf("error, expected condition to be true");
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_false(bool condition, struct mutest_location location)
{
	if (condition)
	{
		printf("error, expected condition to be false");
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_with_info(bool condition, const char *info,
					struct mutest_location location)
{
	if (!condition)
	{
		printf("error (in: %s) expected condition to be true", info);
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_strings_equal(const char *str1, const char *str2,
						struct mutest_location location)
{
	if (strcmp(str1, str2) != 0)
	{
		printf("error expected '%s' and '%s' equal", str1, str2);
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_equal(long v1, long v2, struct mutest_location location)
{
	if (v1 != v2)
	{
		printf("error expected '%li' and '%li' equal", v1, v2);
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_null(void *pointer, struct mutest_location location)
{
	if (pointer != NULL)
	{
		printf("error expected pointer to be null");
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}

void
_muexpect_not_null(void *pointer, struct mutest_location location)
{
	if (pointer == NULL)
	{
		printf("error expected pointer to not be null");
		print_error(location);
		quit_test(MUTEST_ERROR);
	}
}
/* call order:
 * -> mutest_run():
 * 		-> mutest_run_group():
 * 			-> mutest_run_single_test():
 * 				-> run_threaded_test():
 * 					-> run mutest_test.func()
 *
 */
