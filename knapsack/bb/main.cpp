#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
int compare_paraOrden(const void *a, const void *b) {
  const paraOrden *p1=(paraOrden *) a;
  const paraOrden *p2=(paraOrden *) b;
  if(p1->ratio > p2->ratio) return -1;
  else if(p1->ratio < p2->ratio) return 1;
  return 0;
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

int* solucion;
int* bestSolucion;
int bestObj;
paraOrden* orden;

int bound(instance* I,int item,int obj,int remaining) {
  for(int i=item;i<I->n;i++) {
    if(I->weight[orden[i].id]<=remaining) {
      remaining -= I->weight[orden[i].id];
      obj += I->profit[orden[i].id];
      if(remaining==0) return(obj);
    } else {
      return(obj+(floor)(I->profit[orden[i].id]*remaining/(double)(I->weight[orden[i].id])));
    }
  }
  return(obj);
}

void branch(instance* I,int item,int obj,int remaining) {
  //veamos si mejora la mejor solución
  if(obj>bestObj) {
    bestObj=obj;
    for(int i=0;i<item;i++) {
      bestSolucion[i]=solucion[orden[i].id];
    }
    for(int i=item;i<I->n;i++) {
      bestSolucion[orden[i].id]=0;
    }
  }
  //no cabe nada más
  if(remaining==0) {
    return;
  }
  if((item+1)==I->n) {
    return;
  }
  //si no, hay que ramificar
  if((remaining>=I->weight[orden[item].id])&&(bound(I,item,obj+I->profit[orden[item].id],remaining-I->weight[orden[item].id])>bestObj)) {
    solucion[orden[item].id]=1;
    branch(I,item+1,obj+I->profit[orden[item].id],remaining-I->weight[orden[item].id]);
  }
  if(bound(I,item+1,obj,remaining)>bestObj) {
    solucion[orden[item].id]=0;
    branch(I,item+1,obj,remaining);
  }
}

void solveKnapsackBB(instance* I) {
  
  //hay que hacer reservar memoria para estructura de orden
	orden=(paraOrden* )malloc(sizeof(paraOrden)*I->n);
  solucion=generateIntVector(I->n);
  bestSolucion=generateIntVector(I->n);
  for(int i=0;i<I->n;i++) {
    orden[i].id=i;
    orden[i].ratio=(double)(I->profit[i])/(double)(I->weight[i]);
  }
  //https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
  qsort(orden,I->n,sizeof(paraOrden),compare_paraOrden);
  bestObj=0;
  for(int i=0;i<I->n;i++) solucion[i]=0;
  //escoger el primer objeto
  solucion[0]=1;
  branch(I,1,I->profit[orden[0].id],I->c-I->weight[orden[0].id]);

  //no escoger el primer objeto
  solucion[0]=0;
  branch(I,1,0,I->c);


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
  solveKnapsackBB(&I);
  return(0);
}

