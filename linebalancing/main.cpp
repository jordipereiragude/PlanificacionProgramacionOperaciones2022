#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int nt;
  int c;
  int* d;
  int** p;
  int* prioridad; //cuanto más alta, más prioritario
} instance;

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

int readFile(instance* I,char* filename,int c) {

FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d\n",&(I->nt));
  I->c=c;
  I->d=generateIntVector(I->nt+1);
  for(int i=1;i<=I->nt;i++){
    fscanf(f,"%d\n",&(I->d[i]));
  } 
  I->p=generateIntMatrix(I->nt+1,I->nt+1);
  for(int i=1;i<=I->nt;i++){
    for(int j=1;j<=I->nt;j++) {
      I->p[i][j]=0;
    }
  } 
  for(;;) {
    int p1,p2;
    fscanf(f,"%d,%d\n",&p1,&p2);
    if(p1==(-1)) break;
    I->p[p1][p2]=1;
  }
  I->prioridad=generateIntVector(I->nt+1);
  fclose(f);
  return(0);
}

void calcularPrioridadDuracion(instance* I) {
  for(int i=1;i<=I->nt;i++) {
    I->prioridad[i]=I->d[i];
  }
}

int greedy(instance* I) {

  int pendientes[I->nt+1];
  int solucion[I->nt+1];
  int tareasAsignadas=0;
  int libre=I->c;
  int estacion=1;

  for(int i=1;i<=I->nt;i++) {
    solucion[i]=0;
  }
  for(int i=1;i<=I->nt;i++) {
    pendientes[i]=0;
    for(int j=1;j<I->nt;j++) {
      if(I->p[j][i]==1) pendientes[i]++;
    }
  }
  do {
    int best=0;
    int bestPrioridad=0;
    for(int i=1;i<=I->nt;i++) {
      if((pendientes[i]==0)&&(solucion[i]==0)&&(libre>=I->d[i])&&(bestPrioridad<=I->prioridad[i])) {
        best=i;
        bestPrioridad=I->prioridad[i];
      }
    }
    if(best==0) {
      libre=I->c;
      estacion++;
    } else {
      tareasAsignadas++;
      solucion[best]=estacion;
      libre=libre-I->d[best];
      for(int i=1;i<=I->nt;i++) {
        if(I->p[best][i]==1) {
          pendientes[i]--;
        }
      }
    }
  }while(tareasAsignadas<I->nt);
  for(int i=1;i<=I->nt;i++) printf("%d ",solucion[i]);
  printf("m: %d\n",estacion);
  return(0);
}

int main(int argc,char* argv[]) {
  instance I;

  if(argc<3) {
    printf("programa instancia ciclo\n");
    return(0);
  }
  readFile(&I,argv[1],atoi(argv[2]));
  calcularPrioridadDuracion(&I);
  greedy(&I);
  return(0);
}
