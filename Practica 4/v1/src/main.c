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
  
  GeneraSolucionAleatoria(sol,qap.n);
  BusquedaLocalPrimeroMejor(qap,sol,csol,t);
  
  printf("Solucion BLPM: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d Tiempo: %f\n",csol,t);
  
  free(sol);

}