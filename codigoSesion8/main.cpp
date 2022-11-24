#include <stdio.h>
#include <stdlib.h>

typedef struct instance{
  int n;
  int m;
  int* w;
} instance;

typedef struct solucion{
  int* x; //el vector para cada tarea (item) la máquina (bin) que hace la tarea
  int* carga; //para cada máquina la suma de las w de las tareas asignadas
  int maxCarga; //el máximo entre las cargas (la c, el valor de la función objetivo)
  int maquinaMaxCarga; //qué máquina es crítica
}solucion;

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

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

void greedy(instance* I,solucion* S) { //obtener una primera solución (copia de la semana pasada)

  S->x=generateIntVector(I->n); 
  S->carga=generateIntVector(I->m);

  for(int j=0;j<I->m;j++) S->carga[j]=0; 
  for(int i=0;i<I->n;i++) { 
    int mejorMaquina=0;
    for(int j=1;j<I->m;j++) {
      if(S->carga[j]<S->carga[mejorMaquina]) {
        mejorMaquina=j;
      }
    }
    S->carga[mejorMaquina] = S->carga[mejorMaquina]+I->w[i];
    S->x[i]=mejorMaquina;
  }
  printf("operaciones: ");
  for(int i=0;i<I->n;i++) printf("(%d) %d ",i,S->x[i]);
  printf("\tcarga: ");
  for(int j=0;j<I->m;j++) printf("(%d) %d ",j,S->carga[j]);
  printf("\t");
  S->maxCarga=S->carga[0];
  S->maquinaMaxCarga=0;
  for(int j=1;j<I->m;j++) {
    if(S->maxCarga<S->carga[j]) {
      S->maxCarga=S->carga[j];
      S->maquinaMaxCarga=j;
    }
  }
  printf("cmax: %d (máquina %d)\n",S->maxCarga,S->maquinaMaxCarga);
}

void busquedalocal(instance* I,solucion* S) {
  int cambio; //si he implementado algún cambio

  do {
    cambio=0;
    //DESDE AQUÍ
    //cambiar una pieza a otra máquina
    for(int i=0;i<I->n;i++) {
      if(S->x[i]==S->maquinaMaxCarga) {
        for(int j=0;j<I->m;j++) {
          if((S->carga[j]+I->w[i])<S->maxCarga) { //si mejora
            S->carga[S->x[i]] = S->carga[S->x[i]]-I->w[i];
            S->carga[j] = S->carga[j]+I->w[i];
            S->x[i]=j;
            cambio=1;
            //actualizar máquina más cargada
            S->maxCarga=S->carga[0];
            S->maquinaMaxCarga=0;
            for(int jj=1;jj<I->m;jj++) {
              if(S->maxCarga<S->carga[jj]) {
                S->maxCarga=S->carga[jj];
                S->maquinaMaxCarga=jj;
              }
            }
            break; //salir del for de máquinas, seguramente ahora la máquina con mayor carga no es la misma
          }
        }
      }
    }
    //HASTA AQUÍ
    //SEGUNDO VECINDARIO
    //intercambiar dos piezas
    for(int i=0;i<I->n;i++) {
      if(S->x[i]==S->maquinaMaxCarga) {
        for(int ii=0;ii<I->n;ii++) {
          if(S->x[ii]!=S->maquinaMaxCarga) {
            //mejora si las dos máquinas tienen carga menor tras el cambio
            if(((S->carga[S->x[i]]-I->w[i]+I->w[ii])<S->maxCarga)&&((S->carga[S->x[ii]]-I->w[ii]+I->w[i])<S->maxCarga)) { //mejora
              cambio=1;
              S->carga[S->x[i]]=S->carga[S->x[i]]-I->w[i]+I->w[ii];
              S->carga[S->x[ii]]=S->carga[S->x[ii]]-I->w[ii]+I->w[i];
              int a=S->x[i];
              S->x[i]=S->x[ii];
              S->x[ii]=a;
              //actualizar máquina más cargada
              S->maxCarga=S->carga[0];
              S->maquinaMaxCarga=0;
              for(int j=1;j<I->m;j++) {
                if(S->maxCarga<S->carga[j]) {
                  S->maxCarga=S->carga[j];
                  S->maquinaMaxCarga=j;
                }
              }
              break; //salir del for de máquinas, seguramente ahora la máquina con mayor carga no es la misma
            }
          }
        }
      }
    }
  }while(cambio==1);
  printf("operaciones: ");
  for(int i=0;i<I->n;i++) printf("(%d) %d ",i,S->x[i]);
  printf("\tcarga: ");
  for(int j=0;j<I->m;j++) printf("(%d) %d ",j,S->carga[j]);
  printf("\tcmax: %d (máquina %d)\n",S->maxCarga,S->maquinaMaxCarga);
}

int main(int argc, char* argv[]) {

instance I;
solucion S;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  printf("n: %d m: %d\n",I.n,I.m);
  greedy(&I,&S); //hasta aquí sería una solución como las desarrolladas hasta la semana pasada
  busquedalocal(&I,&S); //parte nueva -- van a ser un montón de fors
  free(I.w);
  free(S.carga);
  free(S.x);
  return(0);
}

