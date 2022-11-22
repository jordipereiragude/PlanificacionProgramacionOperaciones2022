#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int m;
  int** p; //tiempo de proceso
} instance;

typedef struct paraOrden{
  int id;
  int peso;
}paraOrden;

//https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
int compare_paraOrden(const void *a, const void *b) {
  const paraOrden *p1=(paraOrden *) a;
  const paraOrden *p2=(paraOrden *) b;
  if(p1->peso > p2->peso) return -1;
  else if(p1->peso < p2->peso) return 1;
  return 0;
}


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

int readFile(instance* I,char* filename) {
  FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&(I->n),&(I->m));
  I->p=generateIntMatrix(I->n,I->m);
  for(int i=0;i<I->n;i++) {
    for(int j=0;j<I->m;j++) {
      fscanf(f,"%d\n",&(I->p[i][j]));
    }
  }
  fclose(f);
  return(0);
}

int max(int a,int b) {
  if(a<b) return b;
  else return a;
}

int objective(instance* I,int* solucion,int n) {
  int obj=0;
  int t[I->m];
  
  for(int j=0;j<I->m;j++) t[j]=0;

  for(int i=0;i<n;i++) {
    t[0]=t[0]+I->p[solucion[i]][0];
    for(int j=1;j<I->m;j++) {
      t[j] = max(t[j],t[j-1])+I->p[solucion[i]][j];
    }
  }
  return(t[I->m-1]);
}

void insertar(int* origen,int* destino,int id,int lengthOrigen,int posicion) {
  for(int i=0;i<posicion;i++) destino[i]=origen[i];
  destino[posicion]=id;
  for(int i=posicion;i<lengthOrigen;i++) destino[i+1]=origen[i];
}

//elegir entre las tareas ardías la que tiene mayor w,
//en caso que ninguna tardía escoger la de menor d
void neh(instance* I) {
  int solucion[I->n];
  int solucionTest[I->n];
  int solucionBest[I->n];
  paraOrden orden[I->n];

  for(int i=0;i<I->n;i++) {
    orden[i].id=i;
    orden[i].peso=0;
    for(int j=0;j<I->m;j++) orden[i].peso += I->p[i][j];
  }
  //https://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
  qsort(orden,I->n,sizeof(paraOrden),compare_paraOrden);
  //check orden de las dos primeras
  solucion[0]=orden[0].id;
  solucion[1]=orden[1].id;
  int o1=objective(I,solucion,2);
  solucionTest[0]=orden[1].id;
  solucionTest[1]=orden[0].id;
  int o2=objective(I,solucionTest,2);
  if(o2<o1) {
    solucion[0]=solucionTest[0];
    solucion[1]=solucionTest[1];
  }
  for(int i=2;i<I->n;i++) {
    //testear todas las secuencias y quedarse con la mejor
    int best=RAND_MAX;
    for(int pos=0;pos<=i;pos++) {
      insertar(&solucion[0],&solucionTest[0],orden[i].id,i,pos);
      int o=objective(I,solucionTest,i+1);
      if(o<best) {
        best=o;
        for(int j=0;j<=i;j++) solucionBest[j]=solucionTest[j];
      }
    }
    for(int j=0;j<=i;j++) solucion[j]=solucionBest[j];
  }
  printf("solucion: ");
  for(int i=0;i<I->n;i++) printf("%d ",solucion[i]);
  printf("obj: %d\n",objective(I,solucion,I->n));
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  neh(&I);
  free(I.p);
  return(0);
}
