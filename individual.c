#include <stdlib.h>
#include <stdio.h>
#include "individual.h"

//a chaque fois je modifie/change/cree le code je le mis dans le github ;)

Individual *individualCreate(int length, int nbVal) {

	Individual * newIndividual = malloc(length*sizeof(int));

	for (int i = 0; i < length; ++i)
	    newIndividual->tab[i] = rand()%nbVal;

}


void individualFree(Individual *ind) {
	free(ind);
	
}


int individualGetLength(Individual *ind) {

	
}


int individualGetGene(Individual *ind, int i) {

	
}


void individualSetGene(Individual *ind, int i, int val) {

	
}


void individualRandomInit(Individual *ind) {

	
}


void individualRandomPermInit(Individual *ind) {


}


Individual *individualCopy(Individual *ind) {
  

}

