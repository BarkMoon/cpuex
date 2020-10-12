#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef union{
  float f;
  unsigned int u;
}fu;

typedef struct{
  float raw;
  unsigned int s, e, f;
}sef;

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

unsigned int GetE(float a){
  const unsigned int emask = ((1 << 8) - 1) << 23;
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

unsigned int CatSEF(sef *b){    //各部位をマスクしていることに注意
  unsigned int ret;
  b->s &= (1 << 31);
  b->e &= ((1 << 8) - 1) << 23;
  b->f &= (1 << 23) - 1;
  ret = b->s + b->e + b->f;
  b->raw = utof(ret);
  return ret;
}

unsigned int RN(unsigned int f28, unsigned int *e){ // X Y a_-1 a_-2 ... a_-23 a_-24 a_-25 a_-26 の下3bitを丸める
  unsigned int ulp, u2, u4, u8, ret;
  //printf("e %u\n", *e);
  //printf("丸め前\n");
  //PrintUIntBin(f28);
  if(f28 & (1 << 27)){
    u4 = (f28 >> 1) & 1;
    u8 = f28 & 1;
    (*e)+= 1 << 23;
    f28 = ((f28 >> 2) << 1) + (u4 | u8);
  }
  //printf("fracの足し算による繰り上がり後\n");
  //PrintUIntBin(f28);
  ulp = (f28 >> 3) & 1;
  u2 = (f28 >> 2) & 1;
  u4 = (f28 >> 1) & 1;
  u8 = f28 & 1;
  ret = (f28 >> 4) + (ulp & u2);
  ret = (ret << 1) + ((ulp ^ u2) & (ulp | u4 | u8));
  //printf("丸め後\n");
  //PrintUIntBin(ret);
  if(ret & (1 << 24)){
    (*e) += 1 << 23;
    ret = (ret >> 1);
  }
  //printf("丸めによる繰り上がり後\n");
  //PrintUIntBin(ret);
  //printf("e %u\n", *e);
  return ret;
}

unsigned int AddFrac(unsigned int lf, unsigned int sf, unsigned int shift, unsigned int *e){
  unsigned int mask, u8, lf28, sf28;
  if(shift >= 3){
    mask = (1 << (shift - 2)) - 1;  //shift数が大きすぎるとアヤシイ
    u8 = ((sf & mask) > 0);    // 1/8ulp以下をORしている
    lf28 = ((1 << 23) + lf) << 3;
    sf28 = ((((1 << 23) + sf) >> (shift - 2)) << 1) + u8;
  }
  else{
    lf28 = ((1 << 23) + lf) << 3;
    sf28 = ((1 << 23) + sf) << (3 - shift);
  }
  return RN(lf28 + sf28, e);
}

float AddFloat(float f1, float f2){
  sef a, b, ans;
  sef *l, *s;
  a.raw = f1;
  b.raw = f2;
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
  ans.f = AddFrac(l->f, s->f, ((l->e - s->e) >> 23), &ans.e);
  CatSEF(&ans);
  return ans.raw;
}

int main(){
  char command[8], opc[8];
  int n;
  float a, b, ans;
  srand((unsigned)time(NULL));
  while(scanf("%s", command) != 0){
    if(strcmp(command, "add") == 0){
      printf("adding numbers: ");
      scanf("%f %f", &a, &b);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true sum: %f\n", a + b);
      PrintFloatBin(a + b);
      ans = AddFloat(a, b);
      printf("my sum: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "random") == 0){
      //scanf("%s", opc);
      //if(strcmp(command, "add") == 0)
      printf("sample number: ");
      scanf("%d", &n);
      for(int i=0;i<n;++i){
        a = utof((unsigned)rand());
        b = utof((unsigned)rand());
        ans = AddFloat(a, b);
        printf("%f %f %f %f ", a, b, a+b, ans);
        if(ans == a + b){
          printf("OK\n");
        }
        else{
          printf("NG\n");
          PrintFloatBin(a);
          PrintFloatBin(b);
          PrintFloatBin(a + b);
          PrintFloatBin(ans);
        }
      }
    }
    else if(strcmp(command, "quit") == 0)
      break;
  }
  return 0;
}