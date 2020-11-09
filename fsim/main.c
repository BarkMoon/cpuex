#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include "faddsub.h"
#include "fmul.h"
#include "finv.h"

int main(){
  char command[8], opc[8];
  int n, miss;
  float a, b, ans, trueans;
  unsigned int ua, ub, uans, utrueans, diff;
  OPERATOR oper;
  srand((unsigned)time(NULL));
  LoadTable();
  while(1){
    printf("command: ");
    scanf("%s", command);
    if(strcmp(command, "add") == 0){
      printf("adding numbers: ");
      scanf("%f %f", &a, &b);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a + b);
      PrintFloatBin(a + b);
      ans = normalize(AddFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "addu") == 0){
      printf("adding numbers(uint): ");
      scanf("%u %u", &ua, &ub);
      a = utof(ua);
      b = utof(ub);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a + b);
      PrintFloatBin(a + b);
      ans = normalize(AddFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "sub") == 0){
      printf("subtracting numbers: ");
      scanf("%f %f", &a, &b);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a - b);
      PrintFloatBin(a - b);
      ans = normalize(SubFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "subu") == 0){
      printf("subtracting numbers(uint): ");
      scanf("%u %u", &ua, &ub);
      a = utof(ua);
      b = utof(ub);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a - b);
      PrintFloatBin(a - b);
      ans = normalize(SubFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "mul") == 0){
      printf("multipling numbers: ");
      scanf("%f %f", &a, &b);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a * b);
      PrintFloatBin(a * b);
      ans = normalize(MulFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "mulu") == 0){
      printf("multipling numbers(uint): ");
      scanf("%u %u", &ua, &ub);
      a = utof(ua);
      b = utof(ub);
      PrintFloatBin(a);
      PrintFloatBin(b);
      printf("true answer: %f\n", a * b);
      PrintFloatBin(a * b);
      ans = normalize(MulFloat(a, b));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "inv") == 0){
      printf("Inverting number: ");
      scanf("%f", &a);
      PrintFloatBin(a);
      printf("true answer: %f\n", 1 / a);
      PrintFloatBin(1 / a);
      ans = normalize(InvFloat(a));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "invu") == 0){
      printf("inverting number(uint): ");
      scanf("%u", &ua);
      a = utof(ua);
      PrintFloatBin(a);
      printf("true answer: %f\n", 1 / a);
      PrintFloatBin(1 / a);
      ans = normalize(InvFloat(a));
      printf("my answer: %f\n", ans);
      PrintFloatBin(ans);
    }
    else if(strcmp(command, "rand") == 0){
      printf("Verifying opecode: ");
      scanf("%s", opc);
      if(strcmp(opc, "add") == 0)
        oper = ADD;
      else if(strcmp(opc, "sub") == 0)
        oper = SUB;
      else if(strcmp(opc, "mul") == 0)
        oper = MUL;
      else if(strcmp(opc, "inv") == 0)
        oper = INV;
      else
        continue;
      printf("sample number: ");
      scanf("%d", &n);
      miss = 0;
      for(int i=0;i<n;++i){
        a = normalize(utof((unsigned)rand()));    // int範囲で生成してunsignedキャストしているので正？
        b = normalize(utof((unsigned)rand()));    //
        switch(oper){
          case ADD:
            ans = AddFloat(a, b);
            trueans = a + b;
            break;
          case SUB:
            ans = SubFloat(a, b);
            trueans = a - b;
            break;
					case MUL:
            ans = MulFloat(a, b);
            trueans = a * b;
            break;
          case INV:
            ans = InvFloat(a);
            trueans = 1 / a;
            break;
        }
        //printf("%f %f %f %f ", a, b, a+b, ans);
        /*if(ans == a + b){
          printf("OK\n");
        }*/
        uans = ftou(ans);
        utrueans = ftou(trueans);
        diff = (uans >= utrueans) ? uans - utrueans : utrueans - uans;
        if(ans != trueans && oper != INV){
          miss++;
          printf("%f %f %f %f NG\n", a, b, trueans, ans);
          printf("uint: %u %u\n", ftou(a), ftou(b));
          PrintFloatBin(a);
          PrintFloatBin(b);
          PrintFloatBin(trueans);
          PrintFloatBin(ans);
        }
        else if(ans != trueans && oper == INV){
          printf("diff = %u\n", diff);
          if(diff >= 5 && GetE(a) != 0 && GetE(a) != emask && GetE(trueans) != 0 && GetE(trueans) != emask){
            miss++;
            printf("%f %f %f NG\n", a, trueans, ans);
            printf("uint: %u\n", ftou(a));
            PrintFloatBin(a);
            PrintFloatBin(trueans);
            PrintFloatBin(ans);
          }
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