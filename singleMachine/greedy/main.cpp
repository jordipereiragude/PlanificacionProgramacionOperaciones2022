#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int* p; //tiempo de proceso
  int* w; //peso
  int* d; //due date
} instance;

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

int readFile(instance* I,char* filename) {
  FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d\n",&(I->n));
  I->p=generateIntVector(I->n);
  I->w=generateIntVector(I->n);
  I->d=generateIntVector(I->n);
  for(int i=0;i<I->n;i++) fscanf(f,"%d\n",&(I->p[i]));
  for(int i=0;i<I->n;i++) fscanf(f,"%d\n",&(I->w[i]));
  for(int i=0;i<I->n;i++) fscanf(f,"%d\n",&(I->d[i]));
  fclose(f);
  return(0);
}

int objective(instance* I,int* solucion) {
  int obj=0;
  int t=0;

  for(int i=0;i<I->n;i++) {
    t += I->p[solucion[i]];
    if(t>I->d[solucion[i]]) obj += I->w[solucion[i]]*(t-I->d[solucion[i]]);
  }
  return(obj);
}

//elegir entre las tareas tardías la que tiene mayor w,
//en caso que ninguna tardía escoger la de menor d
void greedy(instance* I) {
  int solucion[I->n];
  int enSolucion[I->n];
  int t=0;
  for(int i=0;i<I->n;i++) enSolucion[i]=0; //no está en la solucion
  for(int i=0;i<I->n;i++) { //para cada posicion
    int selected=0;
    int maxW=0;
    for(int j=0;j<I->n;j++) {
      if((enSolucion[j]==0)&&((t+I->p[j])>I->d[j])&&(maxW<I->w[j])) {
        selected=j;
        maxW=I->w[j];
      }
    }
    if(maxW==0) {
      int minD=RAND_MAX;
      for(int j=0;j<I->n;j++) {
        if((enSolucion[j]==0)&&(minD>I->d[j])) {
          selected=j;
          minD=I->d[j];
        }
      }
    }
    enSolucion[selected]=1;
    solucion[i]=selected;
    t += I->p[selected];
  }
  printf("solucion: ");
  for(int i=0;i<I->n;i++) printf("%d ",solucion[i]);
  printf("obj: %d\n",objective(I,&solucion[0]));
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  greedy(&I);
  return(0);
}
