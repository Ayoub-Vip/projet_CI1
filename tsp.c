#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "individual.h"
#include "population.h"
#include "tsp.h"


/* ========================================================================== *
 * 								 Prototypes									  *
 * ========================================================================== */

/* Return the number of 1's contained in a binary sequence.
 *
 * PARAMETERS:
 * - `ind`: the individual representing the binary sequence.
 *
 * RETURNS: a double containing the number of 1's.
 */
static double num1InSeq(Individual *ind, __attribute__((unused)) void *param);

/* Return the value of the function f(x) = -x^2/10 + 3x + 4 at a given x.
 *
 * PARAMETERS:
 * - `ind`: the individual containing the binary reprensation of x.
 *
 * RETURNS: a double containing the value of f(x).
 */
static double f(Individual *ind, __attribute__((unused)) void *param);

// Start the genetic algorithm using `num1InSeq` as fitness function.
static void maximizeNumberOf1(void);

// Start the genetic algorithm using `f` as fitness function
static void maximizeFunction(void);

// Start the genetic algorithm on the travelling salesman problem.
static void tsp(int argc, char const *argv[]);


/* ========================================================================== *
 * 							   Implementation                                 *
 * ========================================================================== */

int main(int argc, char const *argv[]) {

        srand(time(NULL));
  
	if (argc == 1) {
		fprintf(stderr, "Usage: ./testga <mode>.\n");
		exit(EXIT_FAILURE);
	}

	int mode = atoi(argv[1]);
	switch (mode) {
	case 1:
		maximizeNumberOf1();
		break;
	case 2:
		maximizeFunction();
		break;
	case 3:
		tsp(argc, argv);
		break;
	default:
		fprintf(stderr, "<mode> should be 1, 2 or 3, got %d\n", mode);
		exit(EXIT_FAILURE);
	}

	return 0;
}


static double num1InSeq(Individual *ind, __attribute__((unused)) void *param) {
	double x = 0;
	for (int i = 0; i < individualGetLength(ind); i++)
		if (individualGetGene(ind, i) > 0)
			x += 1.0;

	return x;

}

static double f(Individual *ind, __attribute__((unused)) void *param) {
	int x = 0;
	for (int i = 0; i < individualGetLength(ind); i++)
		x = 2*x + individualGetGene(ind, i);

	return -x*x/10.0 + 3.0*x + 4;
}

static void maximizeNumberOf1() {
	Population *pop = populationInit(20, 2, 100,
	                                 num1InSeq, NULL, individualRandomInit,
	                                 individualSeqMutation, 0.01,
	                                 individualSeqCrossOver, 2);


	for (int t = 0; t < 100; t++) {
		populationEvolve(pop);
		printf("Step %3d: Max fitness = %lf\n", t+1, populationGetMaxFitness(pop));
	}

	printf("Best individual found: ");
	individualPrint(stdout, populationGetBestIndividual(pop));
	printf(", with fitness of: %lf\n", populationGetMaxFitness(pop));

	populationFree(pop);
}

static void maximizeFunction() {
	Population *pop = populationInit(5, 2, 6,
	                                 f, NULL, individualRandomInit,
	                                 individualSeqMutation, 0.1,
	                                 individualSeqCrossOver, 2);

	for (int t = 0; t < 30; t++) {
		populationEvolve(pop);
		printf("Step %3d: Max fitness = %lf\n", t+1, populationGetMaxFitness(pop));
	}

	printf("Best individual found: ");
	individualPrint(stdout, populationGetBestIndividual(pop));
	printf(", with fitness of: %lf\n", populationGetMaxFitness(pop));

	populationFree(pop);
}

static void tsp(int argc, char const *argv[]) {
	if (argc != 8) {
		fprintf(stderr, "Usage: ./testga 3 <file> <ntowns> <size> <pm> <elitesize> <niterations>\n");
		exit(EXIT_FAILURE);
	}

	const char *filename = argv[2];
	int nTowns = atoi(argv[3]);
	int size = atoi(argv[4]);
	float pm = atof(argv[5]);
	int eliteSize = atoi(argv[6]);
	int nbIterations = atoi(argv[7]);

	Map *map = tspLoadMapFromFile(filename, nTowns);
	int *bestTour = tspOptimizeByGA(map, nbIterations, size, eliteSize, pm, 1);

	printf("Best tour found: (length = %f)", tspGetTourLength(bestTour, map));
	for (int i = 0; i < nTowns; i++)
		printf(" %d", bestTour[i]);
	printf("\n");

	tspTourToGIF(bestTour, map, "tour.gif", 1000);
}
