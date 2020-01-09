#ifndef TTVEC_H
#define TTVEC_H
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  int d;
  int *m, *r;
  double *data;
  size_t *dimVecBegin;
} TTVec;

void loadTTVec(
    char *fileName,
    TTVec *vec);

void saveTTVec(
    char *fileName,
    TTVec *vec);

void destroyTTVec(TTVec *vec);

void printTTVec(
    TTVec *vec,
    FILE *outFile);

double *getTTVecBlock(
    TTVec *vec,
    int dim,
    int rowIdx);

int compareTTVec(
    TTVec *x,
    TTVec *y);

#endif
