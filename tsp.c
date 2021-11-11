#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "gifenc.h"
#include "population.h"
#include "individual.h"
#include "tsp.h"


// Opaque structure of a map.
struct Map_t {
	int nbTowns;
	double *x;
	double *y;
};


static void terminate(const char *message);

static void getField(char *inputstring, char *fields[3]);


static void terminate(const char *message) {
	fprintf(stderr,"%s\n", message);
	exit(EXIT_FAILURE);
}

static void getField(char *inputstring, char *fields[3]) {
	fields[0] = strtok (inputstring, ",");
	for (int i =1; i < 3; i++) {
		fields[i] = strtok (NULL, ",");
	}
}

Map *tspLoadMapFromFile(const char *filename, int nbTowns) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
		terminate("loadMapFromFile: file can not be opened");

	char line[1024];

	Map *map = malloc(sizeof(Map));

	if (map== NULL)
		exit(EXIT_FAILURE);

	map->nbTowns = nbTowns;
	map->x = malloc(nbTowns * sizeof(double));
	map->y = malloc(nbTowns * sizeof(double));

	int i = 0;
	while (i< nbTowns && fgets(line, 1024, fp)) {
		char *fields[4];
		getField(line, fields);
		map->x[i] = strtod(fields[1], NULL);
		map->y[i] = strtod(fields[2], NULL);
		i++;
	}

	printf("Loaded %d towns from %s\n", nbTowns, filename);

	return map;
}

void tspFreeMap(Map *map) {
	free(map->x);
	free(map->y);
	free(map);
}

void tspTourToGIF(int *tour, Map *map, const char *filename, int size) {

  double minX = map->x[0];
  double maxX = minX;
  double minY = map->y[0];
  double maxY = minY;

  for (int pos = 0; pos < map->nbTowns; pos++) {
    
    double currentX = map->x[pos];
    double currentY = map->y[pos];
    
    if (currentX < minX)
      minX = currentX;
    else if (currentX > maxX)
      maxX = currentX;
    
    if (currentY < minY)
      minY = currentY;
    else if (currentY > maxY)
      maxY = currentY;
  }
  
  double xRange = maxX - minX;
  double yRange = maxY - minY;
  
  int sizex, sizey;
  if (xRange > yRange) {
    sizex = size;
    sizey = size*yRange/xRange;
  } else {
    sizex = size*xRange/yRange;
    sizey = size;
  }

  ge_GIF *gif = ge_new_gif(filename,
			   sizex+10, sizey+10,     /* w=1000, h=700 */
			   (uint8_t []) {          /* palette */
			     0xFF, 0xFF, 0xFF,   /* 0 -> white */
			       0xFF, 0x00, 0x00,   /* 1 -> red */
			       0x00, 0x00, 0xFF,   /* 2 -> blue */
			       0x00, 0x00, 0x00    /* 3 -> black */
			       },
			   2,                      /* palette depth == log2(# of colors) */
			   1                       /* Play it once */
			   );
  
  double prevX = (map->x[tour[map->nbTowns-1]] - minX)*sizex/xRange+5;
  double prevY = (map->y[tour[map->nbTowns-1]] - minY)*sizey/yRange+5;
  double nextX, nextY;

  // clean
  for (int j = 0; j < (gif->w * gif->h); j++)
    gif->frame[j] = 0;
  
  for (int pos = 0; pos < map->nbTowns; pos++) {
    nextX = (map->x[tour[pos]] - minX)*sizex/xRange+5;
    nextY = (map->y[tour[pos]] - minY)*sizey/yRange+5;
    ge_draw_line(gif, (int) prevX, (int) prevY, (int) nextX, (int) nextY, 2);
    prevX = nextX;
    prevY = nextY;
  }

  ge_add_frame(gif, 0);
  ge_close_gif(gif);  
}

double tspGetTourLength(int *tour, Map *map) {
	// Implementation
	
	double tourLength;
	int i = 0;

	for (; i < map->nbTowns; ++i)  {

		int villeNum = tour[i];
		tourLength += sqrt(pow(map->x[villeNum] - map->x[villeNum+1], 2) + pow(map->y[villeNum] - map->y[villeNum+1], 2));
	}

	tourLength += sqrt(pow(map->x[tour[i]] - map->x[tour[0]]) + pow(map->y[tour[i]] - map->y[tour[0]]));

	return tourLength;

	return 0.0;
}

static double fitness( int* tour, Map* map) {
	
	return (double)1/tspGetTourLength( tour, map);
}

int *tspOptimizeByGA(Map *map, int nbIterations, int sizePopulation, int eliteSize, float pmutation, int verbose) {
	
	double nbTowns = map->nbTowns;
	
	Population *pop = populationInit( nbTowns, nbTowns, sizePopulation, fitness, map,
	                   individualRandomPermInit, individualPermMutation,pmutation, individualPermCrossOver, eliteSize);
	
	for( int i = 0; i<nbIterations; i++)
	{
		populationEvolve(pop);
		
		if(verbose == 1)
			individualPrint( stderr, pop->tableInd[0] );	
	}
	
	return pop->tableInd[0];
}
