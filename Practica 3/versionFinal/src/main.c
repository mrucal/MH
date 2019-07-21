#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "qap.h"
#include "random_ppio.h"

#define MI 0.3
#define THETA 0.3

using namespace std;

int main(int argc,char *argv[]){
  
  Set_random(5555);
  
  QAP qap;
  
  qap=leerCaso(argv[1]);
  
  int  i, j, csol;
  float t;
  
  int *sol; // sol[i] unidades, i localizaciones
  sol = (int *)malloc(sizeof(int) * qap.n);

  AGGposicion(qap,sol,csol,0.7,0.01,t);
  
  printf("\nSolucion AGGp: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d Tiempo: %f\n\n",csol,t);

  AGEposicion(qap,sol,csol,1,0.01,t);
  
  printf("\nSolucion AGEp: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d Tiempo: %f\n\n",csol,t);

  AGGox(qap,sol,csol,1,0.01,t);
  
  printf("\nSolucion AGGox: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d Tiempo: %f\n\n",csol,t);

  AGEox(qap,sol,csol,1,0.01,t);
  
  printf("\nSolucion AGEox: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d Tiempo: %f\n\n",csol,t);

  free(sol);

}