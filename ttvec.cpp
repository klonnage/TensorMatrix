#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cmath>
#include "ttvec.h"


void loadTTVec(
    char *fileName,
    TTVec *vec)
{
  FILE *file = fopen(fileName, "rb");
  if (!file) { printf("ERROR: Unable to open the file %s.\n", fileName); exit(1); }
  fread(&(vec->d), sizeof(vec->d), 1, file);
  vec->m = (int *)malloc(vec->d * sizeof(vec->m[0]));
  fread(vec->m, sizeof(vec->m[0]), vec->d, file);
  vec->r = (int *)malloc((vec->d + 1) * sizeof(vec->r[0]));
  vec->r[0] = vec->r[vec->d] = 1; // The first and the last ranks are always one.
  fread(vec->r + 1, sizeof(vec->r[0]), vec->d - 1, file);
  vec->dimVecBegin = (size_t *)malloc((vec->d + 1) * sizeof(vec->dimVecBegin[0]));
  vec->dimVecBegin[0] = 0;
  for (int d = 0; d < vec->d; d++) {
    vec->dimVecBegin[d + 1] = vec->dimVecBegin[d] + vec->m[d] * vec->r[d] * vec->r[d + 1];
  }
  vec->data = (double *)malloc(vec->dimVecBegin[vec->d] * sizeof(vec->data[0]));
  fread(vec->data, sizeof(vec->data[0]), vec->dimVecBegin[vec->d], file);
  fclose(file);
}

void saveTTVec(
    char *fileName,
    TTVec *vec)
{
  FILE *file = fopen(fileName, "wb");
  if (!file) { printf("ERROR: Unable to open the file %s.\n", fileName); exit(1); }
  fwrite(&(vec->d), sizeof(vec->d), 1, file);
  fwrite(vec->m, sizeof(vec->m[0]), vec->d, file);
  fwrite(vec->r + 1, sizeof(vec->r[0]), vec->d - 1, file);
  fwrite(vec->data, sizeof(vec->data[0]), vec->dimVecBegin[vec->d], file);
  fclose(file);
}

void destroyTTVec(TTVec *vec)
{
  free(vec->m);
  free(vec->r);
  free(vec->dimVecBegin);
  free(vec->data);
}

void printTTVec(
    TTVec *vec,
    FILE *outFile)
{
  fprintf(outFile, "TTVec:\n");
  fprintf(outFile, "D = %d\n", vec->d);
  for (int d = 0; d < vec->d; d++) {
    fprintf(outFile, "M[%d]=%d\t\t", d, vec->m[d]);
    fprintf(outFile, "R[%d]xR[%d]=%dx%d\n", d, d + 1, vec->r[d], vec->r[d + 1]);
  }
  for (int d = 0; d < vec->d; d++) {
    fprintf(outFile, "Printing matrices for dimension %d:\n", d);
      for (int i = 0; i < vec->m[d]; i++) {
        fprintf(outFile, "  Printing the matrix block (%d):\n", i);
        double *blockBegin = vec->data + vec->dimVecBegin[d] + i * (vec->r[d] * vec->r[d + 1]);
        for (int ri = 0; ri < vec->r[d]; ri++) {
          for (int rj = 0; rj < vec->r[d + 1]; rj++) {
            fprintf(outFile, "    %lf ", blockBegin[ri + rj * vec->r[d]]);
          }
          fprintf(outFile, "\n");
        }
      }
  }
}

double *getTTVecBlock(
    TTVec *vec,
    int dim,
    int rowIdx)
{
  return vec->data + vec->dimVecBegin[dim] + ((size_t)rowIdx * vec->r[dim] * vec->r[dim + 1]);
}

int compareTTVec(
    TTVec *x,
    TTVec *y)
{
  if (x->d != y->d) { return 0; }
  for (int d = 0; d < x->d; d++) {
    if (x->m[d] != y->m[d]) { return 0; }
    if (x->r[d] != y->r[d]) { return 0; }
    if (x->dimVecBegin[d] != y->dimVecBegin[d]) { return 0; }
  }
  if (x->dimVecBegin[x->d] != y->dimVecBegin[y->d]) { return 0; }
  double sqError = 0.0;
  double normX = 0.0;
  double normY = 0.0;
  for (size_t i = 0, ie = x->dimVecBegin[x->d]; i < ie; i++) { 
    double diff = x->data[i] - y->data[i];
    sqError += diff * diff;
    normX += x->data[i] * x->data[i];
    normY += y->data[i] * y->data[i];
  }
  double maxNorm = fabs(normX) > fabs(normY) ? fabs(normX) : fabs(normY);
//  printf("error %e\n", sqError);
  if (maxNorm < 1e-6) { return (sqError < 1e-12); }
  else { return (sqError / maxNorm < 1e-6); }
}
