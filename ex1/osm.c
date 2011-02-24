#include <stdlib.h>
#include <stdio.h> // TODO
#include <unistd.h>
#include <sys/time.h>

#include "osm.h"

#define DEFAULT_ITERATION_COUNT 50000
#define MACHINE_NAME_MAX_LEN 30
#define LOOP_UNROLLING_FACTOR 16
#define RET_SUCCESS 0
#define RET_FAILURE -1
#define ERROR_VALUE -1

int _iterations = DEFAULT_ITERATION_COUNT;

void dummy_func() { /* Intentionally left blank */ }

static double osm_time_difference_ns(const struct timeval* const before, const struct timeval* const after)
{
	//printf("Before: %ld s %ld ms  ;  After: %ld s %ld ms\n", before->tv_sec, before->tv_usec, after->tv_sec, after->tv_usec);
	long beforens = before->tv_sec*1000000000 + before->tv_usec*1000;
	long afterns = after->tv_sec*1000000000 + after->tv_usec*1000;
	//printf("After - Before: %ld ns\n", afterns-beforens);
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

/*static double osm_assignment_loop_time(int iterations)
{
	//printf("*** Begin assignment loop measurement ***\n");
	//int temp[LOOP_UNROLLING_FACTOR] = {0};
	int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,
			i9=0,i10=0,i11=0,i12=0,i13=0,i14=0,i15=0,i16=0;
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < iterations; ++i) {
		temp[0] = 1;// temp[0] = 2;
		temp[1] = 2;// temp[1] = 3;
		temp[2] = 3;// temp[2] = 4;
		temp[3] = 4;// temp[3] = 5;
		temp[4] = 5;// temp[4] = 6;
		temp[5] = 4;// temp[5] = 5;
		temp[6] = 3;// temp[6] = 4;
		temp[7] = 2;// temp[7] = 3;
		temp[8] = 1;// temp[8] = 4;
		temp[9] = 0;// temp[9] = 9;
		temp[10] = 1;// temp[10] = 2;
		temp[11] = 2;// temp[11] = 4;
		temp[12] = 3;// temp[12] = 5;
		temp[13] = 4;// temp[13] = 8;
		temp[14] = 5;// temp[14] = 9;
		temp[15] = 4;// temp[15] = 0;
		i1=1; i2=2; i3=3; i4=4; i5=5; i6=6; i7=7; i8=8;
		i9=1; i10=2; i11=3; i12=4; i13=5; i14=6; i15=7; i16=8;
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	//printf("*** End assignment loop measurement ***\n");
	return osm_time_difference_ns(&tvBefore, &tvAfter);
}*/

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
	//printf("*** Begin function time measurement ***\n");
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	int i = 0;
	int unrolledIterations = _iterations / LOOP_UNROLLING_FACTOR;
	(_iterations % LOOP_UNROLLING_FACTOR) ? unrolledIterations++ : unrolledIterations;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < unrolledIterations; ++i) {
		dummy_func(); /* 1 */
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func();
		dummy_func(); /* 16 */
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	//printf("*** End function time measurement ***\n");
	double emptyLoopTime = osm_empty_loop_time(unrolledIterations);
	if (ERROR_VALUE == emptyLoopTime) {
		return RET_FAILURE;
	}
	return (osm_time_difference_ns(&tvBefore, &tvAfter) - emptyLoopTime) / (double)(unrolledIterations * LOOP_UNROLLING_FACTOR);
}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time()
{
	//printf("*** Begin syscall time measurement ***\n");
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
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
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
	double emptyLoopTime = osm_empty_loop_time(unrolledIterations);
	if (ERROR_VALUE == emptyLoopTime) {
		return RET_FAILURE;
	}
	return (osm_time_difference_ns(&tvBefore, &tvAfter) - emptyLoopTime) / (double)(unrolledIterations * LOOP_UNROLLING_FACTOR);
}


/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time()
{
	//printf("*** Begin instruction time measurement ***\n");
	struct timeval tvBefore = {0};
	struct timeval tvAfter = {0};
	//int temp[LOOP_UNROLLING_FACTOR] = {0};
	int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,
				i9=0,i10=0,i11=0,i12=0,i13=0,i14=0,i15=0,i16=0;
	int i = 0;
	int unrolledIterations = _iterations / LOOP_UNROLLING_FACTOR;
	(_iterations % LOOP_UNROLLING_FACTOR) ? unrolledIterations++ : unrolledIterations;
	int retValBefore = gettimeofday(&tvBefore, NULL);
	for (; i < unrolledIterations; ++i) {
		/*temp[0] += 4;
		temp[1] += 9;
		temp[2] += 15;
		temp[3] += 2;
		temp[4] += 7;
		temp[5] += 10;
		temp[6] += 1;
		temp[7] += 8;
		temp[8] += 21;
		temp[9] += 19;
		temp[10] += 5;
		temp[11] += 3;
		temp[12] += 6;
		temp[13] += 18;
		temp[14] += 13;
		temp[15] += 17;*/
		i1+=1; i2+=2; i3+=3; i4+=4; i5+=5; i6+=6; i7+=7; i8+=8;
			i9+=1; i10+=2; i11+=3; i12+=4; i13+=5; i14+=6; i15+=7; i16+=8;
	}
	int retValAfter = gettimeofday(&tvAfter, NULL);
	if ((-1 == retValBefore) || (-1 == retValAfter)) {
		return RET_FAILURE;
	}
	double emptyLoopTime = osm_empty_loop_time(unrolledIterations);
	if (ERROR_VALUE == emptyLoopTime) {
		return RET_FAILURE;
	}
	/*double assignmentLoopTime = osm_assignment_loop_time(unrolledIterations);
	if (ERROR_VALUE == assignmentLoopTime) {
		return RET_FAILURE;
	}*/
	//printf ("Time diff: (%f - %f) / (%f) = %f\n", time_difference, assignment_loop_time, (double)(unrolledIterations * LOOP_UNROLLING_FACTOR), ((time_difference - assignment_loop_time) / (double)(unrolledIterations * LOOP_UNROLLING_FACTOR)));
	//printf("*** End instruction time measurement ***\n");
	return ((osm_time_difference_ns(&tvBefore, &tvAfter) - emptyLoopTime) / (double)(unrolledIterations * LOOP_UNROLLING_FACTOR));
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
	//printf("instruction: %f  function: %f  syscall: %f\n", tms.instructionTimeNanoSecond, tms.functionTimeNanoSecond, tms.trapTimeNanoSecond);
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
