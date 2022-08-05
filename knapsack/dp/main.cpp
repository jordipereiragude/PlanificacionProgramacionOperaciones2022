#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int c;
  int* profit;
  int* weight;
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

int** p; //proft
int** d; //decision
int obj,currentWeight;

  p=generateIntMatrix(I->n,I->c+1);
  d=generateIntMatrix(I->n,I->c+1);
  for(int i=0;i<I->n;i++)  {
    for(int j=0;j<=I->c;j++) {
      p[i][j]=0;
    }
  }
  //primer item
  p[0][I->weight[0]]=I->profit[0];
  d[0][I->weight[0]]=1;
  d[0][0]=0;
  for(int i=1;i<I->n;i++) {
    for(int j=0;j<I->weight[i];j++) {
      p[i][j]=p[i-1][j];
      d[i][j]=0;
    }
    for(int j=I->weight[i];j<=I->c;j++) {
      p[i][j]=p[i-1][j];
      d[i][j]=0;
      if((p[i-1][j-I->weight[i]]+I->profit[i])>p[i][j]) {
        p[i][j]=p[i-1][j-I->weight[i]]+I->profit[i];
        d[i][j]=1;
      }
    }
  }
  obj=0;
  currentWeight=0;
  for(int i=0;i<=I->c;i++) {
    if(obj<p[I->n-1][i]) {
      obj=p[I->n-1][i];
      currentWeight=i;
    }
  }
  printf("objective: %d\n",obj);
  //reconstruimos con t
  for(int i=I->n-1;i>=0;i--) {
    if(d[i][currentWeight]==1) {
      printf("use item %d profit %d\n",i,I->profit[i]);
      currentWeight=currentWeight-I->weight[i];
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

