#include <stdio.h>
#include <stdlib.h>
#define QS_COMPARE(a,b) ((b)-(a))

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

double * generateDoubleVector(int n) {
	double *vector;
	vector=(double* )malloc(sizeof(double)*n);
	return vector;
}

//quicksort ordena vector list usando vector pesos para marcar el orden usa QS_COMPARE para comparaciones
//de primera a última posición, por tanto en este caso generalmente de beg=0 end=n-1 (end se cambia!!!)
//el QS_COMPARE actual orden de mayor a menor
void sort(int list[], double pesos[], int beg, int end) {
	int piv; 
	int tmp;
	int  l,r,p;

	while (beg<end)    // This while loop will avoid the second recursive call
	{
		l = beg; p = beg + (end-beg)/2; r = end;
		piv = list[p];
		//printf("l %d r %d piv: %d (p: %d)\n",l,r,list[p],p);
		//fflush(stdout);
		while (1)
		{
			//printf("l: %d r: %d\t",l,r);
			while ( (l<=r) && ( QS_COMPARE(pesos[list[l]],pesos[piv]) <= 0.0 ) ) l++;
			while ( (l<=r) && ( QS_COMPARE(pesos[list[r]],pesos[piv])  > 0.0 ) ) r--;
			//printf(" -->  l: %d r: %d\n",l,r);
			if (l>r) break;
			tmp=list[l]; list[l]=list[r]; list[r]=tmp;
			if (p==r) p=l;
			l++; r--;
		}
		list[p]=list[r]; list[r]=piv;
		r--;
		//printf("r: %d beg: %d end %d l %d\n",r,beg,end,l);
		//fflush(stdout);
		// Recursion on the shorter side & loop (with new indexes) on the longer
		if ((r-beg)<(end-l))   
		{
			sort(list, pesos, beg, r);
			beg=l;
		}
		else
		{
			sort(list, pesos, l, end);
			end=r;
		}
	}   
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
  double* ratio;
  int* orden;
  int libre;
  int obj;
  int c;

  ratio=generateDoubleVector(I->n);
  orden=generateIntVector(I->n);
  for(int i=0;i<I->n;i++) ratio[i]=(double)(I->profit[i])/(double)(I->weight[i]);
  for(int i=0;i<I->n;i++) orden[i]=i;
  // alternativa buena
  sort(orden,ratio,0,I->n-1);
  //
  /* alternativa mala 
  do {
    c=0;
    for(int i=0;i<I->n-1;i++) {
      if(ratio[orden[i]]<ratio[orden[i+1]]) {
        c=orden[i];
        orden[i]=orden[i+1];
        orden[i+1]=c;
        c=1;
      }
    }
  }while(c==1); 
  */
  libre=I->c;
  obj=0;
  printf("usa: ");
  for(int i=0;i<I->n;i++) {
    if(libre>=I->weight[orden[i]]) {
      libre=libre-I->weight[orden[i]];
      obj=obj+I->profit[orden[i]];
      printf("%d ",orden[i]);
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
  return(0);
}
