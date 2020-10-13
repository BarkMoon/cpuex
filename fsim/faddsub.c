#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "faddsub.h"

unsigned int RN(unsigned int f28, unsigned int *e){ // X Y a_-1 a_-2 ... a_-23 a_-24 a_-25 a_-26 の下3bitを丸める
  unsigned int ulp, u2, u4, u8, ret;
  /*printf("e %u\n", *e);
  printf("丸め前\n");
  PrintUIntBin(f28);*/
  if(f28 & (1 << 27)){
    u4 = (f28 >> 1) & 1;
    u8 = f28 & 1;
    (*e)+= 1 << 23;
    f28 = ((f28 >> 2) << 1) + (u4 | u8);
  }
  /*printf("fracの足し算による繰り上がり後\n");
  PrintUIntBin(f28);*/
  ulp = (f28 >> 3) & 1;
  u2 = (f28 >> 2) & 1;
  u4 = (f28 >> 1) & 1;
  u8 = f28 & 1;
  ret = (f28 >> 4) + (ulp & u2);
  ret = (ret << 1) + ((ulp ^ u2) & (ulp | u4 | u8));
  /*printf("丸め後\n");
  PrintUIntBin(ret);*/
  if(ret & (1 << 24)){
    (*e) += 1 << 23;
    ret = (ret >> 1);
  }
  /*printf("丸めによる繰り上がり後\n");
  PrintUIntBin(ret);
  printf("e %u\n", *e);*/
  return ret;
}

void AddSEF(sef *l, sef *s, sef *ans){
  unsigned int shift = (l->e - s->e) >> 23;
  unsigned int mask, u8, lf28, sf28;
  unsigned int sub28;
  ans->s = l->s;
  ans->e = l->e;
  if(shift >= 25 || s->e == 0){    // けち表現により、0は2^-127と同一になってしまうため例外
    ans->f = (1 << 23) + l->f;   // 最下位1bitに
    return;
  }
  else if(l->e == emask){
    ans->f = 0;
    return;
  }
  else if(shift >= 3){
    mask = (1 << (shift - 2)) - 1;
    u8 = ((s->f & mask) > 0);    // 1/8ulp以下をORしている
    lf28 = ((1 << 23) + l->f) << 3;
    sf28 = ((((1 << 23) + s->f) >> (shift - 2)) << 1) + u8;
  }
  else{
    lf28 = ((1 << 23) + l->f) << 3;
    sf28 = ((1 << 23) + s->f) << (3 - shift);
  }
  /*printf("lf28, sf28\n");
  PrintUIntBin(lf28);
  PrintUIntBin(sf28);*/
  
  // 答えが負になるパターンがまだ未実装
  
  if(l->s ^ s->s){   // signが異なる場合
    sub28 = lf28 - sf28;
    if(sub28 == 0){
      ans->e = 0;
      ans->f = 0;
    }
    else{
      /*printf("lf28とsf28\n");
      PrintUIntBin(lf28);
      PrintUIntBin(sf28);
      printf("sub28とe\n");
      PrintUIntBin(sub28);
      PrintUIntBin(ans->e);
      PrintUIntBin(1 << 26);*/
      while((sub28 & (1 << 26)) == 0){   // けち表現bitの27bit目が1になるまでシフト
        ans->e -= (1 << 23);
        sub28 <<= 1;
        /*printf("sub28とe\n");
        PrintUIntBin(sub28);
        PrintUIntBin(ans->e);*/
      }
      ans->f = RN(sub28, &ans->e);
    }
  }
  else{
    ans->f = RN(lf28 + sf28, &ans->e);
  }
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
  AddSEF(l, s, &ans);
  CatSEF(&ans);
  return ans.raw;
}

float SubFloat(float f1, float f2){
  unsigned int u2 = ftou(f2);
  u2 += (1 << 31);    // u2のsignビットを反転
  return AddFloat(f1, utof(u2));
}

float normalize(float denf){
  unsigned int e = GetE(denf), f = GetF(denf);
  if(e == 0 && f != 0)
    return 0.0;           // AddFracの場合分けのおかげで不要？
  else if(e == emask)
    return utof(emask);   // inf, nanはinfにしている
  else
    return denf; 
}