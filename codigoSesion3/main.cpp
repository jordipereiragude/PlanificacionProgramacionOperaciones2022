#include <stdio.h>
#include <stdlib.h>

//1o. leer datos
//datos hace 15 días
typedef struct instance{
  int n; //número de items
  int c; //capacidad <=capacidad
  int* profit; //vector profit
  int* weight; //vector weight
} instance;

typedef struct paraOrden{
  int id;
  double ratio;
} paraOrden;

//función de ordenación, obtenida de stack overflow
int compare_paraOrden(const void *a, const void *b) {
  const paraOrden *p1=(paraOrden *) a;
  const paraOrden *p2=(paraOrden *) b;
  if(p1->ratio > p2->ratio) return -1;
  else if(p1->ratio < p2->ratio) return 1;
  return 0;
}

//una rutina para ahorrarnos malloc
int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//leer datos según hace 15 días
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

int solveKnapsack(instance* I) {
  paraOrden orden[I->n];
  int i;
  int aux;
  int cambio;
  double auxDouble;
  int libre; //capacidad libre hasta el momento
  int profitAcumulado;
  int solucion[I->n]; //para guardar la solución

  //1.- calcular los ratios
  for(i=0;i<I->n;i++) {
    orden[i].id=i;
    orden[i].ratio=(double)(I->profit[i])/(double)(I->weight[i]);
  }
  //2.- ordenar
  /*bubblesort
  do
  {
    cambio=0;
    for(i=0;i<I->n-1;i++) {
      if(orden[i].ratio<orden[i+1].ratio) {
        //hay que cambiarlos de orden
        cambio=1;
        aux=orden[i].id;
        orden[i].id=orden[i+1].id;
        orden[i+1].id=aux;
        auxDouble=orden[i].ratio;
        orden[i].ratio=orden[i+1].ratio;
        orden[i+1].ratio=auxDouble;
      }
    }
  }while(cambio==1);
  for(i=0;i<I->n;i++) {
    printf("objeto %d ratio %f\n",orden[i].id,orden[i].ratio);
  }
  */
  //https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
  qsort(orden,I->n,sizeof(paraOrden),compare_paraOrden);
  //3.- escoger
  libre=I->c;
  profitAcumulado=0;
  for(i=0;i<I->n;i++) {
    //3.1. Ver si el objeto cabe
    if(I->weight[orden[i].id]<=libre) {
      //3.2. Si cabe, me lo quedo
      libre=libre-I->weight[orden[i].id];
      profitAcumulado = profitAcumulado + I->profit[orden[i].id];
      solucion[orden[i].id]=1;
    } else {
      //3.3. Si no cabe, no me lo quedo
      solucion[orden[i].id]=0;
    }
  }
  printf("solucion con: %d profit\n",profitAcumulado);
  for(i=0;i<I->n;i++) {
    if(solucion[i]==1) {
      printf("selecciono %d\n",i);
    }
  }
  return(0);
}

//argc -> número de argumentos
//argv[0] -> primer argumento (nombre del programa)
//argv[i] -> i-esimo +1 argumento
int main(int argc, char* argv[]) {
  instance I;

  if(argc<2) {
    printf("al menos escribir nombre del fichero\n");
    return(0);
  }
  printf("Dentro del programa\n");
  readFile(&I,argv[1]);
  printf("nombre fichero %s\tnumero de items %d\n",argv[1],I.n);
  solveKnapsack(&I);
  free(I.profit);
  free(I.weight);
  return(0);
}
