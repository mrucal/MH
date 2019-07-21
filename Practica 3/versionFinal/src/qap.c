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

void GeneraVecinoAleatorio(int *sol, int n/*, int &r, int &s, int *vecino*/){
  
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

void inicializar(int **poblacion, int **padres, int n, int npadres){

  for(int i=0;i<N_CROMOSOMAS;i++){
    poblacion[i]=(int *)malloc(sizeof(int) * n);
    
    GeneraSolucionAleatoria(poblacion[i],n);
  }

  for(int i=0;i<npadres;i++)
    padres[i]=(int *)malloc(sizeof(int) * n);

}

void evaluar(QAP qap, int **poblacion, int *costes_poblacion, int *sol, int &csol){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  for(int i=0;i<N_CROMOSOMAS;i++){
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

void reemplazamientoG(QAP qap, int **poblacion, int **padres, int *costes_padres, int *sol, int csol){

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

  free(indices);

}

void reemplazamientoE(QAP qap, int **poblacion, int *costes_poblacion, int **padres, int *costes_padres, int *sol, int csol){

  int *aux=(int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *indices=(int *)malloc(sizeof(int) * N_CROMOSOMAS);

  copia_solucion(costes_poblacion,aux,N_CROMOSOMAS);

  quicksort(aux,N_CROMOSOMAS,indices);

  if(costes_padres[0]<costes_poblacion[indices[N_CROMOSOMAS-1]]){
    copia_solucion(padres[0],poblacion[indices[N_CROMOSOMAS-1]],qap.n);
    costes_poblacion[indices[N_CROMOSOMAS-1]]=costes_padres[0];
    if(costes_padres[1]<costes_poblacion[indices[N_CROMOSOMAS-2]]){
      copia_solucion(padres[1],poblacion[indices[N_CROMOSOMAS-2]],qap.n);
      costes_poblacion[indices[N_CROMOSOMAS-2]]=costes_padres[1];
    }
  }else{
    if(costes_padres[0]<costes_poblacion[indices[N_CROMOSOMAS-2]]){
      copia_solucion(padres[0],poblacion[indices[N_CROMOSOMAS-2]],qap.n);
      costes_poblacion[indices[N_CROMOSOMAS-2]]=costes_padres[0];
      if(costes_padres[1]<costes_poblacion[indices[N_CROMOSOMAS-2]]){
        copia_solucion(padres[1],poblacion[indices[N_CROMOSOMAS-1]],qap.n);
        costes_poblacion[indices[N_CROMOSOMAS-1]]=costes_padres[1];
      }
    }else{      
      if(costes_padres[1]<costes_poblacion[indices[N_CROMOSOMAS-1]]){
        copia_solucion(padres[1],poblacion[indices[N_CROMOSOMAS-1]],qap.n);
        costes_poblacion[indices[N_CROMOSOMAS-1]]=costes_padres[1];
        if(costes_padres[0]<costes_poblacion[indices[N_CROMOSOMAS-2]]){
          copia_solucion(padres[0],poblacion[indices[N_CROMOSOMAS-2]],qap.n);
          costes_poblacion[indices[N_CROMOSOMAS-2]]=costes_padres[0];
        }
      }else{
        
        if(costes_padres[1]<costes_poblacion[indices[N_CROMOSOMAS-2]]){
          copia_solucion(padres[1],poblacion[indices[N_CROMOSOMAS-2]],qap.n);
          costes_poblacion[indices[N_CROMOSOMAS-2]]=costes_padres[1];
          if(costes_padres[0]<costes_poblacion[indices[N_CROMOSOMAS-1]]){
            copia_solucion(padres[0],poblacion[indices[N_CROMOSOMAS-1]],qap.n);
            costes_poblacion[indices[N_CROMOSOMAS-1]]=costes_padres[0];
          }
        }
      }
    }
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

void cruceOX(QAP qap, int**padres, int npadres, int ncruces, int nsubcadena){

  vector<int> aux;
  vector<int> aux2;
  vector<pair<int, int> > correspondencias;

  int inicial= (qap.n/2)-(nsubcadena/2);
  int k;

  int *hijo= (int *)malloc(sizeof(int) * qap.n);

  if(ncruces%2==1) ncruces--;
  for(int i=0;i<ncruces;i++){

    for(int j=0;j<inicial;j++)
      aux.push_back(padres[i*2][j]);

    for(int j=inicial;j<inicial+nsubcadena;j++)
      hijo[j]=padres[i*2][j];

    for(int j=inicial+nsubcadena;j<qap.n;j++)
      aux.push_back(padres[i*2][j]);

    for(int j=0;j<qap.n;j++)
      if(find(aux.begin(),aux.end(),padres[(i*2)+1][j])!=aux.end())
        aux2.push_back(padres[(i*2)+1][j]);

    k=0;

    for(int j=inicial+nsubcadena;j<qap.n;j++){
      hijo[j]=aux2[k];
      k++;
    }

    for(int j=0;j<inicial;j++){
      hijo[j]=aux2[k];
      k++;
    }

    copia_solucion(hijo,padres[i*2],qap.n);

    aux.clear();
    aux2.clear();

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

void mutacion(QAP qap, int **padres, int npadres, float pm){

  for(int i=0;i<npadres;i++)
    if(pm<Rand())
      GeneraVecinoAleatorio(padres[i], qap.n);

}

void AGGposicion(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * N_CROMOSOMAS);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*N_CROMOSOMAS /2, nmutaciones = pm*qap.n*N_CROMOSOMAS;

  inicializar(poblacion,padres,qap.n,N_CROMOSOMAS);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i++){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,N_CROMOSOMAS);
    crucePosicion(qap, padres, ncruces);
    mutacionG(qap, padres, nmutaciones);
    reemplazamientoG(qap, poblacion, padres, costes_padres, sol, csol);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
  }

  t=elapsed_time(VIRTUAL);

}

void AGEposicion(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(2*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * 2);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*2 /2, nmutaciones = pm*qap.n*2;

  inicializar(poblacion,padres,qap.n,2);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i++){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,2);
    crucePosicion(qap, padres, ncruces);
    mutacion(qap, padres, 2, pm);
    reemplazamientoE(qap, poblacion, costes_poblacion, padres, costes_padres, sol, csol);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
  }

  t=elapsed_time(VIRTUAL);

}

void AGGox(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * N_CROMOSOMAS);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*N_CROMOSOMAS /2, nmutaciones = pm*qap.n*N_CROMOSOMAS;

  inicializar(poblacion,padres,qap.n,N_CROMOSOMAS);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i++){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,N_CROMOSOMAS);
    cruceOX(qap, padres,N_CROMOSOMAS, ncruces,qap.n/4);
    mutacionG(qap, padres, nmutaciones);
    reemplazamientoG(qap, poblacion, padres, costes_padres, sol, csol);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
  }

  t=elapsed_time(VIRTUAL);

}

