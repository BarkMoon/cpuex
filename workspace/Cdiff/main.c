#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "fmul.h"

int main(){
    fu a, b, cans, vans;
    unsigned int diff;
    while(scanf("%u %u\n%u", &a.u, &b.u, &vans.u) != EOF){
        cans.f = MulFloat(a.f, b.f);
        if(cans.u != vans.u){
            diff = (cans.u >= vans.u) ? cans.u - vans.u : vans.u - cans.u;
            printf("diff = %d\n", diff);
            PrintUIntBin(a.u);
            PrintUIntBin(b.u);
            PrintUIntBin(vans.u);
            PrintUIntBin(cans.u);
        }
    }
    return 0;
}