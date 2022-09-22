#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int seed=19835709;

typedef struct instance{
  int n;
  int m;
  int* w;
} instance;

typedef struct ga{
  double mutacion;
  int torneo;
  int nIndividuos;
  int iteraciones;
  int** poblacion;
  int* hijo;
  int* fitnessIndividuos;
  int fitnessHijo;
} ga;

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

int** generateIntMatrix(int n, int m) {
  int i;
  int **matrix;

  if((matrix = (int**)malloc(sizeof(int) * n * m +
		      sizeof(int *) * n	 )) == NULL){
    printf("Out of memory, exit.");
    exit(1);
  }
  for ( i = 0 ; i < n ; i++ ) {
    matrix[i] = (int *)(matrix + n) + i*m;
  }
  return matrix;
}

double r8_uniform_01 (  ) {
  int k;
  double r;

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 )
  {
    seed = seed + 2147483647;
  }
	/*
  Although SEED can be represented exactly as a 32 bit integer,
  it generally cannot be represented exactly as a 32 bit real number!
	*/
  r = ( double ) ( seed ) * 4.656612875E-10;
  return r;
}

int r8_int (int lower,int upper) {
  return (lower+floor(r8_uniform_01()*(double)(upper-lower+1)) );
}

int readFile(instance* I,char* filename) {
  FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&(I->n),&(I->m));
  I->w=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d\n",&(I->w[i]));
  } 
  fclose(f);
  return(0);
}

int evaluarFitness(instance* I,int* solucion) {
  int carga[I->m];
  for(int j=0;j<I->m;j++) carga[j]=0;
  for(int i=0;i<I->n;i++) {
    carga[solucion[i]]=carga[solucion[i]]+I->w[i];
  }
  int maxCarga=carga[0];
  for(int j=1;j<I->m;j++) {
    if(maxCarga<carga[j]) {
      maxCarga=carga[j];
    }
  }
  return(maxCarga);
}

void stats(ga* G,int iteracion) {
  int best=G->fitnessIndividuos[0];
  double average=G->fitnessIndividuos[0];

  for(int i=1;i<G->nIndividuos;i++) {
    average = average + G->fitnessIndividuos[i];
    if(best>G->fitnessIndividuos[i]) {
      best=G->fitnessIndividuos[i];
    }
  }
  printf("it: %d best: %d average: %4.5f\n",iteracion,best,average/(double)(G->nIndividuos));
}

void inicializarPoblacion(ga* G,instance* I) {
  for(int i=0;i<G->nIndividuos;i++) {
    for(int ii=0;ii<I->n;ii++) {
      G->poblacion[i][ii]=r8_int(0,I->m-1);
    }
    G->fitnessIndividuos[i]=evaluarFitness(I,&(G->poblacion[i][0]));
  }
}

void crossover(instance* I, ga* G) {
  int enTorneo[G->torneo];
  int diferente;

  for(int i=0;i<G->torneo;i++) {
    do {
      diferente=1;
      enTorneo[i]=r8_int(0,G->nIndividuos-1);
      for(int j=0;j<i;j++) {
        if(enTorneo[i]==enTorneo[j]) diferente=0;
      }
    }while(diferente==0);
  }
  int padre1=enTorneo[0];
  for(int i=1;i<G->torneo;i++) {
    if(G->fitnessIndividuos[padre1]>G->fitnessIndividuos[enTorneo[i]]) {
      padre1=enTorneo[i];
    }
  }
  for(int i=0;i<G->torneo;i++) {
    do {
      diferente=1;
      do {
        enTorneo[i]=r8_int(0,G->nIndividuos-1);
      }while(enTorneo[i]==padre1);
      for(int j=0;j<i;j++) {
        if(enTorneo[i]==enTorneo[j]) diferente=0;
      }
    }while(diferente==0);
  }
  int padre2=enTorneo[0];
  for(int i=1;i<G->torneo;i++) {
    if(G->fitnessIndividuos[padre2]>G->fitnessIndividuos[enTorneo[i]]) {
      padre2=enTorneo[i];
    }
  }
  //el hijo contiene informacion de padre1 y padre2
  for(int i=0;i<I->n;i++) {
    int alea=r8_int(0,G->fitnessIndividuos[padre1]+G->fitnessIndividuos[padre2]);
    if(alea<G->fitnessIndividuos[padre1]) G->hijo[i]=G->poblacion[padre2][i];
    else G->hijo[i]=G->poblacion[padre1][i];
  }
}

void mutacion(instance* I,ga* G) {

  for(int i=0;i<I->n;i++) {
    if(G->mutacion>r8_uniform_01()) {
      G->hijo[i]=r8_int(0,I->m-1);
    }
  }
  G->fitnessHijo=evaluarFitness(I,&(G->hijo[0]));
}

void reemplazo(instance* I,ga* G) {
  int enTorneo[G->torneo];

  for(int i=0;i<G->torneo;i++) {
    int diferente;
    do {
      diferente=1;
      enTorneo[i]=r8_int(0,G->nIndividuos-1);
      for(int j=0;j<i;j++) {
        if(enTorneo[i]==enTorneo[j]) diferente=0;
      }
    }while(diferente==0);
  }
  int peor=0;
  for(int i=1;i<G->torneo;i++) {
    if(G->fitnessIndividuos[enTorneo[i]]>G->fitnessIndividuos[enTorneo[peor]]) {
      peor=i;
    }
  }
  for(int i=0;i<I->n;i++) {
    G->poblacion[enTorneo[peor]][i]=G->hijo[i];
  }
  G->fitnessIndividuos[enTorneo[peor]]=G->fitnessHijo;
}

int main(int argc, char* argv[]) {
  instance I;
  ga G;

  if(argc<5) {
    printf("archivo poblacion torneo iteraciones mutacion\n");
    return(0);
  }
  readFile(&I,argv[1]);
  G.nIndividuos=atoi(argv[2]);
  G.torneo=atoi(argv[3]);
  G.iteraciones=atoi(argv[4]);
  G.mutacion=atof(argv[5]);
  G.hijo=generateIntVector(I.n);
  G.poblacion=generateIntMatrix(G.nIndividuos,I.n);
  G.fitnessIndividuos=generateIntVector(G.nIndividuos);
  inicializarPoblacion(&G,&I);
  stats(&G,0);
  for(int i=0;i<G.iteraciones;i++) {
    crossover(&I,&G);
    mutacion(&I,&G);
    reemplazo(&I,&G);
    stats(&G,i);
  }
}
