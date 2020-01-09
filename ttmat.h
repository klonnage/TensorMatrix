#ifndef TTMAT_H
#define TTMAT_H

#include "ttvec.h"

typedef struct
{
  int d;
  int *m, *n, *r;
  size_t *dimMatBegin;
  double *data;
} TTMat;

void loadTTMat(
    char *fileName,
    TTMat *mat);

void destroyTTMat(TTMat *mat);

void printTTMat(
    TTMat *mat,
    FILE *outFile);

double *getTTMatBlock(
    TTMat *mat,
    int dim,
    int rowIdx,
    int colIdx);

void multiplyTTMatVec(
    TTMat *A,
    TTVec *x,
    TTVec *y);

#endif
