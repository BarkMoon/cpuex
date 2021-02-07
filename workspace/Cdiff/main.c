#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "util.h"
#include "faddsub.h"
#include "fmul.h"
#include "finv.h"
#include "fdiv.h"
#include "sqrt.h"

int main(){
    fu a, b, cans, vans;
    unsigned int diff;
    int count = 0;
    LoadTable();
    SqrtLoadTable();
    //while(scanf("%u %u\n%u", &a.u, &b.u, &vans.u) != EOF){
    //    cans.f = DivFloat(a.f, b.f);
    while(scanf("%u\n%u", &a.u, &vans.u) != EOF){
        cans.f = SqrtFloat(a.f);
        b.f = sqrtf(a.f);
        //printf("%f\n%f\n", a.f, cans.f);
        if(cans.u != vans.u/* || (count >= 0 && count < 1000)*/){
            diff = (cans.u > vans.u) ? cans.u - vans.u : vans.u - cans.u;
            printf("diff = %d\n", diff);
            PrintUIntBin(a.u);
            PrintUIntBin(b.u);
            PrintUIntBin(vans.u);
            PrintUIntBin(cans.u);
        }
        ++count;
    }
    return 0;
}