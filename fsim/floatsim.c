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

unsigned int RN(unsigned int f28){ // X Y a_-1 a_-2 ... a_-23 a_-24 a_-25 a_-26 の下3bitを丸める
  unsigned int ulp = (f28 >> 3) & 1;
  unsigned int u2 = (f28 >> 2) & 1;
  unsigned int u4 = (f28 >> 1) & 1;
  unsigned int u8 = f28 & 1;
  unsigned int ret;
  ret = (f28 >> 4) + (ulp & u2);
  ret = (ret << 1) + ((ulp ^ u2) & (ulp | u4 | u8));
  return ret;
}

unsigned int AddFrac(unsigned int lf, unsigned int sf, unsigned int shift){
  unsigned int mask = (1 << (shift - 2)) - 1;
  unsigned int u8 = ((sf & mask) > 0);    // 1/8ulp以下をORしている
  unsigned int lf28 = ((1 << 23) + lf) << 3;
  unsigned int sf28 = ((((1 << 23) + sf) >> (shift - 2)) << 1) + u8;
  return RN(lf28 + sf28);
}

int main(){
  sef a, b, ans;
  sef *l, *s;
  while(scanf("%f %f", &a.raw, &b.raw) != 0){
    PrintFloatBin(a.raw);
    PrintFloatBin(b.raw);
    printf("%f\n", a.raw + b.raw);
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
    ans.f = AddFrac(l->f, s->f, ((l->e - s->e) >> 23));
    CatSEF(&ans);
    printf("%f\n", ans.raw);
    PrintFloatBin(ans.raw);
  }
  return 0;
}