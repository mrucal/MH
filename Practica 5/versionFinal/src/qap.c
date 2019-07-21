
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

void GeneraVecinoAleatorio(int *sol, int n){
  
  int r,s;

  r=Randint(0,n-1);
  
  do
    s=Randint(0,n-1);
  while(r==s);

  intercambia(sol[r],sol[s]);
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

void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, int &n_evaluaciones){
  
  int *svec=(int *)malloc(sizeof(int) * qap.n), r, s, coste_vec;
  bool espaciocompleto=false,reiniciar=true;
  
  int caux=0, nvecinos=0;

  costo(qap,sol,csol);
  
  do{
    nvecinos=0;
    n_evaluaciones++;
    do{

      espaciocompleto=GeneraVecinos(sol,qap.n,r,s,svec,reiniciar);
      reiniciar=false;
      costo_factorizado(qap,sol,r,s,coste_vec);
      
      nvecinos++;

    }while(coste_vec>=0 && !espaciocompleto && nvecinos<400);

    if(coste_vec<0){
      reiniciar=true;
      copia_solucion(svec,sol,qap.n);
      csol=csol+coste_vec;
    }

  }while(coste_vec<0);

  free(svec);

}

void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t, int &n_evaluaciones){
 
  start_timers();

  BusquedaLocalPrimeroMejor(qap, sol, csol, n_evaluaciones);
  
  t=elapsed_time(VIRTUAL);
  
}

void inicializar(int **poblacion, int **padres, int n, int npadres){

  for(int i=0;i<N_CROMOSOMAS;i++){
    poblacion[i]=(int *)malloc(sizeof(int) * n);
    
    GeneraSolucionAleatoria(poblacion[i],n);
  }

  for(int i=0;i<npadres;i++)
    padres[i]=(int *)malloc(sizeof(int) * n);

}

void evaluar(QAP qap, int **poblacion, int *costes_poblacion, int *sol, int &csol,bool aplicar_local=false){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  for(int i=0;i<N_CROMOSOMAS;i++){
    if(!aplicar_local)
      costo(qap,poblacion[i],costes_poblacion[i]);
    aux[i]=costes_poblacion[i];
  }

  quicksort(aux,N_CROMOSOMAS,indices);
  copia_solucion(poblacion[indices[0]],sol,qap.n);
  csol=aux[0];

  free(aux);
  free(indices);

}

void seleccion(QAP qap, int **poblacion, int *costes_poblacion,int**padres, int *costes_padres, int npadres){

  int j,k;
  for(int i=0;i<npadres;i++){

      j=Randint(0,N_CROMOSOMAS-1);  

      do
         k=Randint(0,N_CROMOSOMAS-1);
      while(j==k);

      if(costes_poblacion[j]<costes_poblacion[k]){
        copia_solucion(poblacion[j],padres[i],qap.n);
        costes_padres[i]=costes_poblacion[j];
      }
      else{
        copia_solucion(poblacion[k],padres[i],qap.n);
        costes_padres[i]=costes_poblacion[k];
      }

    }

}

void reemplazamientoT(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, bool aplicar_local=false){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  bool mejor_perdido=true;

  for(int i=0;i<N_CROMOSOMAS;i++){
    aux[i]=costes_padres[i];
    if(aux[i]==csol) mejor_perdido=false;
    copia_solucion(padres[i],poblacion[i],qap.n);
  }

  if(mejor_perdido){
    quicksort(aux,N_CROMOSOMAS,indices);
    copia_solucion(sol,poblacion[indices[N_CROMOSOMAS-1]],qap.n);
  }

  if(aplicar_local)
    for(int i=0;i<N_CROMOSOMAS;i++)
      BusquedaLocalPrimeroMejor(qap,poblacion[i],costes_poblacion[i],n_evaluaciones);

  free(indices);

}

void reemplazamientoS(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, float pls, bool aplicar_local=false){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  bool mejor_perdido=true;

  for(int i=0;i<N_CROMOSOMAS;i++){
    aux[i]=costes_padres[i];
    if(aux[i]==csol) mejor_perdido=false;
    copia_solucion(padres[i],poblacion[i],qap.n);
  }

  if(mejor_perdido){
    quicksort(aux,N_CROMOSOMAS,indices);
    copia_solucion(sol,poblacion[indices[N_CROMOSOMAS-1]],qap.n);
  }

  if(aplicar_local)
    for(int i=0;i<N_CROMOSOMAS;i++){
      if(Rand()<pls)
        BusquedaLocalPrimeroMejor(qap,poblacion[i],costes_poblacion[i],n_evaluaciones);
    }

  free(indices);

}

