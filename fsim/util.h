#pragma once

typedef union{
  float f;
  unsigned int u;
}fu;

typedef struct{
  float raw;
  unsigned int s, e, f;
}sef;

extern const unsigned int emask;
extern const unsigned int fmask;

unsigned int ftou(float a);
float utof(unsigned int u);
void PrintUIntBin(unsigned int u);
void PrintFloatBin(float a);
unsigned int GetS(float a);
unsigned int GetE(float a);
unsigned int GetF(float a);
void SepSEF(sef *a);
unsigned int CatSEF(sef *b);