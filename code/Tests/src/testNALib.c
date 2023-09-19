// MSVC meldet für veraltete, unsichere C-Funktionen wie fopen einen Fehler.
// Mittels folgender Zeile können diese Fehler ausgeschaltet werden:
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define iN 7
#define fN 3

int iArr[iN];
float fArr[fN];

int main(void) {
  FILE* fp = fopen("test.dat", "rb");

  int count = 0;
  if (fp == NULL) {
    printf("Kann Datei nicht oeffnen.\n");
  } else {
    count += (int)fread(iArr, sizeof(int), iN , fp);
    for (int i = 0; i < iN; ++i) {
      printf("%d ", iArr[i]);
    }
    printf("\n");

    count += (int)fread(fArr, sizeof(float), fN , fp);
    for (int i = 0; i < fN; ++i) {
      printf("%f ", fArr[i]);
    }
    printf("\n");

    printf("Insgesamt wurden %d Werte gelesen.\n", count);
    fclose(fp);
  }
  return 0;
}
