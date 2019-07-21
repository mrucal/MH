#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "quicksort.h"
#include "random_ppio.h"
#include "timer.h"
#include "qap.h"

#include <utility>
#include <vector>
#include <set>
#include <algorithm>  
#include <math.h>

QAP leerCaso(const char *nombre_fichero){
  
  FILE *fichero;
  char buf[N_BUF];
  
  int n=0, i, j;
  
  QAP qap;
  
  if ( (fichero = fopen(nombre_fichero, "r")) == NULL ){
    fprintf(stderr,"Error al abrir el fichero.\n");
    exit(1); 
  }
  
  assert(fichero != NULL);
  // printf("\nleyendo fichero %s ... \n", nombre_fichero);
  
  fscanf(fichero,"%d", &n);
  
  qap.n=n;	
  
  if(n!=0){
    
    // Reserva de memoria

    if((qap.flujo = (int **)malloc(n*sizeof(int *))) == NULL )
	exit(EXIT_FAILURE);

    if((qap.distancia = (int **)malloc(n*sizeof(int *))) == NULL )
	exit(EXIT_FAILURE);
    
    for ( i = 0 ; i < qap.n ; i++ ){
      if((qap.flujo[i]=(int *)malloc(n*sizeof(int))) == NULL )
	exit(EXIT_FAILURE);
      if((qap.distancia[i]=(int *)malloc(n*sizeof(int))) == NULL )
	exit(EXIT_FAILURE);
    }
    	
    // Leer flujos
    for ( i = 0 ; i < qap.n ; i++ ) {
      for ( j = 0 ; j < qap.n ; j++ )
	fscanf(fichero,"%d", &qap.flujo[i][j]);
    }
    
    // Leer distancias
    for ( i = 0 ; i < qap.n ; i++ ) {
      for ( j = 0 ; j < qap.n ; j++ )
	fscanf(fichero,"%d", &qap.distancia[i][j]);
    }
    
  }
  
  return qap;
  
}

void pintaVector(int *sol,int n){
   
  int i;
   
  for ( i = 0 ; i < n ; i++ ) {
    if(sol[i]<100)
      if(sol[i]<10)
	printf("  %d ",sol[i]);
      else
	printf(" %d ",sol[i]);
    else
      printf("%d ",sol[i]);
  }
  
  printf("\n");
  
}

void copia_solucion(int *s1, int *s2, int n){
  
  for (int i=0; i<n ; i++)
    s2[i]=s1[i];
  
}

void costo(QAP qap, int *sol, int &csol){
  
  csol=0;
  for(int i=0; i<qap.n ; i++)
    for(int j=0; j<qap.n; j++)
      csol+=qap.flujo[i][j]*qap.distancia[sol[i]][sol[j]];
  
}

void costo_factorizado(QAP qap, int *sol, int r, int s, int &csol){
  
  csol=0;
  for(int k=0;k<qap.n;k++)
     if(k!=r && k!=s)
       csol+=qap.flujo[r][k]*(qap.distancia[sol[s]][sol[k]]-qap.distancia[sol[r]][sol[k]])+
	      qap.flujo[s][k]*(qap.distancia[sol[r]][sol[k]]-qap.distancia[sol[s]][sol[k]])+
	      qap.flujo[k][r]*(qap.distancia[sol[k]][sol[s]]-qap.distancia[sol[k]][sol[r]])+
	      qap.flujo[k][s]*(qap.distancia[sol[k]][sol[r]]-qap.distancia[sol[k]][sol[s]]);
}

bool GeneraVecinos(int *sol, int n, int &r, int &s, int *vecino, bool reiniciar){
  
  static int i=0,j=1,nv=1;
  
  if(reiniciar) { i=0;j=1; }
  if(i==n-1) return true;
  
  r=i; s=j;
  
  copia_solucion(sol,vecino,n);
  
  intercambia(vecino[i],vecino[j]);
  
  if(j>=n-1){
    i++;
    j=i+1;
  }else
    j++;
  
  nv++;
  
  return false;
  
}

void GeneraVecinoAleatorio(int *sol, int n, int &r, int &s, int *vecino){
  
  r=Randint(0,n-1);
  
  do
    s=Randint(0,n-1);
  while(r==s);

  if(vecino[r]>vecino[s])
  	intercambia(r,s);

  copia_solucion(sol,vecino,n);
  intercambia(vecino[r],vecino[s]);
  
}

void GeneraSolucionAleatoria(int *sol, int n){
  
  std::set<int> aux;
  int r;
  
  for(int i=0; i<n; i++){
    while(!(aux.insert(r=Randint(0,n-1))).second);
    sol[i]=r;
  }
  
}

