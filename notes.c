Individual *individualCreate(int length, int nbVal) : crée un individu dont le gé-
notype aura une longueur length et sera constitué d’entiers de valeurs comprises entre 0
et nbVal-1.

void individualFree(Individual *ind) : libère la mémoire prise par l’individu.

int individualGetLength(Individual *ind) : renvoie la longueur du génotype de l’in-
dividu.

int individualGetGene(Individual *ind, int i) : renvoie la valeur du gène à la posi-
tion i de l’individu.

void individualSetGene(Individual *ind, int i, int val) : met la valeur du gène à
la position i à val.

void individualRandomInit(Individual *ind) : Initialise le génotype de l’individu à
une valeur aléatoire (voir la section 1).

void individualRandomPermInit(Individual *ind) : Initialise le génotype de l’individu
à une permutation aléatoire des valeurs entre 0 et len-1 (voir la section 2).
Individual *individualCopy(Individual *ind) : crée et renvoie un nouvel individu qui
est une copie parfaite de l’individu ind.

void individualPrint(FILE *fp, Individual *ind) : affiche le génotype de l’individu
dans le fichier fp (fonction fprintf). Le format attendu est une liste des gènes (entiers)
séparées par des virgules, sans retour à la ligne.

void individualSeqMutation(Individual *ind, float pm) : implémente l’opérateur de
mutation décrit dans la section 1 pour une séquence non contrainte.

Individual *individualSeqCrossOver(Individual *parent1, Individual *parent2):
implémente l’opérateur de recombinaison décrit dans la section 1 pour une séquence non
contrainte.

void individualPermMutation(Individual *ind, float pm) : implémente l’opérateur
de mutation décrit dans la section 2 pour une permutation d’entiers.
Individual *individualPermCrossOver(Individual *parent1, Individual *parent2):
implémente l’opérateur de recombinaison décrit dans la section 2 pour une permutation
d’entiers.