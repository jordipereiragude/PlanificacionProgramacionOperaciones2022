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

}

int bestFit(instance* I) {
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
