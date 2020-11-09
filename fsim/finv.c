#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "fmul.h"
#include "finv.h"

//const unsigned int m_high10_mask = ((1 << 10) - 1) << 13;
const unsigned int m_low13_mask = (1 << 13) - 1;

unsigned int const_table[1024];
unsigned int grad_table[1024];

void LoadTable(){
  FILE *fp;
  fp = fopen("const_table.txt", "r");
  for(int i=0;i<1024;++i)
    fscanf(fp, "%u", &const_table[i]);
  fclose(fp);
  fp = fopen("grad_table.txt", "r");
  for(int i=0;i<1024;++i)
    fscanf(fp, "%u", &grad_table[i]);
  fclose(fp);
}

float InvFloat(float f){
  sef a, x, ans, M;
  a.raw = f;
  SepSEF(&a);
  if(a.f == 0){
    ans.s = a.s;
    ans.e = (254 << 23) - a.e;
    ans.f = 0;
    CatSEF(&ans);
    return ans.raw;
  }
  else{
    M.s = a.s;
    M.e = 127 << 23;
    M.f = a.f;
    CatSEF(&M);
    printf("\nM = %f\n", M.raw);
    PrintFloatBin(M.raw);
    x.raw = 2.0 / M.raw;  //x.rawに真の値を使っている。この場合誤差は出ないはず。(1ulpは許容)
    SepSEF(&x);
    printf("2 / M = %f\n", x.raw);
    PrintFloatBin(x.raw);
    unsigned int A0_p1 = (a.f >> 13) + (1 << 10);
    unsigned int A1 = a.f & m_low13_mask;
    unsigned long long xm = (1 << 23) + x.f;
    unsigned long long cst_num = (xm << 35) - A0_p1 * xm * xm;//(xm << 1) - ((A0_p1 * xm * xm) >> 34);
    //unsigned long long grd_num = (xm * xm) >> 47;  //ここで多分かなり情報落ちてたね
    unsigned long long mantissa = (cst_num - ((A1 * xm * xm) >> 13)) >> 34;//cst_num - A1 * grd_num;
    //printf("A0_p1 * xm * xm\n");
    //unsigned long long tmp = 2 * xm - (((A0_p1 << 13) + A1) >> 4) * (((xm >> 4) * (xm >> 4)) >> 35);
    //PrintUIntBin((A0_p1 << 13) + A1);
    //PrintUIntBin(xm);
    //PrintULLBin(A0_p1 * xm * xm);
    //PrintULLBin((unsigned long long)1 << 59);
    //PrintUIntBin((unsigned int)tmp);
    printf("cst_num(*2^34), A1 * grd_num(*2^34), uintmantissa\n");
    PrintULLBin(cst_num);
    PrintULLBin((A1 * xm * xm) >> 13);
    PrintUIntBin((unsigned int)mantissa);
    ans.s = a.s;
    ans.e = (253 << 23) - a.e;
    ans.f = (unsigned int)(mantissa & fmask);
    CatSEF(&ans);
    printf("\n");
    return ans.raw;
  }
}