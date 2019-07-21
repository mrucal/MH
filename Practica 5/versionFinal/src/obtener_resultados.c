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

  string instancias[20]={"data/chr20b.dat", "data/chr20c.dat", "data/chr22a.dat", "data/chr22b.dat", "data/els19.dat", 
                      "data/esc32b.dat", "data/kra30b.dat", "data/lipa90b.dat", "data/nug30.dat", "data/sko56.dat", 
                      "data/sko64.dat", "data/sko72.dat", "data/sko81.dat", "data/sko90.dat", "data/sko100a.dat", 
                      "data/sko100b.dat", "data/sko100c.dat", "data/sko100d.dat", "data/sko100e.dat", "data/wil50.dat"};

  for(int i=0; i<20; i++){ 

    qap=leerCaso(instancias[i].c_str()); 

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
  }
  printf("\n");
  free(sol);

}