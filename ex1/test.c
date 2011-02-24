#include <stdio.h>
#include <stdlib.h>
#include "osm.h"

void testAndPrint(unsigned int numOfIterations)
{
	osm_init(numOfIterations);
	timeMeasurmentStructure t = measureTimes(numOfIterations);
	printf("machine name:\t%s\n",t.machineName);
	printf("num of iterations:\t%d\n",t.numberOfIterations);
	free(t.machineName);
	printf("instructionTimeNanoSecond:\t%f\n",t.instructionTimeNanoSecond);
	printf("functionTimeNanoSecond:\t%f\n",t.functionTimeNanoSecond);
	printf("trapTimeNanoSecond:\t%f\n",t.trapTimeNanoSecond);
	printf("functionInstructionRatio:\t%f\n",t.functionInstructionRatio);
	printf("trapInstructionRatio:\t%f\n",t.trapInstructionRatio);
}

int main()
{
	// test empty func:
	printf("Num of iterations: 1000\n");
	testAndPrint(1000);
	printf("\n");

	printf("Num of iterations: 0\n");
	testAndPrint(0);
	printf("\n");

	printf("Num of iterations: 100000\n");
	testAndPrint(100000);
	printf("\n");

	printf("Num of iterations: 140873\n");
	testAndPrint(140873);
	printf("\n");

	printf("Num of iterations: 200000\n");
	testAndPrint(200000);
	printf("\n");

	printf("Num of iterations: 1000000\n");
	testAndPrint(1000000);
	printf("\n");

	return 0;
}
