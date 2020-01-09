#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "ttvec.h"

char *XFileName;
char *YFileName;
TTVec x, y;

int processArguments(int argc, char **argv)
{
  int readArgCount = 0;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-X") == 0 || strcmp(argv[i], "-x") == 0) {
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
  return (readArgCount == 2);
}

void printUsage()
{
  printf("Compare two TT-vectors in provided in binary files.\n");
  printf("Usage: compare-ttvec -x [ttvec-x-file-name] -y [ttvec-y-file-name]\n");
  printf("  Example: ./compare-ttvec -x vecx.bin -y vecy.bin\n");
}


int main(int argc, char **argv)
{
  if (processArguments(argc, argv) == 0) {
    printUsage();
    exit(1);
  }
  loadTTVec(XFileName, &x);
  loadTTVec(YFileName, &y);
  if (compareTTVec(&x, &y)) {
    printf("EQUAL\n");
  } else {
    printf("NOT EQUAL\n");
  }
  destroyTTVec(&x);
  destroyTTVec(&y);
  return 0;
}
