#include <stdlib.h>
#include <stdio.h> // TODO
#include <unistd.h>
#include <sys/time.h>

#include "osm.h"

#define DEFAULT_ITERATION_COUNT 50000
#define MACHINE_NAME_MAX_LEN 30
#define LOOP_UNROLLING_FACTOR 5
#define RET_SUCCESS 0
#define RET_FAILURE -1
#define ERROR_VALUE -1

int _iterations = DEFAULT_ITERATION_COUNT;

void dummy_func() { /* Intentionally left blank */ }

static double osm_time_difference_ns(const struct timeval* const before, const struct timeval* const after)
{
	long beforens = before->tv_sec*1000000000 + before->tv_usec*1000;
	long afterns = after->tv_sec*1000000000 + after->tv_usec*1000;
	return afterns - beforens;
}

static double osm_empty_loop_time(int iterations)
{
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < iterations; ++i) { /* Blank */ }
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	return osm_time_difference_ns(&tvBefore, &tvAfter);
}

static double osm_assignment_loop_time(int iterations)
{
	int temp[5] = {0};
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < iterations; ++i) {
		temp[0] = 1;
		temp[1] = 2;
		temp[2] = 3;
		temp[3] = 4;
		temp[4] = 5;
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	return osm_time_difference_ns(&tvBefore, &tvAfter);
}

/* Initialization function that the user must call
 * before running any other library function.
 * Returns 0 uppon success and -1 on failure
 */
int osm_init(unsigned int osm_iterations)
{
	if (0 < osm_iterations) {
		_iterations = osm_iterations;
		return RET_SUCCESS;
	} else {
		return RET_FAILURE;
	}
}

/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time()
{
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int unrolledIterations = _iterations / LOOP_UNROLLING_FACTOR;
	(_iterations % LOOP_UNROLLING_FACTOR) ? unrolledIterations++ : unrolledIterations;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < unrolledIterations; ++i) {
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}

	return (osm_time_difference_ns(&tvBefore, &tvAfter) - osm_empty_loop_time(unrolledIterations)) / (unrolledIterations * LOOP_UNROLLING_FACTOR);
}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time()
{
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int unrolledIterations = _iterations / LOOP_UNROLLING_FACTOR;
	(_iterations % LOOP_UNROLLING_FACTOR) ? unrolledIterations++ : unrolledIterations;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < unrolledIterations; ++i) {
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}

	return (osm_time_difference_ns(&tvBefore, &tvAfter) - osm_empty_loop_time(unrolledIterations)) / (unrolledIterations * LOOP_UNROLLING_FACTOR);
}


/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time()
{
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int temp[5] = {0};
	int i = 0;
	int unrolledIterations = _iterations / LOOP_UNROLLING_FACTOR;
	(_iterations % LOOP_UNROLLING_FACTOR) ? unrolledIterations++ : unrolledIterations;
	printf("unrolledIterations = %d\n", unrolledIterations);
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < unrolledIterations; ++i) {
		temp[0]++;
		temp[1]++;
		temp[2]++;
		temp[3]++;
		temp[4]++;
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	return (osm_time_difference_ns(&tvBefore, &tvAfter) - osm_assignment_loop_time(unrolledIterations)) / (unrolledIterations * LOOP_UNROLLING_FACTOR);
}

timeMeasurmentStructure measureTimes (int numOfIterations)
{
	timeMeasurmentStructure tms = {0};
	tms.machineName = (char*)malloc(MACHINE_NAME_MAX_LEN);
	if (NULL != tms.machineName) {
		int retVal = gethostname(tms.machineName, MACHINE_NAME_MAX_LEN);
		if (0 != retVal) {
			free(tms.machineName);
			tms.machineName = NULL;
		}
	}
	if (0 < numOfIterations) {
		_iterations = numOfIterations;
	} else {
		_iterations = DEFAULT_ITERATION_COUNT;
	}
	tms.numberOfIterations = _iterations;
	tms.functionTimeNanoSecond = osm_function_time();
	tms.instructionTimeNanoSecond = osm_operation_time();
	tms.trapTimeNanoSecond = osm_syscall_time();
	if ((-1 != tms.instructionTimeNanoSecond) && (0 != tms.instructionTimeNanoSecond)) {
		if (-1 != tms.functionTimeNanoSecond) {
			tms.functionInstructionRatio = tms.functionTimeNanoSecond / tms.instructionTimeNanoSecond;
		} else {
			tms.functionInstructionRatio = ERROR_VALUE;
		}
		if (-1 != tms.trapInstructionRatio) {
			tms.trapInstructionRatio = tms.trapTimeNanoSecond / tms.instructionTimeNanoSecond;
		} else {
			tms.trapInstructionRatio = ERROR_VALUE;
		}
	} else {
		tms.functionInstructionRatio = ERROR_VALUE;
		tms.trapInstructionRatio = ERROR_VALUE;
	}

	return tms;
}
