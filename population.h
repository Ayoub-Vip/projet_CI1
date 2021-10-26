#ifndef POPULATION_H
#define POPULATION_H

#include "individual.h"


typedef struct Population_t Population;


/* ========================================================================== *
 * 								 Prototypes									  *
 * ========================================================================== */

/* Create a new population of individuals.
 *
 * PARAMETERS:
 * - `length`: the length of the individuals' genotype;
 * - `nbVal`: the number of values that can be taken by the individuals' genes;
 * - `size`: the number of individuals in the population;
 * - `fitness`: a pointer to the fitness function;
 * - `paramFitness`: a void pointer to optional parameters that can be passed to the fitness function;
 * - `init`: a pointer to the function that initializes the individuals;
 * - `mutation`: a pointer to the mutation function;
 * - `crossover`: a pointer to the crossover function;
 * - `eliteSize`: the number of best ranked individuals that will be kept in the
 *                next generation (expected to be > 0).
 *
 * RETURNS: the newly created population
 */
Population *populationInit(int length, int nbVal, int size,
                           double (*fitness)(Individual *, void *),
			   void *paramFitness,
                           void (*init)(Individual *),
                           void (*mutation)(Individual *, float), float pmutation,
                           Individual* (*crossover)(Individual *, Individual *),
                           int eliteSize);

/* Get the maximum fitness value obtained by an individual in a given population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 *
 * RETURNS: the maximum fitness value.
 */
double populationGetMaxFitness(Population *pop);

/* Get the minimum fitness value obtained by an individual in a given population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 *
 * RETURNS: the minimum fitness value.
 */
double populationGetMinFitness(Population *pop);

/* Get the average of the fitness values obtained by the individuals in a given
 * population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 *
 * RETURNS: the average fitness value.
 */
double populationGetAvgFitness(Population *pop);

/* Get the individual with the highest fitness value of a given population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 *
 * RETURNS: the best individual.
 */
Individual *populationGetBestIndividual(Population *pop);

/* Deallocate the memory associated to a given population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 */
void populationFree(Population *pop);

/* Sample an individual from a population according to the fitness values.
 *
 * PARAMETERS:
 * - `pop`: the population.
 *
 * RETURNS: the sampled individual.
 */
Individual *populationSelection(Population *pop);

/* Generate the next generation of a population.
 *
 * PARAMETERS:
 * - `pop`: the population.
 */
void populationEvolve(Population *pop);

#endif
