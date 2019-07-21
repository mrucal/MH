#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "quicksort.h"
#include "random_ppio.h"
#include "timer.h"
#include "qap.h"

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

void costo(QAP qap, int *sol, int &csol){
  
  csol=0;
  for(int i=0; i<qap.n ; i++)
    for(int j=0; j<qap.n; j++)
      csol+=qap.flujo[i][j]*qap.distancia[sol[i]][sol[j]];
  
}

void greedy(QAP qap, int *sol,int &csol, float &t){
  
  start_timers();
  
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
  
  t=elapsed_time(VIRTUAL);
  
}

void copia_solucion(int *s1, int *s2, int n){
  
  for (int i=0; i<n ; i++)
    s2[i]=s1[i];
  
}

bool GeneraVecinos(int *sol, int n, int &r, int &s, int *vecino, bool reiniciar){
  
  static int i=0,j=1,nv=1;
  
  if(reiniciar) { i=0;j=0; }
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

void costo_factorizado(QAP qap, int *sol, int r, int s, int &csol){
  
  csol=0;int a1,a2,a3,a4;
  for(int k=0;k<qap.n;k++)
     if(k!=r && k!=s)
       csol+=qap.flujo[r][k]*(qap.distancia[sol[s]][sol[k]]-qap.distancia[sol[r]][sol[k]])+
	      qap.flujo[s][k]*(qap.distancia[sol[r]][sol[k]]-qap.distancia[sol[s]][sol[k]])+
	      qap.flujo[k][r]*(qap.distancia[sol[k]][sol[s]]-qap.distancia[sol[k]][sol[r]])+
	      qap.flujo[k][s]*(qap.distancia[sol[k]][sol[r]]-qap.distancia[sol[k]][sol[s]]);
}

void GeneraAleatorio(int *sol, int n){
  
  std::set<int> aux;
  int r;
  
  for(int i=0; i<n; i++){
    while(!(aux.insert(r=Randint(0,n-1))).second);
    sol[i]=r;
  }
  
}

void BusquedaLocalPrimeroMejor(QAP qap, int *sol, int &csol, float &t){
 
  start_timers();
  
  int *svec=(int *)malloc(sizeof(int) * qap.n),r,s,csolvec;
  bool espaciocompleto=false,reiniciar=false;
  
  GeneraAleatorio(sol,qap.n);
  
  do{
    do{

      espaciocompleto=GeneraVecinos(sol,qap.n,r,s,svec,reiniciar);
      reiniciar=false;
      costo_factorizado(qap,sol,r,s,csolvec);

    }while(csolvec>=0 && !espaciocompleto);

    if(csolvec<0){
      reiniciar=true;
      copia_solucion(svec,sol,qap.n);
      costo(qap,sol,csol);
      csol=csol+csolvec;
    }

  }while(csolvec<0);

  free(svec);
  
  t=elapsed_time(VIRTUAL);
  
}

float calcularTemperatura(float T, float Tinicial, float Tfinal, int n_enfriamientos){
  
  float beta= (Tinicial-Tfinal)/(n_enfriamientos*Tinicial*Tfinal);
  
  return T/(1+(beta*T));
}

void EnfiramientoSimulado(QAP qap, int *sol, int &csol, float mi, float theta, float max_vecinos, float max_exitos, float &t){

  start_timers();
  
  int *sact, *vecino, coste_sact, coste_mejor, n_exitos=1, r, s, n_enfriamientos=25000/qap.n, dif_costes, coste_vecino;
  float Tinicial=(mi*coste_sact)/(-log(theta)), T=Tinicial, Tfinal=0.001;

  sact =(int *)malloc(sizeof(int) * qap.n);
  vecino =(int *)malloc(sizeof(int) * qap.n);
  
  greedy(qap,sact,coste_sact,t);
  coste_mejor=coste_sact;
  copia_solucion(sact,sol,qap.n);

  for(int i=0;(i<n_enfriamientos) && (n_exitos!=0);i++){

    n_exitos=0;

    for(int j=0;(j<max_vecinos) && (n_exitos<max_exitos);j++){
      
      GeneraVecinoAleatorio(sact,qap.n,r,s,vecino);
      costo_factorizado(qap,sact,r,s,dif_costes);
      coste_vecino = coste_sact + dif_costes;
       
      if((dif_costes<0) || (Rand() < exp(-dif_costes/T))){

        n_exitos++;
      	intercambia(sact[r],sact[s]);
      	coste_sact=coste_vecino;

        if(coste_sact<coste_mejor){
          coste_mejor=coste_sact;
          copia_solucion(sact,sol,qap.n);
        }

      }
    }

    T=calcularTemperatura(T,Tinicial,Tfinal,n_enfriamientos);

  }

  csol=coste_mejor;
  
  free(sact);
  free(vecino);

  t=elapsed_time(VIRTUAL);

}

bool esTabu(Tabu t, std::vector<Tabu> lt){  
  return (std::find(lt.begin(),lt.end(),t)!=lt.end());
}

void actualizaListaTabu(Tabu t, std::vector<Tabu> &lt, int tenencia, int &i){

  lt[i]=t;
  i=(i+1)%tenencia;

}

void actualizaFrecuencia(int **frec, int *sact, int n){

  for(int i=0;i<n;i++)
    frec[sact[i]][i]++;

}

void reinicializarDiversidad(int *sact,int **frec,int n){

  std::vector<int> u(n,0),l(n,0);
  int max=20000,mu=0,ml=0,nul=n,ju=0,il=0;

  for(int i=0;i<n;i++){
    u[i]=i; l[i]=i;
  }

  for(int k=0;k<n;k++){
    for(int i=0;i<u.size();i++){
      max=20000;
      for(int j=0;j<l.size();j++)
        if(max>frec[l[i]][u[j]]){
          max=frec[l[i]][u[j]];
          mu=u[j]; ju=j;
          ml=l[i]; il=i;
        }
    }
    sact[ml]=mu;
    u.erase(u.begin()+ju);
    l.erase(l.begin()+il);  
  }

}

void reinicializar(int *sact, int *mejor, int **frec,int n,std::vector<Tabu> &lt, int &tenencia){

  float p=Rand(), tam=(Rand()>0.5)?0.5:1.5;
  int nuevatenencia=tenencia*tam;

  if(p>0.50)
    reinicializarDiversidad(sact,frec,n);    
  else
    if(p>0.25)
      copia_solucion(mejor,sact,n);
    else
      GeneraAleatorio(sact,n);

  if(nuevatenencia<=0 || nuevatenencia>200)
    nuevatenencia=20;

  std::vector<Tabu> aux(nuevatenencia,Tabu());
  lt.erase(lt.begin(),lt.end());
  lt=aux;
  tenencia=nuevatenencia;

}

void BusquedaTabu(QAP qap, int *sol, int &csol, float &t){

	start_timers();

	int *sact, *vecino, coste_sact, coste_mejor, *coste_vecinos, dif_costes, *r, *s, *aux, **vecinos,**frec, tenencia=qap.n/2, j, ilt=0, n=0, nr=0, n_exitos=0, ns=0;;
  std::vector<Tabu> ListaTabu(tenencia,Tabu());
  bool caspiracion = false;

  sact =(int *)malloc(sizeof(int) * qap.n);
  vecino =(int *)malloc(sizeof(int) * qap.n);
  coste_vecinos = (int *)malloc(sizeof(int)*30);
  aux = (int *)malloc(sizeof(int)*30);
  r = (int *)malloc(sizeof(int)*30);
  s = (int *)malloc(sizeof(int)*30);
  vecinos = (int **)malloc(30*sizeof(int *));
  frec = (int **)malloc(30*sizeof(int *));
  for(int i=0;i<qap.n;i++){
    frec[i] =(int *)malloc(sizeof(int) * qap.n);
    for(int j=0; j<qap.n;j++)
      frec[i][j]=0;
  }

  GeneraAleatorio(sact,qap.n);
  costo(qap,sact,coste_sact);
  coste_mejor=coste_sact;

  while(n<25000 && ns<3000){

    if(nr==10){
      if(n_exitos==0){
        reinicializar(sact,sol,frec,qap.n,ListaTabu,tenencia);
        ilt=0;
      }
      nr=0; n_exitos=0;
    }

    costo(qap,sact,coste_sact);

    for(j=0; j<30;j++){

    	GeneraVecinoAleatorio(sact,qap.n,r[j],s[j],vecino);
    	vecinos[j]=vecino;
      costo_factorizado(qap,sact,r[j],s[j],dif_costes);
    	coste_vecinos[j]=coste_sact+dif_costes;

    }

  	quicksort(coste_vecinos,30,aux);
    j=0;caspiracion = false;
    
  	while(esTabu(Tabu(vecinos[j][r[j]],vecinos[j][s[j]],r[j],s[j]),ListaTabu) && !caspiracion){
      if(coste_vecinos[j]<coste_mejor){ caspiracion=true; j--; }
      j++;
  	}

    copia_solucion(vecinos[j],sact,qap.n);
    coste_sact=coste_vecinos[j];
    actualizaListaTabu(Tabu(vecinos[j][r[j]],vecinos[j][s[j]],r[j],s[j]),ListaTabu,tenencia,ilt);
    actualizaFrecuencia(frec,sact,qap.n);

    if(coste_sact<coste_mejor){

      copia_solucion(sact,sol,qap.n);
      coste_mejor=coste_sact;
      n_exitos++; ns=0;

    }

    n++; nr++; ns++;

  }

  csol=coste_mejor;

	t=elapsed_time(VIRTUAL);

}