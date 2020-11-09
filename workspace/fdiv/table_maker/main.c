#include <stdio.h>
#include <stdlib.h>
#include "util_t.h"

int main(){
	//unsigned long long cst_table[1024], grd_table[1024];
	FILE *cfp, *gfp;
	sef a[1025], x;
	unsigned int A0_p1;
	unsigned long long xm;
	cfp = fopen("const_table.txt", "w");
	gfp = fopen("grad_table.txt", "w");
	for(int i=0;i<1024;++i){
		a[i].s = 0;
		a[i].e = 127 << 23;
		a[i].f = i << 13;
		CatSEF(&a[i]);
		//printf("M = %f\n", a.raw);
		//PrintFloatBin(a.raw);
	}
	a[1024].raw = 2.0;
	for(int i=0;i<1024;++i){
		x.raw = (a[i].raw + a[i+1].raw) / (a[i].raw * a[i+1].raw);
		SepSEF(&x);
		//printf("Xm = %f\n", x.raw);
		//PrintFloatBin(x.raw);
		A0_p1 = (1 << 10) + i;
		xm = (1 << 23) + x.f;
		unsigned long long cst_num = (xm << 35) - A0_p1 * xm * xm;
		fprintf(cfp, "%llu\n", cst_num);
		//printf("xm = %llu\n", xm);
		unsigned long long grd_num = (xm * xm) >> 13;
		fprintf(gfp, "%llu\n", grd_num);
	}
	fclose(cfp);
	fclose(gfp);
	return 0;
}
