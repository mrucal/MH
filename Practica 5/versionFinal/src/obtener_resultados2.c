#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "qap.h"
#include "random_ppio.h"

#include <string>

#define MI 0.3
#define THETA 0.3

using namespace std;

int main(int argc,char *argv[]){

  Set_random(atoi(argv[2]));
  
  QAP qap;
  int  i, j, csol;
  float t=0;


  

  int *sol; // sol[i] unidades, i localizaciones
  
  float max_vecinos, max_exitos;

    qap=leerCaso(argv[3]); 
    // free(sol);
    max_vecinos= 100*qap.n, max_exitos=10*qap.n;
    sol = (int *)malloc(sizeof(int) * qap.n);
    t=0;        
    switch(atoi(argv[1])){
      case 0:
        greedy(qap,sol,csol,t);

        printf("%d\t%f\t\t",csol,t);
        break;
      case 1:
        MemeticoTodos(qap,sol,csol,0.7,0.001,t);

        printf("%d\t%f\t\t",csol,t);
        break;
      case 2:
        MemeticoSubconjunto(qap,sol,csol,0.7,0.001,t);

        printf("%d\t%f\t\t",csol,t);
        break;
      case 3:
        MemeticoMejores(qap,sol,csol,0.7,0.001,t);

        printf("%d\t%f\t\t",csol,t);
        break;
    }
  
  free(sol);

}