#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "faddsub.h"
#include "fmul.h"
#include "finv.h"
#include "fdiv.h"

int main(){
    fu a, b, cans, vans;
    unsigned int diff;
    int count = 0;
    LoadTable();
    while(scanf("%u %u\n%u", &a.u, &b.u, &vans.u) != EOF){
        cans.f = DivFloat(a.f, b.f);
        if(cans.u != vans.u/* || (count >= 1000 && count < 10001)*/){
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