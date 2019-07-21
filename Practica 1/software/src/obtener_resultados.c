#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include "qap.h"
#include "random_ppio.h"

#define MI 0.3
#define THETA 0.3

using namespace std;

int main(int argc,char *argv[]){
  
  Set_random(atoi(argv[3]));
  
  QAP qap;
  int  i, j, csol;
  float t=0;

  qap=leerCaso(argv[1]);

  int *sol; // sol[i] unidades, i localizaciones
  sol = (int *)malloc(sizeof(int) * qap.n);
  float max_vecinos= 100*qap.n, max_exitos=10*qap.n;

  switch(atoi(argv[2])){
    case 0:
      greedy(qap,sol,csol,t);
      printf("%d\t%f\n",csol,t);
      break;
    case 1:
      BusquedaLocalPrimeroMejor(qap,sol,csol,t);
      printf("%d\t%f\n",csol,t);
      break;
    case 2:
      EnfiramientoSimulado(qap,sol,csol,MI,THETA,max_vecinos,max_exitos,t);
      printf("%d\t%f\n",csol,t);
      break;
    case 3:
      BusquedaTabu(qap,sol,csol,t);
      printf("%d\t%f\n",csol,t);
      break;
  }
  
  free(sol);

}
/*
  int sol2[4]={2,3,0,1};
  costo(qap,sol2,csol);
  printf("\nCosto: %d\n",csol);*/

/*int  i, j, csol;
  
  int *sol; // sol[i] unidades, i localizaciones
  sol = (int *)malloc(sizeof(int) * qap.n);
  
  greedy(qap,sol,csol);
  
  printf("Solucion: "); pintaVector(sol,qap.n);
  
  printf("Costo: %d\n",csol);
  
  int r,s,csolv,csolv2;
  int *vecino; // sol[i] unidades, i localizaciones
  vecino = (int *)malloc(sizeof(int) * qap.n);
  
//   GeneraAleatorio(vecino, qap.n);
  
   for(i=0;!GeneraVecinos(sol, qap.n, r,s,vecino);i++)  {
//     GeneraVecinos(sol, qap.n, r,s,vecino);
    printf("Vecino (%d %d) %d: ",r,s,i); pintaVector(vecino,qap.n);
    costo_factorizado(qap,sol,r,s,csolv); printf("Costo vecino factorizado: %d\n",csolv);
    costo(qap,vecino,csolv2); printf("Costo vecino: %d\n",csolv2);
   }
  
  free(sol);
  free(vecino);*/