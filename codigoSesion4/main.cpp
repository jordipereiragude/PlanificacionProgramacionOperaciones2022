#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int c;
  int* profit;
  int* weight;
} instance;

int * generateIntVector(int n) { //vector de 1 dimensión
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

int** generateIntMatrix(int n, int m) { //vector de dos dimensiones (n y m)
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

int solveKnapsackDp(instance* I) {
  //voy a necesitar guardar memoria para mis dos matrices
  // * -> puntero (acceso a una dimensión de memoria)
  // ** -> puntero a puntero (acceso a dos dimensiones de memoria)
  int** p; //profit acumulado para cada estado (fila) y etapa (columna)
  int** d; //decision (si se usa o no)
  
  //reservar espacio de memoria (generateIntMatrix)
  p=generateIntMatrix(I->n+1,I->c+1);
  d=generateIntMatrix(I->n+1,I->c+1);
  //inicializar
  for(int i=0;i<I->n;i++)  {
    for(int j=0;j<=I->c;j++) {
      p[i][j]=0; //me aseguro que al principio todos son 0 (valor más bajo posible)
    }
  }
  //resolver la recurrencia
  //voy a considerar la primera columna de forma separada porque esa es trivial
  p[0][I->weight[0]]=I->profit[0]; //subir --> escoger objeto
  d[0][I->weight[0]]=1; //subir
  d[0][0]=0; //no subir --> no escoger objeto
  //caso general para todas las columnas que no son la primera
  //para todo el resto de decisiones
  for(int i=1;i<I->n;i++) {
    //considerar el caso "no subir" (aquellos estados con peso acumulado inferior al peso del objeto)
    for(int j=0;j<I->weight[i];j++) {
      p[i][j]=p[i-1][j]; //profit máximo equivale al mismo conocido hasta ahora (etapa / columna anterior)
      d[i][j]=0; //decisión es no seleccionar el objeto
    }
    //considerar el caso "no subir/subir" (aquellos estados con peso acumulado igual o superior al peso del objeto)
    for(int j=I->weight[i];j<I->c;j++) {
      p[i][j]=p[i-1][j]; //por defecto voy a decir que escoge "no subir" --> no seleccionar el item
      d[i][j]=0; //no seleccionar item
      if((p[i-1][j-I->weight[i]]+I->profit[i])>p[i][j]) { //si ese valor (p[i][j] es más pequeño que "subir" (venir de un estado con carga menor + valor del objeto)
        p[i][j]=p[i-1][j-I->weight[i]]+I->profit[i]; //entonces selecciono subir
        d[i][j]=1;
      }
    }
  } 
  //parte 1, determinar la mejor carga posible de todas las cargas
  int obj=0; //mira el mejor obj
  int currentWeight=0;
  for(int i=0;i<=I->c;i++) {
    if(obj<p[I->n-1][i]) { //cuál es el p (profit) más alto 
      obj=p[I->n-1][i];
      currentWeight=i;
    }
  }
  printf("el valor optimo es %d\ty corresponde a cargar la mochila hasta %d\n",obj,currentWeight);

  //parte 2, ir de derecha a izquierda (de final a principio) viendo qué decisión se ha tomado.
  for(int i=I->n-1;i>=0;i--) { //de la última a la primera columna
    if(d[i][currentWeight]==1) { //si ha escogido el objeto hay que actualizar el weight
      printf("use item %d profit %d\n",i,I->profit[i]);
      currentWeight=currentWeight-I->weight[i]; //si ha usado el objeto, cambiame la fila de lectura
    }
  }

  
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
  solveKnapsackDp(&I);
  return(0);
}


