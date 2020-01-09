#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

char *OutputFileName;
int D;
int *M;
int *R;

void parseCsvInt(
    char *csv,
    int *arr,
    int size)
{
  char seps[] = ",";
  char *token = strtok(csv, seps);
  int i = 0;
  while (token != NULL)
  {
    if (i > size) { printf("ERROR: The size of the CSV list exceeds the size of the array: %d.\n", size); }
    sscanf (token, "%d", arr + i);
    token = strtok (NULL, seps);
    i++;
  }
}

int processArguments(int argc, char **argv)
{
  int readArgCount = 0;
  D = -1;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "-f") == 0) {
      OutputFileName = argv[i + 1];
      i++;
      readArgCount++;
    } else if (strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "-d") == 0) {
      D = atoi(argv[i + 1]);
      i++;
      readArgCount++;
    } else if (strcmp(argv[i], "-M") == 0 || strcmp(argv[i], "-m") == 0) {
      if (D == -1) {
        printf("ERROR: The parameter -d must be provided before -m.\n");
        return 0;
      }
      M = (int *)malloc(D * sizeof(int));
      parseCsvInt(argv[i + 1], M, D);
      i++;
      readArgCount++;
    } else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "-r") == 0) {
      if (D == -1) {
        printf("ERROR: The parameter -d must be provided before -m.\n");
        return 0;
      }
      R = (int *)malloc((D - 1) * sizeof(int));
      parseCsvInt(argv[i + 1], R, D - 1);
      i++;
      readArgCount++;
    } else {
      printf("WARNING: Ignoring argument: %s\n", argv[i]);
      i++;
    }
  }
  return (readArgCount == 4);
}

void printUsage()
{
  printf("Usage: create-ttvec -f [output-file] -d [num-dimensions] -m [dimension-num-rows] "
      "-r [dimension-rank (excluding the first and the last which are always one)]\n");
  printf("  Example: create-ttvec -f ttvec.bin -d 3 -m 10,12,14 -r 5,8\n");
}

void createTTVector()
{
  FILE *file = fopen(OutputFileName, "wb");
  fwrite(&D, sizeof(D), 1, file);
  fwrite(M, sizeof(M[0]), D, file);
  fwrite(R, sizeof(R[0]), D - 1, file);
  srand(time(0));
  for (int d = 0; d < D; d++) {
    int rank0 = (d == 0) ? 1 : R[d - 1];
    int rank1 = (d == (D - 1)) ? 1 : R[d];
    int arrSize = M[d] * rank0 * rank1;
    double *arr = (double *)malloc(arrSize * sizeof(double));
    for (int i = 0; i < arrSize; i++) {
      arr[i] = rand() / (double)RAND_MAX;
    }
    fwrite(arr, sizeof(arr[0]), arrSize, file);
    free(arr);
  }
  fclose(file);
}

int main(int argc, char **argv)
{
  if (!processArguments(argc, argv)) {
    printf("ERROR: Invalid argument set.\n");
    printUsage();
    exit(1);
  }
  createTTVector();
  free(M);
  free(R);
  return 0;
}