void AGEox(QAP qap, int *sol, int &csol, float pc, float pm, float &t){

  start_timers(); 

  int **poblacion=(int **)malloc(N_CROMOSOMAS*sizeof(int *));
  int **padres=(int **)malloc(2*sizeof(int *));
  int *costes_poblacion = (int *)malloc(sizeof(int) * N_CROMOSOMAS);
  int *costes_padres = (int *)malloc(sizeof(int) * 2);

  int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);

  int ncruces = pc*2 /2, nmutaciones = pm*qap.n*2;

  inicializar(poblacion,padres,qap.n,2);
  evaluar(qap,poblacion,costes_poblacion,sol,csol);

  for(int i=0;i<25000;i++){
    seleccion(qap,poblacion,costes_poblacion,padres,costes_padres,2);
    cruceOX(qap, padres,2, ncruces,qap.n/4);
    mutacion(qap, padres, 2, pm);
    reemplazamientoE(qap, poblacion, costes_poblacion, padres, costes_padres, sol, csol);
    evaluar(qap,poblacion,costes_poblacion,sol_blr,coste_blr);

    if(coste_blr<csol){
      csol=coste_blr;
      copia_solucion(sol_blr,sol,qap.n);
    }
  }

  t=elapsed_time(VIRTUAL);

}