#include <stdio.h>
#include <stdlib.h>

//cómo voy a guardar los datos
typedef struct instance{
  int n; //número de pieza
  int m; //número de máquinas
  int* w; //tiempo de proceso de cada pieza en una máquina 
} instance;

//generar vectores
int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//lectura de datos
int readFile(instance* I,char* filename) {

FILE* f;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&(I->n),&(I->m));
  I->w=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d\n",&(I->w[i]));
  } 
  fclose(f);
  return(0);
}

//algoritmo que hemos descrito
void greedy(instance* I) {
  int carga[I->m]; //tenemos que mantener de alguna manera la carga de trabajo de cada máquina
  int solucion[I->n]; //voy a guardar la asignación (para cada pieza pongo la máquina que la hace, 0,...,m-1 )

  for(int j=0;j<I->m;j++) carga[j]=0;  //inicializo a cero la carga de trabajo.
  for(int i=0;i<I->n;i++) { //miro todas las piezas (una a una en el orden que aparecen en el archivo)
    //**************** Contabilidad interna para detectar "mejor/peor" **************//
    int mejorMaquina=0; //tentativamente digo que es la máquina 0
    for(int j=1;j<I->m;j++) { //para el resto de máquinas
      if(carga[j]<carga[mejorMaquina]) { //si me he equivocado (la carga de la máquina nueva es menor a la carga de la máquina que consideraba mejor)
        mejorMaquina=j; //actualizar la mejor máquina
      }
    }
    //******************************//
    carga[mejorMaquina] = carga[mejorMaquina]+I->w[i]; //hay que aumentarla porque ahora también hace la pieza i
    solucion[i]=mejorMaquina; //la pieza i la hace la mejor máquina
  }
  //primero imprimir qué máquina hace cada pieza
  printf("operaciones: ");
  for(int i=0;i<I->n;i++) printf("(%d) %d ",i,solucion[i]);
  //segundo ver qué carga tiene cada máquina
  printf("\tcarga: ");
  for(int j=0;j<I->m;j++) printf("(%d) %d ",j,carga[j]);
  printf("\t");
  //ver la máquina más cargada (detectar el valor de la función objetivo
  int cmax=carga[0];
  for(int j=1;j<I->m;j++) {
    if(cmax<carga[j]) {
      cmax=carga[j];
    }
  }
  printf("cmax: %d\n",cmax);
}

//ejecutar el programa
int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //lee los datos
  readFile(&I,argv[1]);
  printf("n: %d m: %d\n",I.n,I.m);
  //ejecuta el "greedy"
  greedy(&I);
  free(I.w);
  return(0);
}
