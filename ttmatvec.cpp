#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "ttmat.h"
#include "ttvec.h"

char *AFileName;
char *XFileName;
char *YFileName;

int processArguments(int argc, char **argv)
{
  int readArgCount = 0;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-A") == 0 || strcmp(argv[i], "-a") == 0) {
      AFileName = argv[i + 1];
      i++;
      readArgCount++;
    } else if (strcmp(argv[i], "-X") == 0 || strcmp(argv[i], "-x") == 0) {
      XFileName = argv[i + 1];
      i++;
      readArgCount++;
    } else if (strcmp(argv[i], "-Y") == 0 || strcmp(argv[i], "-y") == 0) {
      YFileName = argv[i + 1];
      i++;
      readArgCount++;
    } else {
      printf("WARNING: Ignoring argument: %s\n", argv[i]);
      i++;
    }
  }
  return (readArgCount == 3);
}

TTMat A;
TTVec x, y;

void loadTTMat(char *fileName, TTMat *mat);

void destroyTTMat(TTMat *mat);

void loadTTVec(char *fileName, TTVec *vec);

void saveTTVec(char *fileName, TTVec *vec);

void destroyTTVec(TTVec *vec);

void multiplyTTMatVec(TTMat *A, TTVec *x, TTVec *y);

void printUsage()
{
  printf("Matrix-vector multiplication in tensor-train format\n");
  printf("Usage: ttmatvec -a [file-name-matrix-A] -x [file-name-vector-x] -y [file-name-vector-y]\n");
  printf("  Example: ./ttmatvec -a mata.bin -x vecx.bin -y vecy.bin\n");
}

int main(int argc, char **argv)
{
  if (processArguments(argc, argv) == 0) {
    printUsage();
    exit(1);
  }
  loadTTMat(AFileName, &A);
//  printTTMat(&A, stdout);
  loadTTVec(XFileName, &x);
//  printTTVec(&x, stdout);
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  multiplyTTMatVec(&A, &x, &y);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  printf("TTMatVec took %e seconds.\n", time_span.count());
//  printTTVec(&y, stdout);
  saveTTVec(YFileName, &y);
  destroyTTVec(&x);
  destroyTTVec(&y);
  destroyTTMat(&A);
  return 0;
}
