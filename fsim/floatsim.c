#include <stdio.h>
#include <stdlib.h>

typedef union{
  float f;
  unsigned int d;
}fd;

typedef struct{
  float raw;
  unsigned int s, e, f;
}sef;

unsigned int ftou(float a){
  fd t;
  t.f = a;
  return t.d;
}

float utof(unsigned int u){
  fd t;
  t.d = u;
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

unsigned int GetE(float a){
  const unsigned int emask = (1 << 31) - (1 << 23);
  unsigned int u = ftou(a);
  u &= emask;
  return u;
}

unsigned int GetF(float a){
  const unsigned int fmask =  (1 << 23) - 1;
  unsigned int u = ftou(a);
  u &= fmask;
  return u;
}

void SepSEF(sef *a){
  a->s = ftou(a->raw) & (1 << 31);
  a->e = GetE(a->raw);
  a->f = GetF(a->raw);
}

void CatSEF(sef *b){    //各部位をマスクしていることに注意
  b->s &= (1 << 31);
  b->e &= (1 << 31) - (1 << 23);
  b->f &= (1 << 23) - 1;
  b->raw = utof(b->s + b->e + b->f);
}

int main(){
  sef a, b, ans;
  sef *l, *s;
  while(scanf("%f %f", &a.raw, &b.raw) != 0){
    PrintFloatBin(a.raw);
    PrintFloatBin(b.raw);
    PrintFloatBin(a.raw + b.raw);
    SepSEF(&a);
    SepSEF(&b);
    if(a.e >= b.e){
      l = &a;
      s = &b;
    }
    else{
      l = &b;
      s = &a;
    }
    ans.s = l->s;
    ans.e = l->e;
    ans.f = l->f + ((s->f) >> ((l->e - s->e) >> 23));
    CatSEF(&ans);
    printf("%f\n", ans.raw);
    PrintFloatBin(ans.raw);
  }
  return 0;
}