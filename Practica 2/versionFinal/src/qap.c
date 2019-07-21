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

void MutacionILS(int *sol, int *sol_mutada,int n){
  
  std::set<int> aux;
  std::vector<int> aux2;
  int r,t=n/4,inicial=Randint(0,n-t-1);
  
  copia_solucion(sol,sol_mutada,n);

  for(int i=0; i<t; i++){
    while(!(aux.insert(r=Randint(inicial,inicial+t-1)).second));
    aux2.push_back(r);
  }

  if(t%2==1) t--;
  for(int i=0;i<t;i+=2)
  	intercambia(sol_mutada[aux2[i]],sol_mutada[aux2[i+1]]);
  
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

void greedyAleatorio(QAP qap, int *sol, float alfa, int &csol){
  
  int *pf,*pd; // potenciales de flujo y distancia
  int *pfi, *pdi; // indices de los potenciales de flujo y distancia ORDENADOS
  
  pf = (int *)malloc(sizeof(int) * qap.n);
  pd = (int *)malloc(sizeof(int) * qap.n);
  pfi = (int *)malloc(sizeof(int) * qap.n);
  pdi = (int *)malloc(sizeof(int) * qap.n);
  
  // Calcular los potenciales de flujo y distancia
  int i,j;
  for(i=0; i < qap.n; i++){
    pf[i]=pd[i]=0;sol[i]=-1;
    for(j=0; j < qap.n; j++){
      pf[i]+=qap.flujo[i][j];
      pd[i]+=qap.distancia[i][j];
    }
  }

  // printf("BREAK 1\n");

  // Ordenar los potenciales de menor a mayor
  quicksort(pf,qap.n,pfi);
  quicksort(pd,qap.n,pdi);

  // for(int i=0;i<qap.n;i++){
  	// printf("**%d\t%d\n",pf[i],pd[i]);
  // }  
  vector<int> LRCu, LRCl;

  int uu, ul;

  // PRIMERA PARTE -----------------------------------------------------------------------------------------------------------------

  uu = pf[qap.n-1]-alfa*(pf[qap.n-1]-pf[0]);
  ul = pd[0]+alfa*(pd[qap.n-1]-pd[0]);

  i=0;
  while (pf[i]<=uu && i<qap.n){
  	LRCu.push_back(pfi[i]);
  	i++;
  }
  i=0;
  while (pd[i]<=ul && i<qap.n){
  	LRCl.push_back(pdi[i]);
  	i++;
  }

  int a,b,c,d;
  sol[a=Randint(0,LRCu.size()-1)]=b=Randint(0,LRCl.size()-1);

  while(a==(c=Randint(0,LRCu.size()-1)));
  while(b==(d=Randint(0,LRCl.size()-1)));

  sol[c]=d;

  // SEGUNDA PARTE -----------------------------------------------------------------------------------------------------------------

  vector<pair<int, int> > LC, LRC, costeLC;
  vector<int> solu, soll;

  solu.push_back(a);
  solu.push_back(c);

  soll.push_back(b);
  soll.push_back(d);


int cont;

  for(cont=2; cont<qap.n;cont++){
	  int ic=0;

	  for(i=0;i<qap.n;i++){
	  	for(int k=0;k<qap.n;k++){
	  		if(find(solu.begin(),solu.end(),i)==solu.end() && find(soll.begin(),soll.end(),k)==soll.end()){
	  			LC.push_back(make_pair(i,k));
	  			costeLC.push_back(make_pair(0,ic));
	  			for(int j=0;j<solu.size();j++){
	  				for(int l=0;l<soll.size();l++){
	  					costeLC[ic].first+=qap.flujo[i][solu[j]]*qap.distancia[k][soll[l]];
	  				}
	  			}
	  			ic++;

	  		}
		}
	  }

	  sort(costeLC.begin(),costeLC.end());

	  float u = costeLC[0].first+alfa*(costeLC[costeLC.size()-1].first-costeLC[0].first);

	  i=0;
	  while (costeLC[i].first<=u && i<costeLC.size()){
	  	LRC.push_back(costeLC[i]);
	  	i++;
	  }  

	  i=Randint(0,LRC.size()-1);

	  sol[LC[LRC[i].second].first]=LC[LRC[i].second].second;

	  solu.push_back(LC[LRC[i].second].first);
	  soll.push_back(LC[LRC[i].second].second);

	  LC.erase(LC.begin(),LC.end());
	  LRC.erase(LRC.begin(),LRC.end());
	  costeLC.erase(costeLC.begin(),costeLC.end());
  }

  costo(qap,sol,csol);

  free(pf);
  free(pd);
  free(pfi);
  free(pdi);

  
}

void greedyAleatorio(QAP qap, int *sol,int &csol, float &t){
  
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

void BusquedaMultiarranqueBasica(QAP qap, int *sol, int &csol, float &t){

	start_timers(); t=0;

	int coste_bl, *sol_bl=(int *)malloc(sizeof(int) * qap.n);

	GeneraSolucionAleatoria(sol,qap.n);
	BusquedaLocalPrimeroMejor(qap,sol,csol);

	for(int i=0; i<24; i++){
		GeneraSolucionAleatoria(sol_bl,qap.n);
		BusquedaLocalPrimeroMejor(qap,sol_bl,coste_bl);
		if(coste_bl<csol){
			csol=coste_bl;
			copia_solucion(sol_bl,sol,qap.n);
		}
	}

	t=elapsed_time(VIRTUAL);

}

void GRASP(QAP qap, int *sol, int &csol, float &t){

	start_timers(); t=0;

	int coste_bl, *sol_bl=(int *)malloc(sizeof(int) * qap.n);

	greedyAleatorio(qap,sol,0.3,csol);
	BusquedaLocalPrimeroMejor(qap,sol,csol);

	for(int i=0; i<2; i++){

		greedyAleatorio(qap,sol_bl,0.3,coste_bl);
		BusquedaLocalPrimeroMejor(qap,sol_bl,coste_bl);

		if(coste_bl<csol){
			csol=coste_bl;
			copia_solucion(sol_bl,sol,qap.n);
		}
	}

	t=elapsed_time(VIRTUAL);

}

void BusquedaLocalReiterada(QAP qap, int *sol, int &csol, float &t){

	start_timers(); t=0;

	int coste_blr, *sol_blr=(int *)malloc(sizeof(int) * qap.n);
	GeneraSolucionAleatoria(sol,qap.n);
	BusquedaLocalPrimeroMejor(qap,sol,csol);

	int caux=0;

	for(int i=0; i<24; i++){

		MutacionILS(sol,sol_blr,qap.n);
		BusquedaLocalPrimeroMejor(qap,sol_blr,coste_blr);

		if(coste_blr<csol){
			csol=coste_blr;
			copia_solucion(sol_blr,sol,qap.n);
		}
	}

	t=elapsed_time(VIRTUAL);

}