#include <stdio.h>
#include <stdlib.h>
#include "util.h"

const unsigned int emask = ((1 << 8) - 1) << 23;
const unsigned int fmask =  (1 << 23) - 1;
const unsigned int efmask = emask + fmask;

unsigned int ftou(float a){
  fu t;
  t.f = a;
  return t.u;
}

float utof(unsigned int u){
  fu t;
  t.u = u;
  return t.f;
}

void PrintUIntBin(unsigned int u){
  char bin[35];
  bin[34] = '\0';
  for(int i=33;i >= 0;i--){
    if(i == 1 || i == 10){
      bin[i] = ' ';
    }
    else{
      bin[i] = (u % 2) + '0';
      u >>= 1;
    }
  }
  printf("%s\n", bin);
}

void PrintFloatBin(float a){
  PrintUIntBin(ftou(a));
}

unsigned int GetS(float a){
    return ftou(a) & (1 << 31);
}

unsigned int GetE(float a){
  return ftou(a) & emask;
}

unsigned int GetF(float a){
  return ftou(a) & fmask;
}
unsigned int GetEF(float a){
  return ftou(a) & efmask;
}

void SepSEF(sef *a){
  a->s = GetS(a->raw);
  a->e = GetE(a->raw);
  a->f = GetF(a->raw);
}

unsigned int CatSEF(sef *b){    //各部位をマスクしていることに注意
  unsigned int ret;
  b->s &= (1 << 31);
  b->e &= emask;
  b->f &= fmask;
  ret = b->s + b->e + b->f;
  b->raw = utof(ret);
  return ret;
}