void reemplazamientoM(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol, int &n_evaluaciones, bool aplicar_local=false){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  bool mejor_perdido=true;

  for(int i=0;i<N_CROMOSOMAS;i++){
    aux[i]=costes_padres[i];
    if(aux[i]==csol) mejor_perdido=false;
    copia_solucion(padres[i],poblacion[i],qap.n);
  }

  if(mejor_perdido){
    quicksort(aux,N_CROMOSOMAS,indices);
    copia_solucion(sol,poblacion[indices[N_CROMOSOMAS-1]],qap.n);
  }

  if(aplicar_local){
    quicksort(aux,N_CROMOSOMAS,indices);
    for(int i=0;i<N_CROMOSOMAS*0.1;i++)
      BusquedaLocalPrimeroMejor(qap,poblacion[indices[N_CROMOSOMAS-i-1]],costes_poblacion[indices[N_CROMOSOMAS-i-1]],n_evaluaciones);
  }

  free(indices);

}

void crucePosicion(QAP qap, int**padres,int ncruces){

  vector<int> posiciones;
  vector<int> asignaciones;

  int pos;

  if(ncruces%2==1) ncruces--;
  for(int i=0;i<ncruces;i++){
    for(int j=0;j<qap.n;j++){
      if(padres[i*2][j]!=padres[(i*2)+1][j]){
        posiciones.push_back(j);
        asignaciones.push_back(padres[i*2][j]);
      }
    }
    while(posiciones.size()!=0){
      pos=Randint(0,posiciones.size()-1);
      padres[i*2][posiciones[pos]]=asignaciones[0];
      posiciones.erase(posiciones.begin()+pos);
      asignaciones.erase(asignaciones.begin());
    }
  }

}

void mutacionG(QAP qap, int **padres, int nmutaciones){

  set<int> posiciones;
  int p;

  for(int i=0;i<nmutaciones;i++){
    while(!(posiciones.insert(p=Randint(0,N_CROMOSOMAS-1))).second);
    GeneraVecinoAleatorio(padres[p], qap.n);
  }

}

void MemeticoTodos(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * N_CROMOSOMAS);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*N_CROMOSOMAS /2, nmutaciones = pm*qap.n*N_CROMOSOMAS, ngeneraciones = 0;

  bool aplicar_local = false;

  inicializar(poblacion,padres,qap.n,N_CROMOSOMAS);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i+=N_CROMOSOMAS){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,N_CROMOSOMAS);
    crucePosicion(qap, padres, ncruces);
    mutacionG(qap, padres, nmutaciones);
    aplicar_local = (ngeneraciones==0 && i!=0);
    reemplazamientoT(qap, poblacion, costes_poblacion, padres, costes_padres, sol, csol,i,aplicar_local);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr,aplicar_local);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
    ngeneraciones=(ngeneraciones+1)%10;
  }

  t=elapsed_time(VIRTUAL);

}

void MemeticoSubconjunto(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * N_CROMOSOMAS);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*N_CROMOSOMAS /2, nmutaciones = pm*qap.n*N_CROMOSOMAS, ngeneraciones = 0;

  bool aplicar_local = false;

  inicializar(poblacion,padres,qap.n,N_CROMOSOMAS);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i+=N_CROMOSOMAS){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,N_CROMOSOMAS);
    crucePosicion(qap, padres, ncruces);
    mutacionG(qap, padres, nmutaciones);
    aplicar_local = (ngeneraciones==0 && i!=0);
    reemplazamientoS(qap, poblacion, costes_poblacion, padres, costes_padres, sol, csol,i, 0.1, aplicar_local);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr,aplicar_local);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
    ngeneraciones=(ngeneraciones+1)%10;
  }

  t=elapsed_time(VIRTUAL);

}

void MemeticoMejores(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * N_CROMOSOMAS);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*N_CROMOSOMAS /2, nmutaciones = pm*qap.n*N_CROMOSOMAS, ngeneraciones = 0;

  bool aplicar_local = false;

  inicializar(poblacion,padres,qap.n,N_CROMOSOMAS);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i+=N_CROMOSOMAS){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,N_CROMOSOMAS);
    crucePosicion(qap, padres, ncruces);
    mutacionG(qap, padres, nmutaciones);
    aplicar_local = (ngeneraciones==0 && i!=0);
    reemplazamientoM(qap, poblacion, costes_poblacion, padres, costes_padres, sol, csol,i, aplicar_local);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr,aplicar_local);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
    ngeneraciones=(ngeneraciones+1)%10;
  }

  t=elapsed_time(VIRTUAL);

}