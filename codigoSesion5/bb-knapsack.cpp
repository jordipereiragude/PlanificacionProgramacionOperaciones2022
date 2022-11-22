#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//definir el problema 
typedef struct instance{
  int n;
  int c;
  int* profit;
  int* weight;
} instance;

//ordenación
typedef struct paraOrden{
  int id;
  double ratio;
}paraOrden;

//vectores
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

//variables globales para guardar la solución en construcción y la mejor solución
int* solucion;
int* bestSolucion;
int bestObj; //valor de la mejor solución
paraOrden* orden; //orden de las variables

//esta función es ad-hoc del problema (depende de cada problema)
int bound(instance* I,int position,int obj,int remaining) {
  for(int i=position;i<I->n;i++) {
    if(I->weight[orden[i].id]<=remaining) { //si cabe la selecciona entera
      remaining -= I->weight[orden[i].id];
      obj += I->profit[orden[i].id];
      if(remaining==0) return(obj);
    } else { //si no cabe sólo añade la parte proporcional que cabe
      return(obj+(floor)(I->profit[orden[i].id]*remaining/(double)(I->weight[orden[i].id])));
    }
  }
  return(obj);
}

//I-> datos del problema
//position -> profundidad
//obj -> valor de la función objetivo acumulado (profit de las variables fijadas a 1)
//remaining -> C-weight de las variables fijadas a 1
void branch(instance* I,int position,int obj,int remaining) {
  int item=orden[position].id; //pasa a item la variable que le toca de ratio (position-> profundidad
  //veamos si mejora la mejor solución
  if(obj>bestObj) { //si mejora la solución, me la quedo
    bestObj=obj;
    for(int i=0;i<I->n;i++) {
      bestSolucion[i]=solucion[i];
    }
  }
  //no cabe nada más
  if(remaining==0) {
    return; //fin
  }
  //si ya estoy en el último objeto
  if((position+1)==I->n) {
    return; //fin
  }
  //si no, hay que ramificar
  // remaining >= weight[item] (condición c, el objeto tiene que caber)
  // bound cota si la cota no es superior a la mejor solución conocida no es necesario ramificar
  if((remaining>=I->weight[item])&&(bound(I,position,obj+I->profit[item],remaining-I->weight[item])>bestObj)) {
    solucion[item]=1; //fije el item a 1
    branch(I,position+1,obj+I->profit[item],remaining-I->weight[item]);
  }
  //la condición de bound hay que comprobarla también con el caso que no uso el item
  if(bound(I,position+1,obj,remaining)>bestObj) {
    solucion[item]=0;
    branch(I,position+1,obj,remaining);
  }
}

//función para inicializar, llamar a memoria, etc.
void solveKnapsackBB(instance* I) {
  
  //hay que hacer reservar memoria para estructura de orden
	orden=(paraOrden* )malloc(sizeof(paraOrden)*I->n);
  solucion=generateIntVector(I->n);
  for(int i=0;i<I->n;i++) {
    solucion[i]=0;
  }
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
  solucion[orden[0].id]=1;
  branch(I,1,I->profit[orden[0].id],I->c-I->weight[orden[0].id]);

  //no escoger el primer objeto
  solucion[orden[0].id]=0;
  branch(I,1,0,I->c);
  printf("mejor solucion con valor: %d\n",bestObj);
  for(int i=0;i<I->n;i++) {
    if(bestSolucion[i]==1) printf("%d ",i);
  }
  printf("\n");


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
  free(I.profit);
  free(I.weight);
  return(0);
}

