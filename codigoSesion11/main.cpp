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
  int solucion[I->n]; //un vector para guardar la secuencia de tareas
  int enSolucion[I->n]; //un vector para anotar qué tareas he hecho ya (evitar repeticiones de tareas)
  int t=0; //tiempo actual
  for(int i=0;i<I->n;i++) enSolucion[i]=0; //no está en la solucion
  for(int i=0;i<I->n;i++) { //para cada posicion (for grande que he puesto en las láminas)
    int selected=0;
    int maxW=0; //peso máximo de las tareas tardías (si es 0 equivale a que no hay tardías)0
    for(int j=0;j<I->n;j++) { //primer for (que es el que escoge por tardía)
      if((enSolucion[j]==0)&&((t+I->p[j])>I->d[j])&&(maxW<I->w[j])) { //esta es la lógica que indica, job no en solución, job llega tarde, el peso tiene que ser el mayor si hay varias
        selected=j; //tarea seleccionada
        maxW=I->w[j];
      }
    }
    if(maxW==0) {
      int minD=RAND_MAX;
      for(int j=0;j<I->n;j++) { //segundo for (que es el que escoge por fecha de entrega)
        if((enSolucion[j]==0)&&(minD>I->d[j])) {
          selected=j;
          minD=I->d[j];
        }
      }
    }
    enSolucion[selected]=1; //marcar como en solución
    solucion[i]=selected; //añadir a la solución
    t += I->p[selected]; //actualizar tiempo
  }
  printf("solucion: ");
  for(int i=0;i<I->n;i++) printf("%d ",solucion[i]);
  //calcular la función objetivo y mostrar por pantalla
  printf("obj: %d\n",objective(I,&solucion[0]));
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  readFile(&I,argv[1]);
  greedy(&I);
  free(I.d);
  free(I.w); 
  free(I.p);
  return(0);
}
