#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int nt; //número de tareas
  int c; //tiempo de ciclo
  int* d; //vector para las duraciones de cada tarea
  int** p; //matriz con el grafo de precedencias
  int* prioridad; //cuanto más alta, más prioritario GUARDA UN NÚMERO PARA CADA TAREA (Y EL MÁS GRANDE, MEJOR)
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

/***********************************************************/
/* HAY QUE IR CON CUIDADO PORQUE LOS DATOS VAN DE 1 a N    */
/* Todo va ir numerado de 1 a n (por tanto reserve         */
/* voy a reservar n+1 posiciones (y voy a dejar la 0 vacía */
/***********************************************************/


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

void calcularPrioridadHelgersonBirnie(instance* I) {
  int pAbs[I->nt+1][I->nt+1];
  for(int i=1;i<=I->nt;i++) {
    for(int j=1;j<I->nt;j++) {
      pAbs[i][j]=I->p[i][j];
    }
  }
  int cambio;
  do{
    cambio=0;
    for(int i=1;i<=I->nt;i++) {
      for(int j=1;j<=I->nt;j++) {
        if(pAbs[i][j]==1) {
          for(int k=1;k<=I->nt;k++) {
            if((pAbs[i][k]==0)&&(pAbs[j][k]==1)) {
              pAbs[i][k]=1;
              cambio=1;
            }
          }
        }
      }
    }
  }while(cambio==1);
  for(int i=1;i<I->nt;i++) {
    I->prioridad[i]=I->d[i];
    for(int j=1;j<I->nt;j++) {
      if(pAbs[i][j]==1) {
        I->prioridad[i]=I->prioridad[i]+I->d[j];
      }
    }
  }
}

int greedy(instance* I) {

  int pendientes[I->nt+1]; //vector de precedentes pendientes (explicado en las láminas)
  for(int i=1;i<=I->nt;i++) {
    pendientes[i]=0;
    for(int j=1;j<I->nt;j++) {
      if(I->p[j][i]==1) pendientes[i]++; //si j es predecesora de i, entonces i tiene una predecesora pendiente (suma de columnas que expliqué en las láminas)
    }
  }

  int solucion[I->nt+1]; //la solución es a qué estación está asignada cada tarea (inicialmente voy a ponerla a 0)
                         //si solucion[i]=0, la tarea no se ha asignado a ninguna estación
  for(int i=1;i<=I->nt;i++) {
    solucion[i]=0;
  }

  int tareasAsignadas=0; //que me avise si he asignado todas las tareas (cuando haya asignado todas las tareas, fin)
  int libre=I->c; //inicializar la primera estación con su tiempo disponible
  int estacion=1; //e identificarla con estación 1

  do { //repetir esto hasta que todas las tareas estén asignadas
    //primera parte. Busca tareas que puedan asignarse a la estación en curso
    //                En vez de guardar todas las tareas asignables, mantengo sólo la mejor
    int best=0; //al principio ninguna
    int bestPrioridad=0; //la que tiene mayor prioridad
    for(int i=1;i<=I->nt;i++) { //paso por todas las tareas
      // 1 (prendientes[i]==0) -> verificar las precedencias
      // 2 (solucion[i]==0)    -> verificar que esta tarea todavía no la hace ninguna estación
      // 3 (libre>=I->d[i])    -> verifica que el tiempo disponible en la estación es suficiente para realizar la tarea
      // 4 (bestPrioridad<I->prioridad[i]) -> verifica si la tarea i sería mejor que la mejor hasta ahora
      if((pendientes[i]==0)&&(solucion[i]==0)&&(libre>=I->d[i])&&(bestPrioridad<I->prioridad[i])) {
        best=i; //actualizo la mejor tarea  
        bestPrioridad=I->prioridad[i]; //guardo su valor de prioridad
      }
    }
    if(best==0) { //hacer lo mismo que hacía arriba (libre=I->c, estacion=estacion+1
      libre=I->c;
      estacion++;
    } else { //tengo que asignar la tarea
      tareasAsignadas++; //he asignado una tarea más (para poder verificar condición de final)
      solucion[best]=estacion; //añado esta tarea a la estación (asignación en las láminas corresponden al vector solucion
      libre=libre-I->d[best]; //reduzco el tiempo libre
      for(int i=1;i<=I->nt;i++) { //restar la fila
        if(I->p[best][i]==1) {
          pendientes[i]--; //decremento en 1
        }
      }
    }
  }while(tareasAsignadas<I->nt); //salir cuando tareasAsignadas>= número de tareas
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
  //calcularPrioridadHelgersonBirnie(&I);
  greedy(&I);

  free(I.d);
  free(I.p); 
  free(I.prioridad);
  return(0);
}
