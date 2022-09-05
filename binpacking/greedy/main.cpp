#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int c;
  int* w;
} instance;

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

int readFile(instance* I,char* filename) {

FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&(I->n),&(I->c));
  I->w=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d\n",&(I->w[i]));
  } 
  fclose(f);
  return(0);
}

int firstFit(instance* I) {
  int remanente[I->n];
  int solucion[I->n];

  for(int i=0;i<I->n;i++) remanente[i]=I->c;
  for(int i=0;i<I->n;i++) solucion[i]=(-1);
  for(int i=0;i<I->n;i++) {
    for(int j=0;j<I->n;j++) {
      if(remanente[j]>=I->w[i]) {
        remanente[j] = remanente[j] - I->w[i];
        solucion[i]=j;
        break;
      }
    }
  }
  int numBins=0;
  for(int j=0;j<=I->n;j++) {
    if(remanente[j]<I->c) {
      numBins++;
    }
  }
  for(int i=0;i<I->n;i++) {
    printf("%d ",solucion[i]);
  }
  printf("firstFit usa %d cajas\n",numBins);
  return 0;
}

int bestFit(instance* I) {
  int remanente[I->n];
  int solucion[I->n];
  int mejorCaja;
  int mejorCarga;

  for(int i=0;i<I->n;i++) remanente[i]=I->c;
  for(int i=0;i<I->n;i++) solucion[i]=(-1);
  for(int i=0;i<I->n;i++) {
    mejorCaja=(-1);
    mejorCarga=I->c;
    for(int j=0;j<I->n;j++) {
      if((remanente[j]>=I->w[i])&&(mejorCarga>(remanente[j]-I->w[i]))) {
          mejorCaja=j;
          mejorCarga=remanente[j]-I->w[i];
      }
    }
    remanente[mejorCaja] = mejorCarga;
    solucion[i]=mejorCaja;
  }
  int numBins=0;
  for(int j=0;j<=I->n;j++) {
    if(remanente[j]<I->c) {
      numBins++;
    }
  }
  for(int i=0;i<I->n;i++) {
    printf("%d ",solucion[i]);
  }
  printf("bestFit usa %d cajas\n",numBins);
  return 0;
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  printf("n: %d c: %d\n",I.n,I.c);
  firstFit(&I);
  bestFit(&I);
  return(0);
}
