#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H


typedef struct Individual_t Individual;


/* ========================================================================== *
 * 								 Prototypes									  *
 * ========================================================================== */

/* Create a new individual.
 *
 * PARAMETERS:
 * - `length`: the length of the individual's genotype;
 * - `nbVal`: the number of values that can be taken by the individual's genes.
 *
 * RETURNS: a pointer to the new individual.
 */
Individual *individualCreate(int length, int nbVal);

/* Deallocate the memory associated to a given individual.
 *
 * PARAMETERS:
 * - `ind`: the individual.
 */
void individualFree(Individual *ind);

/* Return the length of an individual's genotype.
 *
 * PARAMETERS:
 * - `ind`: the individual.
 *
 * RETURNS: the length of the genotype.
 */
int individualGetLength(Individual *ind);

/* Return the value of an individual's gene at a given index.
 *
 * PARAMETERS:
 * - `ind`: the individual;
 * - `i`: the gene's index.
 *
 * RETURNS: the value of the gene.
 */
int individualGetGene(Individual *ind, int i);

/* Change the value of a given individual's gene.
 *
 * PARAMETERS:
 * - `ind`: the individual;
 * - `i`: the gene's index;
 * - `val`: the new value of the gene.
 */
void individualSetGene(Individual *ind, int i, int val);

/* Randomly initialize an individual's genotype by taking random values for each
 * gene.
 *
 * PARAMETERS:
 * - `ind`: the individual.
 */
void individualRandomInit(Individual *ind);

/* Randomly initialize an individual's genotype by making a random permutation
 * of the ordered list (0, ..., length-1).
 *
 * PARAMETERS:
 * - `ind`: the individual.
 */
void individualRandomPermInit(Individual *ind);

/* Create a copy of a given individual.
 *
 * PARAMETERS:
 * - `ind`: the individual.
 *
 * RETURNS: the copy.
 */
Individual *individualCopy(Individual *ind);

/* Print in a file or a stream the genotype of an individual, with the following
 * pattern: "g_0, g_1, ..., g_l" where g_i is the ith gene.
 *
 * PARAMETERS:
 * - `fp`: a file pointer;
 * - `ind`: the individual
 */
void individualPrint(FILE *fp, Individual *ind);

/* Perform a random mutation on an individual as described in the section 1 of
 * the statement.
 *
 * PARAMETERS:
 * - `ind`: the individual;
 * - `pm`: the mutation probability.
 */
void individualSeqMutation(Individual *ind, float pm);

/* Perform a crossover between two individuals as described in the section 1 of
 * the statement.
 *
 * PARAMETERS:
 * - `parent1`: the first parent;
 * - `parent2`: the second parent.
 *
 * RETURNS: the child created from the crossover.
 */
Individual* individualSeqCrossOver(Individual *parent1, Individual *parent2);

/* Perform a random mutation on an individual as described in the section 2 of
 * the statement.
 *
 * PARAMETERS:
 * - `ind`: the individual;
 * - `pm`: the mutation probability.
 */
void individualPermMutation(Individual *ind, float pm);

/* Perform a crossover between two individuals as described in the section 2 of
 * the statement.
 *
 * PARAMETERS:
 * - `parent1`: the first parent;
 * - `parent2`: the second parent.
 *
 * RETURNS: the child created from the crossover.
 */
Individual* individualPermCrossOver(Individual *parent1, Individual *parent2);

#endif
