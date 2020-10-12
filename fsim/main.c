#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include "faddsub.h"

int main(){
  char command[8], opc[8];
  int n, miss;
  float a, b, ans;
  unsigned int ua, ub;
  srand((unsigned)time(NULL));
  while(1){
    printf("command: ");
    scanf("%s", command);
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
    else if(strcmp(command, "addu") == 0){
      printf("adding numbers(uint): ");
      scanf("%u %u", &ua, &ub);
      a = utof(ua);
      b = utof(ub);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true sum: %f\n", a + b);
      PrintFloatBin(a + b);
      ans = normalize(AddFloat(a, b));
      printf("my sum: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "rand") == 0){
      //scanf("%s", opc);
      //if(strcmp(command, "add") == 0)
      printf("sample number: ");
      scanf("%d", &n);
      miss = 0;
      for(int i=0;i<n;++i){
        a = normalize(utof((unsigned)rand()));
        b = normalize(utof((unsigned)rand()));
        ans = normalize(AddFloat(a, b));
        //printf("%f %f %f %f ", a, b, a+b, ans);
        /*if(ans == a + b){
          printf("OK\n");
        }*/
        if(ans != a + b){
          miss++;
          printf("%f %f %f %f NG\n", a, b, a+b, ans);
          printf("uint: %u %u\n", ftou(a), ftou(b));
          PrintFloatBin(a);
          PrintFloatBin(b);
          PrintFloatBin(a + b);
          PrintFloatBin(ans);
        }
      }
      if(!miss)
        printf("all OK!\n");
    }
    else if(strcmp(command, "quit") == 0)
      break;
  }
  return 0;
}