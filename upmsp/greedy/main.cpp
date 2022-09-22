#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int m;
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
  fscanf(f,"%d %d\n",&(I->n),&(I->m));
  I->w=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d\n",&(I->w[i]));
  } 
  fclose(f);
  return(0);
}

void greedy(instance* I) {
  int carga[I->m];
  int solucion[I->n];
  for(int j=0;j<I->m;j++) carga[j]=0; 
  for(int i=0;i<I->n;i++) { 
    int mejorMaquina=0;
    for(int j=1;j<I->m;j++) {
      if(carga[j]<carga[mejorMaquina]) {
        mejorMaquina=j;
      }
    }
    carga[mejorMaquina] = carga[mejorMaquina]+I->w[i];
    solucion[i]=mejorMaquina;
  }
  printf("operaciones: ");
  for(int i=0;i<I->n;i++) printf("(%d) %d ",i,solucion[i]);
  printf("\tcarga: ");
  for(int j=0;j<I->m;j++) printf("(%d) %d ",j,carga[j]);
  printf("\t");
  int cmax=carga[0];
  for(int j=1;j<I->m;j++) {
    if(cmax<carga[j]) {
      cmax=carga[j];
    }
  }
  printf("cmax: %d\n",cmax);
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  printf("n: %d m: %d\n",I.n,I.m);
  greedy(&I);
  return(0);
}
