#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int c;
  int* profit;
  int* weight;
} instance;

typedef struct paraOrden{
  int id;
  double ratio;
}paraOrden;

//https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
int compare_paraOrden(const void *a, const void *b) {
  const paraOrden *p1=(paraOrden *) a;
  const paraOrden *p2=(paraOrden *) b;
  if(p1->ratio > p2->ratio) return -1;
  else if(p1->ratio < p2->ratio) return 1;
  return 0;
}

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

double * generateDoubleVector(int n) {
	double *vector;
	vector=(double* )malloc(sizeof(double)*n);
	return vector;
}

int readFile(instance* I,char* filename) {

FILE* f;
int uno,dos;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&uno,&dos);
  I->n=uno;
  I->c=dos;
  I->profit=generateIntVector(I->n);
  I->weight=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d %d\n",&(I->weight[i]),&(I->profit[i]));
  } 
  fclose(f);
  return(0);
}

int solveKnapsackGreedy(instance* I) {
  paraOrden orden[I->n];
  int libre;
  int obj;
  int c;
  double r;

  for(int i=0;i<I->n;i++) {
    orden[i].id=i;
    orden[i].ratio=(double)(I->profit[i])/(double)(I->weight[i]);
  }
  //https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
  qsort(orden,I->n,sizeof(paraOrden),compare_paraOrden);

  /* alternativa mala
  do {
    c=0;
    for(int i=0;i<I->n-1;i++) {
      if(orden[i].ratio<orden[i+1].ratio) {
        c=orden[i].id;
        orden[i].id=orden[i+1].id;
        orden[i+1].id=c;
        c=1;
        r=orden[i].ratio;
        orden[i].ratio=orden[i+1].ratio;
        orden[i+1].ratio=r;
      }
    }
  }while(c==1); 
  */
  /*
  printf("orden: ");
  for(int i=0;i<I->n;i++) {
    printf("%d ",orden[i].id);
  }
  printf("\n");
  */
  libre=I->c;
  obj=0;
  //printf("usa: ");
  for(int i=0;i<I->n;i++) {
    if(libre>=I->weight[orden[i].id]) {
      libre=libre-I->weight[orden[i].id];
      obj=obj+I->profit[orden[i].id];
      //printf("%d ",orden[i].id);
    }
  }
  printf("\nbeneficio total: %d\n",obj);

  return(0);
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
  solveKnapsackGreedy(&I);
  free(I.profit);
  free(I.weight);
  return(0);
}
