#ifndef TSP_H
#define TSP_H


typedef struct Map_t Map;


/* ========================================================================== *
 * 								 Prototypes									  *
 * ========================================================================== */

/* Load a Map from a csv file containing a list of towns with their coordinates.
 *
 * PARAMETERS:
 * - `filename`: the name of the file containing the towns;
 * - `nbTowns`: the number of towns to load from the file.
 *
 * RETURNS: the map.
 */
Map *tspLoadMapFromFile(const char *filename, int nbTowns);

/* Deallocate the memory associated to a map.
 *
 * PARAMETERS:
 * - `map`: the map.
 */
void tspFreeMap(Map *map);

/* Generate a GIF image from a map and a given tour of this map.
 *
 * PARAMETERS:
 * - `tour`: the tour of the map, i.e. the visiting order of the towns;
 * - `map`: the map;
 * - `gifName`: the name of the GIF image;
 * - `size`: the size of the image.
 */
void tspTourToGIF(int *tour, Map *map, const char *gifName, int size);

/* Get the total length of a tour of a map.
 *
 * PARAMETERS:
 * - `tour`: the tour of the map, i.e. the visiting order of the towns;
 * - `map`: the map.
 *
 * RETURNS: the tour's length.
 */
double tspGetTourLength(int *tour, Map *map);

/* Find a good map's tour using the genetic algorithm.
 *
 * PARAMETERS:
 * - `map`: the map;
 * - `nbIterations`: the number of iterations to make;
 * - `sizePopulation`: the number of individuals in the population;
 * - `eliteSize`: the number of best ranked individuals that will be kept in the
 *                next generation.
 * - `pmutation`: the mutation's probability;
 * - `verbose`: 0 to disable verbose, 1 to enable it.
 *
 * RETURNS: the best obtained tour.
 */
int *tspOptimizeByGA(Map *map, int nbIterations, int sizePopulation,
                     int eliteSize, float pmutation, int verbose);


#endif