void greedy(QAP qap, int *sol,int &csol){
  
  int *pf,*pd; // potenciales de flujo y distancia
  int *pfi, *pdi; // indices de los potenciales de flujo y distancia ORDENADOS
  
  pf = (int *)malloc(sizeof(int) * qap.n);
  pd = (int *)malloc(sizeof(int) * qap.n);
  pfi = (int *)malloc(sizeof(int) * qap.n);
  pdi = (int *)malloc(sizeof(int) * qap.n);
  
  // Calcular los potenciales de flujo y distancia
  int i,j;
  for(i=0; i < qap.n; i++){
    pf[i]=pd[i]=0,sol[i]=0;
    for(j=0; j < qap.n; j++){
      pf[i]+=qap.flujo[i][j];
      pd[i]+=qap.distancia[i][j];
    }
  }
  
  // Ordenar los potenciales de menor a mayor
  quicksort(pf,qap.n,pfi);
  quicksort(pd,qap.n,pdi);
  
  // sol[i]: localizaciones, i: unidades
  // emparejar la menor distancia con el mayor flujo
  for ( i = 0 ; i < qap.n ; i++ ){
    sol[pfi[qap.n-i-1]] = pdi[i];
  }
  
  costo(qap,sol,csol);
  
  free(pf);
  free(pd);
  free(pfi);
  free(pdi);
  
}

void greedy(QAP qap, int *sol,int &csol, float &t){
  
  start_timers();
  
  greedy(qap, sol, csol);
  
  t=elapsed_time(VIRTUAL);
  
}

void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol){
  
  int *svec=(int *)malloc(sizeof(int) * qap.n), r, s, coste_vec, n_evaluaciones = 0;
  bool espaciocompleto=false,reiniciar=true;
  
  int caux=0;

  costo(qap,sol,csol);
  
  do{
    do{

      espaciocompleto=GeneraVecinos(sol,qap.n,r,s,svec,reiniciar);
      reiniciar=false;
      costo_factorizado(qap,sol,r,s,coste_vec);
	    n_evaluaciones++;

    }while(coste_vec>=0 && !espaciocompleto);

    if(coste_vec<0){
      reiniciar=true;
      copia_solucion(svec,sol,qap.n);
      csol=csol+coste_vec;
    }

  }while(coste_vec<0 && n_evaluaciones<25000);

  free(svec);

}

void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t){
 
  start_timers();

  BusquedaLocalPrimeroMejor(qap, sol, csol);
  
  t=elapsed_time(VIRTUAL);
  
}

void inicializarSCH(int ** L, vector<int> &unidades, vector<int> &localizaciones, int n){

  L=(int **)malloc(N_HOR*sizeof(int *));

  for(int i=0; i<N_HOR;i++){
    L[i]=(int *)malloc(sizeof(int) * n);
  }

  for (int i=0; i < n; i++ ){
    unidades.push_back(i);
    localizaciones.push_back(i);
  }
  
}


void SCH(QAP qap, int *sol, int &csol, float &t){

  start_timers();

  int n_evaluaciones=0, **L, **feromona, **heuristica, r;

  const int MAX_EVALUACIONES=25000;
  std::set<int> aux;

  vector<int> unidades, localizaciones;

  inicializarSCH(L, unidades, localizaciones, qap.n);

  while(n_evaluaciones<MAX_EVALUACIONES){

    aux.clear();
    for(int k=0;k<N_HOR;k++){
      while(!(aux.insert(r=Randint(0,qap.n-1))).second);
      L[k][0]=r;      
      // L[k][0] <- nodo inicial
    }

    for(int i=1;i<qap.n;i++){
      for(int k=0; k<N_HOR;k++){
        L[k][i] = ReglaTransición (L[k], feromona , heuristica );
        // Actualización_local_feromona (L[k][i-1],L[k][i])
      }
    }

    for(int k=0; k<10;k++){

    }
  }
  t=elapsed_time(VIRTUAL);
}

/*
void SCH(QAP qap, int *sol, int &csol, float &t){

  start_timers();

  int n_evaluaciones=0, **L, r;

  const int MAX_EVALUACIONES=25000;
  std::set<int> aux;

  vector<int> unidades, localizaciones;

  inicializarSCH(L, unidades, localizaciones, qap.n);

  pf = (int *)malloc(sizeof(int) * qap.n);
  pfi = (int *)malloc(sizeof(int) * qap.n);

  for(int i=0; i < qap.n; i++){
    pf[i]=0;
    for(int j=0; j < qap.n; j++)
      pf[i]+=qap.flujo[i][j];
  }

  quicksort(pf,qap.n,pfi);

  vector<pair<int, int> > L;


  for (int i=0; i < qap.n; i++){

  }



  while(n_evaluaciones<MAX_EVALUACIONES){

  }
  t=elapsed_time(VIRTUAL);
}*/